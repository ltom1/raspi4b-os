#pragma once

#include <vmem.h>


// see info mtree
// might be different for real hardware
#define MMIO_BASE       (KERNEL_BASE + 0xfe000000)

#define AUX_BASE        (MMIO_BASE + 0x00215000)
#define GPIO_BASE       (MMIO_BASE + 0x00200000)
#define MBOX_BASE       (MMIO_BASE + 0x0000b880)
#define IC_BASE         (MMIO_BASE + 0x0000b000)
#define SP804_BASE      (MMIO_BASE + 0x0000b400)
#define SYS_TIMER_BASE  (MMIO_BASE + 0x00003000)
#define DMA_BASE        (MMIO_BASE + 0x00007000)
#define EMMC_BASE       (MMIO_BASE + 0x00300000)
#define CTRL_BASE       (KERNEL_BASE + 0xff800000)
#define GICD_BASE       (KERNEL_BASE + 0xff841000)
#define GICC_BASE       (KERNEL_BASE + 0xff842000)
#define SPIN_CORE_BASE  (KERNEL_BASE + 0x000000d8)


// 2gb
// #define RAM_SIZE    0x7fffffff
// #define RAM_SIZE_GB 2

// 8gb
#define RAM_SIZE    0x200000000
#define RAM_SIZE_GB 8

#define VC_START    0x3b400000
#define VC_SIZE     0x04c00000

#define VC_UNCACHED 0xc0000000
#define VC_CACHED   0x40000000

#define PERI_START  0xfc000000
#define PERI_SIZE   0x04000000
