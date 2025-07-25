.global sys_exit
sys_exit:
    mov     x8, #0
    svc     #0
    ret

.global sys_debug
sys_debug:
    mov     x8, #1
    svc     #0
    ret

.global sys_get_pid
sys_get_pid:
    mov     x8, #2
    svc     #0
    ret

.global sys_set_prio
sys_set_prio:
    mov     x8, #3
    svc     #0
    ret

.global sys_yield
sys_yield:
    mov     x8, #4
    svc     #0
    ret

.global sys_fork
sys_fork:
    mov     x8, #5
    svc     #0
    ret

.global sys_alloc_heap
sys_alloc_heap:
    mov     x8, #6
    svc     #0
    ret

.global sys_print
sys_print:
    mov     x8, #7
    svc     #0
    ret

.global sys_spawn
sys_spawn:
    mov     x8, #8
    svc     #0
    ret
