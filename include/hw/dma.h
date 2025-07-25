#pragma once

#include <types.h>
#include <hw/mmap.h>


typedef struct PACKED {
    u32 ti;
    u32 src_addr;
    u32 dest_addr;
    u32 trans_len;
    u32 stride_2d_mode;
    u32 next_cb;
    u32 res[2];
} dma_cb_t;


typedef struct PACKED {
    u32 cs;
    u32 cb_addr;
    dma_cb_t cb;
    u32 dbg;
} dma_chan_t;


#define DMA(n)          ((dma_chan_t*)(DMA_BASE + n * 0x100))
#define DMA_INT_STATUS  ((u32*)(DMA_BASE + 0xfe0))
#define DMA_ENABLE      ((u32*)(DMA_BASE + 0xff0))


#define DMA_CS_ACTIVE       (1 << 0)
#define DMA_CS_END          (1 << 1)
#define DMA_CS_INT          (1 << 2)
#define DMA_CS_DREQ         (1 << 3)
#define DMA_CS_PAUSED       (1 << 4)
#define DMA_CS_WAIT_FOR_OUTSTANDING_WRITES (1 << 6)
#define DMA_CS_ERROR        (1 << 8)
#define DMA_CS_ABORT        (1 << 30)
#define DMA_CS_RESET        (1 << 31)

#define DMA_CS_PRIO_SHIFT           16
#define DMA_CS_PANIC_PRIO_SHIFT     20

#define DMA_DEFAULT_PRIO            1
#define DMA_DEFAULT_PANIC_PRIO      15


#define DMA_TI_INTEN        (1 << 0)
#define DMA_TI_2D_MODE      (1 << 1)
#define DMA_TI_WAIT_RESP    (1 << 3)
#define DMA_TI_DEST_INC     (1 << 4)
#define DMA_TI_DEST_WIDTH   (1 << 5)
#define DMA_TI_DEST_DREQ    (1 << 6)
#define DMA_TI_DEST_IGNORE  (1 << 7)
#define DMA_TI_SRC_INC      (1 << 8)
#define DMA_TI_SRC_WIDTH    (1 << 9)
#define DMA_TI_SRC_DREQ     (1 << 10)
#define DMA_TI_SRC_IGNORE   (1 << 11)


#define DMA_TI_BURST_LEN_SHIFT      12
#define DMA_TI_PERMAP_SHIFT         16
#define DMA_TI_WAIT_CYCLES_SHIFT    21
