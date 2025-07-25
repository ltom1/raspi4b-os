#include <types.h>
#include <proc.h>
#include <asm.h>
#include <sched.h>


void schedule(void) {

    SCHED_OFF

    u32 next_pid;
    s32 max_counter = -1;

    while (1) {
        for (u64 i = 0; i < N_PROCS; i++) {
            if (!procs[i]) continue;
            if (procs[i]->state != RUNNABLE) continue;

            if (procs[i]->counter > max_counter) {
                max_counter = procs[i]->counter;
                next_pid = i;
            }
        }

        if (max_counter) break;

        for (u64 i = 0; i < N_PROCS; i++) {
            if (!procs[i]) continue;

            procs[i]->counter = (procs[i]->counter >> 1) + procs[i]->prio;
        }
    }

    proc_switch(procs[next_pid]);

    SCHED_ON
}


void schedule_on(void) {
    cur_proc->preempt_count--;
}

void schedule_off(void) {
    cur_proc->preempt_count++;
}
