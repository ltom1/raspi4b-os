.global sl_acquire
sl_acquire:
    ldxr    w1, [x0]
    cbnz    w1, sl_acquire
    mov     w1, #1
    stxr    w2, w1, [x0]
    cbnz    w2, sl_acquire
    dmb     ish
    ret


.global sl_release
sl_release:
    dmb     ish 
    mov     w1, #0
    str     w1, [x0]
    ret
