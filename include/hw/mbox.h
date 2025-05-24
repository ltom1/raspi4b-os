#ifndef _MBOX_H
#define _MBOX_H

#include <types.h>
#include <layout.h>


// VPU -> CPU
#define MBOX0_READ      ((u32*)(MBOX_BASE + 0x00))
#define MBOX0_PEAK      ((u32*)(MBOX_BASE + 0x10))
#define MBOX0_SENDER    ((u32*)(MBOX_BASE + 0x14))
#define MBOX0_STATUS    ((u32*)(MBOX_BASE + 0x18))
#define MBOX0_CFG       ((u32*)(MBOX_BASE + 0x1c))

// CPU -> VPU
#define MBOX1_WRITE     ((u32*)(MBOX_BASE + 0x20))
#define MBOX1_PEAK      ((u32*)(MBOX_BASE + 0x30))
#define MBOX1_SENDER    ((u32*)(MBOX_BASE + 0x34))
#define MBOX1_STATUS    ((u32*)(MBOX_BASE + 0x38))
#define MBOX1_CFG       ((u32*)(MBOX_BASE + 0x3c))

#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LED     4
#define MBOX_CH_BUTTON  5
#define MBOX_CH_TOUCH   6
// 7 undefined
#define MBOX_CH_PROP0   8 // ARM -> VC
#define MBOX_CH_PROP1   9 // VC -> ARM

#define MBOX_FULL       0x80000000  // -> first bit
#define MBOX_EMPTY      0x40000000  // -> second bit
#define MBOX_SUCCESS    0x80000000

#define MBOX_TAG_GET_FIRM_REV   0x00000001
#define MBOX_TAG_GET_BOARD_MOD  0x00010001
#define MBOX_TAG_GET_BOARD_REV  0x00010002
#define MBOX_TAG_GET_MAC_ADDR   0x00010003
#define MBOX_TAG_GET_SERIAL     0x00010004
#define MBOX_TAG_GET_ARM_MEM    0x00010005
#define MBOX_TAG_GET_VC_MEM     0x00010006
#define MBOX_TAG_GET_CLOCKS     0x00010007
#define MBOX_TAG_GET_CMD_LINE   0x00050001
#define MBOX_TAG_GET_GET_DMA_CH 0x00060001

#define MBOX_REQUEST    0
#define MBOX_END        0


u8 mbox_call(u32 *mbox, u8 ch);

#endif
