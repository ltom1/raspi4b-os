#pragma once

#include <types.h>
#include <layout.h>


#define AUX_ENABLE      ((u32*)(AUX_BASE + 0x04))

#define AUX_MU_IO       ((u32*)(AUX_BASE + 0x40))
#define AUX_MU_IER      ((u32*)(AUX_BASE + 0x44))
#define AUX_MU_IIR      ((u32*)(AUX_BASE + 0x48))
#define AUX_MU_LCR      ((u32*)(AUX_BASE + 0x4c))
#define AUX_MU_MCR      ((u32*)(AUX_BASE + 0x50))
#define AUX_MU_LSR      ((u32*)(AUX_BASE + 0x54))
#define AUX_MU_MSR      ((u32*)(AUX_BASE + 0x58))
#define AUX_MU_SCRATCH  ((u32*)(AUX_BASE + 0x5c))
#define AUX_MU_CTRL     ((u32*)(AUX_BASE + 0x60))
#define AUX_MU_STAT     ((u32*)(AUX_BASE + 0x64))
#define AUX_MU_BAUD     ((u32*)(AUX_BASE + 0x68))
