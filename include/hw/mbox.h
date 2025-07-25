#pragma once

#include <types.h>
#include <hw/mmap.h>


typedef struct PACKED {
    u32 rw;
    u32 _res0[3];
    u32 peak;
    u32 sender;
    u32 status;
    u32 cfg;
} mbox_t;


// 0 -> GPU to CPU
// 1 -> CPU to GPU
#define MBOX0       ((mbox_t*)(MBOX_BASE))
#define MBOX1       ((mbox_t*)(MBOX_BASE + sizeof(mbox_t)))


typedef enum {
    POWER   = 0,
    FB      = 1,
    VUART   = 2,
    VCHIQ   = 3,
    LED     = 4,
    BUTTON  = 5,
    TOUCH   = 6,
    PROP0   = 8,    // CPU -> GPU
    PROP1   = 9     // GPU -> CPU
} mbox_channel_t;

typedef enum {
    GET_FIRM_REV    = 0x00000001,
    GET_BOARD_MOD   = 0x00010001,
    GET_BOARD_REV   = 0x00010002,
    GET_MAC_ADDR    = 0x00010003,
    GET_SERIAL      = 0x00010004,
    GET_ARM_MEM     = 0x00010005,
    GET_VC_MEM      = 0x00010006,
    GET_CLOCKS      = 0x00010007,
    GET_CMD_LINE    = 0x00050001,
    GET_GET_DMA_CH  = 0x00060001,

    SET_FB_PHYS_RES = 0x00048003,
    SET_FB_VIRT_RES = 0x00048004,

    SET_FB_VIRT_OFF = 0x00048009,
    SET_FB_DEPTH    = 0x00048005,
    SET_FB_PXL_ORD  = 0x00048006,

    GET_FB          = 0x00040001,
    GET_FB_PITCH    = 0x00040008,
    
    GET_CLOCK_RATE  = 0x00030002
} mbox_tag_id_t;


#define MBOX_FULL       0x80000000  // -> first bit
#define MBOX_EMPTY      0x40000000  // -> second bit
#define MBOX_SUCCESS    0x80000000

#define MBOX_REQUEST    0
#define MBOX_END        0


typedef struct PACKED { 
    u32 id;
    u32 buf_size;
    u32 val_length;
} mbox_tag_t;


typedef struct PACKED ALIGNED(16) {
    u32 size;
    u32 status;
    union {
        mbox_tag_t tag;
        u32 data[256]; 
    };
} mbox_msg_t;
