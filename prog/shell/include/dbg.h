#pragma once

#include <types.h>


void dbg_init(void);
void dbg_info(const char *fmt, ...);

void dbg_putc(char c);
void dbg_puts(const char *str);
void dbg_putu(u64 num);
void dbg_putd(s64 num);
void dbg_putx(u64 hex);
