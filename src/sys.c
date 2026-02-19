#include <fs/fat32.h>
#include <fs/vfs.h>
#include <pmem.h>
#include <std.h>
#include <vmem.h>
#include <types.h>
#include <sys.h>
#include <dbg.h>
#include <proc.h>
#include <asm.h>
#include <sched.h>


void sys_invalid(void) {
    dbg_info("INVALID SYSCALL\n");
    asm_irq_disable();
    asm_halt();
}


void sys_debug(u64 val) {

    dbg_info("PID %u [%s]: %x\n", cur_proc[asm_get_core()]->pid, cur_proc[asm_get_core()]->name, val);
}


void sys_exit(void) {

    dbg_info("PID %u [%s]: Terminated\n", cur_proc[asm_get_core()]->pid, cur_proc[asm_get_core()]->name);
    
    proc_destroy(cur_proc[asm_get_core()]);

    schedule();
}


u64 sys_get_pid(void) {
    return cur_proc[asm_get_core()]->pid;
}


u64 sys_alloc_heap(u64 n_bytes) {

    u64 paddr = pmem_alloc(n_bytes);
    u64 vaddr = cur_proc[asm_get_core()]->heap_vaddr;

    vmem_map(cur_proc[asm_get_core()]->pt0, cur_proc[asm_get_core()]->heap_vaddr, paddr, n_bytes, PTE_USER | PTE_NG);

    // todo: make a page hole in between blocks to avoid buffer overflow
    cur_proc[asm_get_core()]->heap_vaddr += (n_bytes + PAGE_SIZE - 1) & ~0xfff;

    dbg_info("PID %u [%s]: ALLOC: %x - %x\n", cur_proc[asm_get_core()]->pid, cur_proc[asm_get_core()]->name, vaddr, cur_proc[asm_get_core()]->heap_vaddr);

    return vaddr;
}


void sys_yield(void) {

    dbg_info("CORE %u: yield PID %u\n", asm_get_core(), cur_proc[asm_get_core()]->pid);

    // schedule
    cur_proc[asm_get_core()]->counter--;
    // current process has time slices left
    if (cur_proc[asm_get_core()]->counter > 0 || cur_proc[asm_get_core()]->preempt_count > 0) {

        dbg_info("CORE %u resumes PID %u\n", asm_get_core(), cur_proc[asm_get_core()]->pid);
        return;
    }

    cur_proc[asm_get_core()]->counter = 0;
    pcb_t *old = cur_proc[asm_get_core()];
    dbg_info("CORE %u gives up PID %u\n", asm_get_core(), cur_proc[asm_get_core()]->pid);
    ctx_save(old);
    cur_proc[asm_get_core()] = &kernel_ctx;
    old->state = RUNNABLE;


    schedule();
}


void sys_set_prio(u8 pid, u64 prio) {

    if (!procs[pid]) {
        dbg_info("ERROR: Unused PID: %u\n", pid);
        asm_irq_disable();
        asm_halt();
    }

    procs[pid]->prio = prio;
}


u64 sys_fork(void) {

    proc_fork(cur_proc[asm_get_core()]);
    return 0;
}


void sys_print(const char *str) {

    dbg_info("CORE %u: PID %u [%s]: %s", asm_get_core(), cur_proc[asm_get_core()]->pid, cur_proc[asm_get_core()]->name, str);
}


u64 sys_spawn(const char *path) {

    dbg_info("PID %u [%s] spawns %s", cur_proc[asm_get_core()]->pid, cur_proc[asm_get_core()]->name, path);
    file_t *f = fat32_load_file(fs, (alloc_t*)&heap, path);

    const char *basename = vfs_get_basename(path);
    pcb_t *proc = proc_create(cur_proc[asm_get_core()], basename, str_len(basename), f);

    if (!proc) {
        dbg_info("PID %u [%s]: Failed to spawn %s", cur_proc[asm_get_core()]->pid, cur_proc[asm_get_core()]->name, path);
        asm_irq_disable();
        asm_halt();
    }

    return proc->pid;
}


const void *sys_tbl[] = { sys_exit, sys_debug, sys_get_pid, sys_set_prio, sys_yield, sys_fork, sys_alloc_heap, sys_print, sys_spawn };
