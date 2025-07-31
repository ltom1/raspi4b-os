#include <types.h>
#include <proc.h>
#include <dbg.h>
#include <alloc.h>
#include <std.h>
#include <elf64.h>
#include <pmem.h>
#include <asm.h>
#include <sched.h>
#include <vmem.h>
#include <smp.h>
#include <sl.h>
#include <fs/vfs.h>


u8 cur_pid = 1;

// initial kernel process
pcb_t kernel_ctx;
pcb_t *cur_proc[N_CORES]= { &kernel_ctx, &kernel_ctx, &kernel_ctx, &kernel_ctx };

pcb_t* procs[N_PROCS] = { &kernel_ctx };

sl_t proc_lock = 0;


pcb_t *proc_create(pcb_t *parent, const char *name, u8 length, file_t *f) {

    if (!elf64_check(f) || length > MAX_NAME) return 0;

    elf64_header_t *elf = (elf64_header_t*)f->data;

    // 1 page:
    //
    // pcb_t
    // ^
    // |
    // kernel stack
    // trapframe
    pcb_t *proc = (pcb_t*)P2V(pmem_alloc(PAGE_SIZE));
    proc->kstack = (u64)((u64)proc + PAGE_SIZE);

    proc->state = UNINITIALIZED;
    proc->pid = cur_pid;

    proc->file = f;
    proc->parent = parent;

    mem_cpy((u8*)&proc->name, (u8*)name, length);

    // create the address space and extract the elf
    proc->pt0 = (u64*)P2V(pmem_alloc(PAGE_SIZE));
    proc->ustack = elf64_extract(proc);
    proc->heap_vaddr = proc->ustack + STACK_SIZE;

    // set up trapframe
    proc->tf = (trapframe_t*)((u64)proc + PAGE_SIZE - sizeof(trapframe_t));
    proc->tf->elr = elf->code_entry;
    proc->tf->sp = proc->ustack + STACK_SIZE;
    proc->tf->pstate = 0; // EL0t

    proc->ctx.lr = (u64)proc_start;
    proc->ctx.sp = (u64)proc->tf;

    procs[cur_pid++] = proc;

    proc->state = RUNNABLE;

    return proc;
}


pcb_t *proc_fork(pcb_t *parent) {

    pcb_t *proc = (pcb_t*)P2V(pmem_alloc(PAGE_SIZE));
    mem_cpy((u8*)proc, (u8*)parent, PAGE_SIZE);
    proc->kstack = (u64)((u64)proc + PAGE_SIZE);

    // proc->pt0 = elf64_copy_address_space(parent);
    proc->pt0 = vmem_copy_address_space(parent->pt0);

    // change to the new kernel stack
    proc->tf = (trapframe_t*)((u64)proc + PAGE_SIZE - sizeof(trapframe_t));
    proc->ctx.sp = (u64)proc->tf;
    proc->tf->regs[0] = cur_pid;    // return the new process id

    if (parent->child) {
        proc->next = parent->child;
        proc->next->prev = proc;
    } else
        proc->next = 0;

    parent->child = proc;
    proc->parent = parent;
    proc->child = 0;
    proc->prev = 0;

    sl_acquire(&proc_lock);
    proc->pid = cur_pid;
    procs[cur_pid++] = proc;
    sl_release(&proc_lock);

    proc->state = RUNNABLE;
    return proc;
}


void proc_switch(pcb_t *new) {

    pcb_t *old = cur_proc[asm_get_core()];

    new->state = RUNNING;

    if (new == old) return;

    cur_proc[asm_get_core()] = new;

    mmu_activate_address_space();

    asm_barrier();
    asm_cache_clean();
    asm_barrier();

    ctx_switch(old, new);
}


void proc_destroy(pcb_t *proc) {

    schedule_off();
    procs[proc->pid] = 0;

    // remove the process from its parent's children linked list
    if (proc->prev)
        proc->prev->next = proc->next;
    else
        proc->parent->child = proc->next;


    if (proc->next)
        proc->next->prev = proc->prev;


    // move all orphaned children to pid 1
    if (proc->child) {
        pcb_t *child = proc->child;

        while (1) {
            child->parent = procs[1];
            if (!child->next) break;
            child = child->next;
        }

        child->next = procs[1]->child;

        if (procs[1]->child)
            procs[1]->child->prev = child;
        procs[1]->child = proc->child;
    }

    vmem_destroy_address_space(proc->pt0);

    pmem_free(V2P(proc), PAGE_SIZE);

    schedule_on();
}
