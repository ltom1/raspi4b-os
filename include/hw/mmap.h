#pragma once

#include <vmem.h>


// see info mtree
// might be different for real hardware
#define MMIO_BASE   (KERNEL_BASE + 0xfe000000)

#define AUX_BASE    (MMIO_BASE + 0x00215000)
#define GPIO_BASE   (MMIO_BASE + 0x00200000)
#define MBOX_BASE   (MMIO_BASE + 0x0000b880)
#define IC_BASE     (MMIO_BASE + 0x0000b000)
#define SP804_BASE  (MMIO_BASE + 0x0000b400)
#define TIMER_BASE  (MMIO_BASE + 0x00003000)
#define CTRL_BASE   (KERNEL_BASE + 0xff800000)
#define GICD_BASE   (KERNEL_BASE + 0xff841000)
#define GICC_BASE   (KERNEL_BASE + 0xff842000)

// 2gb
#define RAM_SIZE    0x7fffffff
#define RAM_SIZE_GB 2
