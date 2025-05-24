#ifndef _INT_H
#define _INT_H


// trap frame size
#define S_FRAME_SIZE    256


#define VEC_SYNC_EL1T   0
#define VEC_IRQ_EL1T    1
#define VEC_FIQ_EL1T    2
#define VEC_ERR_EL1T    3

#define VEC_SYNC_EL1H   4
#define VEC_IRQ_EL1H    5
#define VEC_FIQ_EL1H    6
#define VEC_ERR_EL1H    7

#define VEC_SYNC_EL0_64 8
#define VEC_IRQ_EL0_64  9
#define VEC_FIQ_EL0_64  10
#define VEC_ERR_EL0_64  11

#define VEC_SYNC_EL0_32 8
#define VEC_IRQ_EL0_32  9
#define VEC_FIQ_EL0_32  10
#define VEC_ERR_EL0_32  11

#endif
