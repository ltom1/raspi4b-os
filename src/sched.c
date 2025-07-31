#include <types.h>
#include <proc.h>
#include <asm.h>
#include <sched.h>
#include <sl.h>


sl_t sched_lock = 0;


void schedule(void) {

    cur_proc[asm_get_core()]->state = RUNNABLE;

    schedule_off();

    u32 next_pid;
    s32 max_counter = -1;

    while (1) {
        for (u64 i = 1; i < N_PROCS; i++) {
            if (!procs[i]) continue;
            if (procs[i]->state != RUNNABLE) continue;

            if (procs[i]->counter > max_counter) {
                max_counter = procs[i]->counter;
                next_pid = i;
            }
        }

        if (max_counter != -1) break;

        for (u64 i = 1; i < N_PROCS; i++) {
            if (!procs[i]) continue;

            procs[i]->counter = (procs[i]->counter >> 1) + procs[i]->prio;
        }
    }

    proc_switch(procs[next_pid]);

    schedule_on();
}

void schedule_on(void) {
    cur_proc[asm_get_core()]->preempt_count--;
    sl_release(&sched_lock);
}

void schedule_off(void) {
    sl_acquire(&sched_lock);
    cur_proc[asm_get_core()]->preempt_count++;
}
