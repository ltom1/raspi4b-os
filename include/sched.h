#pragma once

#include <types.h>


#define SCHED_ON    cur_proc->preempt_count--;
#define SCHED_OFF   cur_proc->preempt_count++;


void schedule(void);
void schedule_on(void);
void schedule_off(void);
