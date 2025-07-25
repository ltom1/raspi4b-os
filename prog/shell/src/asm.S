.global asm_get_time
asm_get_time:
    mrs     x0, cntpct_el0
    ret
.global asm_get_timer_freq
asm_get_timer_freq:
    mrs     x0, cntfrq_el0
    ret
