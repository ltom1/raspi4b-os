#pragma once

#include <types.h>
#include <func.h>


#define N_LOGS  2048

#define LOG(func_id) do { \
    log[log_idx].core = asm_get_core(); \
    log[log_idx].pid = cur_proc[asm_get_core()]->pid; \
    log[log_idx].func = func_id; \
    log[log_idx].line = __LINE__; \
    log_idx++; \
} while (0)

typedef struct {
    u8 core;
    u8 pid;
    func_id_t func;
    u32 line;
} log_t;


extern log_t log[N_LOGS];
extern u64 log_idx;


void log_print(void);
