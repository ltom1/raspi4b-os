#pragma once

#include <types.h>
#include <int.h>
#include <smp.h>
#include <vmem.h>
#include <fs/vfs.h>
#include <alloc.h>


#define MAX_NAME    16
#define N_REGS      31
#define N_PROCS     32

#define STACK_SIZE  (16 * PAGE_SIZE)


typedef struct {
    u64 regs[31];
    u64 sp;
    u64 elr;
    u64 pstate;
} trapframe_t;


typedef struct {
    u64 x19;
    u64 x20;
    u64 x21;
    u64 x22;
    u64 x23;
    u64 x24;
    u64 x25;
    u64 x26;
    u64 x27;
    u64 x28;
    u64 fp;
    u64 sp;
    u64 lr;
} cpu_ctx_t;

typedef enum { 
    UNINITIALIZED, 
    ZOMBIE,
    SLEEPING, 
    RUNNABLE, 
    RUNNING, 
    BLOCKED 
} proc_state_t;


typedef struct PCB {
    cpu_ctx_t       ctx;
    u8              pid;
    char            name[MAX_NAME];
    file_t          *file;
    u64             *pt0;
    trapframe_t     *tf;

    proc_state_t    state;
    u64             kstack;
    u64             ustack;
    u64             heap_vaddr;

    // scheduling
    s64             counter;
    u64             prio;
    s64             preempt_count;
    
    struct PCB      *parent;
    struct PCB      *child;
    struct PCB      *next;
    struct PCB      *prev;
} pcb_t;


pcb_t *proc_create(pcb_t *parent, const char *name, u8 length, file_t *f);
pcb_t *proc_fork(pcb_t *parent);
void proc_switch(pcb_t *proc);
void proc_destroy(pcb_t *proc);

extern void proc_start(void);
extern void ctx_switch(pcb_t *old, pcb_t *new);

extern pcb_t *cur_proc[N_CORES];
extern pcb_t* procs[N_PROCS];
