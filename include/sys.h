#pragma once

#include <types.h>


void sys_invalid(void);
void sys_exit(void);
void sys_debug(u64 val);
u64 sys_get_pid(void);
void sys_set_prio(u8 pid, u64 prio);
void sys_yield(void);
u64 sys_fork(void);
u64 sys_alloc_heap(u64 n_bytes);
void sys_print(const char *str);
u64 sys_spawn(const char *path);
