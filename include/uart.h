#pragma once

#include <types.h>


void uart1_init(void);
void uart1_putc(char c);
void uart1_puts(const char *str);
char uart1_getc(void);
