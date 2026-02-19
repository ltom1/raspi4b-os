.global ctx_switch
ctx_switch:
    mov x9, sp

    // save context into old->ctx
    stp x19, x20, [x0], #16
    stp x21, x22, [x0], #16
    stp x23, x24, [x0], #16
    stp x25, x26, [x0], #16
    stp x27, x28, [x0], #16
    stp x29, x9,  [x0], #16
    str x30, [x0]

    // restore context from new->ctx
    ldp x19, x20, [x1], #16
    ldp x21, x22, [x1], #16
    ldp x23, x24, [x1], #16
    ldp x25, x26, [x1], #16
    ldp x27, x28, [x1], #16
    ldp x29, x9,  [x1], #16
    ldr x30, [x1]

    mov sp, x9
    ret

.global ctx_save
ctx_save:
    mov x9, sp

    // save context into old->ctx
    stp x19, x20, [x0], #16
    stp x21, x22, [x0], #16
    stp x23, x24, [x0], #16
    stp x25, x26, [x0], #16
    stp x27, x28, [x0], #16
    stp x29, x9,  [x0], #16
    str x30, [x0]

    ret


.global ctx_restore
ctx_restore:

    // restore context from new->ctx
    ldp x19, x20, [x0], #16
    ldp x21, x22, [x0], #16
    ldp x23, x24, [x0], #16
    ldp x25, x26, [x0], #16
    ldp x27, x28, [x0], #16
    ldp x29, x9,  [x0], #16
    ldr x30, [x0]

    mov sp, x9
    ret
