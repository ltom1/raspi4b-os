#ifndef _ARMC_H
#define _ARMC_H

#include <types.h>
#include <layout.h>


#define IRQ0_PENDING0		((u32*)(IC_BASE + 0x200)) // ARM Core 0 IRQ Enabled Interrupt Pending bits [31:0]
#define IRQ0_PENDING1		((u32*)(IC_BASE + 0x204)) // ARM Core 0 IRQ Enabled Interrupt Pending bits [63:32]
#define IRQ0_PENDING2		((u32*)(IC_BASE + 0x208)) // ARM Core 0 IRQ Enabled Interrupt Pending bits [79:64]
#define IRQ0_SET_EN_0		((u32*)(IC_BASE + 0x210)) // Write to Set ARM Core 0 IRQ enable bits [31:0]
#define IRQ0_SET_EN_1		((u32*)(IC_BASE + 0x214)) // Write to Set ARM Core 0 IRQ enable bits [63:32]
#define IRQ0_SET_EN_2		((u32*)(IC_BASE + 0x218)) // Write to Set ARM Core 0 IRQ enable bits[79:64]
#define IRQ0_CLR_EN_0		((u32*)(IC_BASE + 0x220)) // Write to Clear ARM Core 0 IRQ enable bits [31:0]
#define IRQ0_CLR_EN_1		((u32*)(IC_BASE + 0x224)) // Write to Clear ARM Core 0 IRQ enable bits [63:32]
#define IRQ0_CLR_EN_2		((u32*)(IC_BASE + 0x228)) // Write to Clear ARM Core 0 IRQ enable bits [79:64]

#define IRQ_STATUS0		    ((u32*)(IC_BASE + 0x230)) // Interrupt Line bits [31:0]
#define IRQ_STATUS1		    ((u32*)(IC_BASE + 0x234)) // Interrupt Line bits [63:32]
#define IRQ_STATUS2		    ((u32*)(IC_BASE + 0x238)) // Interrupt Line bits [79:64]

#define IRQ1_PENDING0		((u32*)(IC_BASE + 0x240)) // ARM Core 1 IRQ Enabled Interrupt pending bits [31:0]
#define IRQ1_PENDING1		((u32*)(IC_BASE + 0x244)) // ARM Core 1 IRQ Enabled Interrupt pending bits [63:32]
#define IRQ1_PENDING2		((u32*)(IC_BASE + 0x248)) // ARM Core 1 IRQ Enabled Interrupt pending bits [79:64]
#define IRQ1_SET_EN_0		((u32*)(IC_BASE + 0x250)) // Write to Set ARM Core 1 IRQ enable bits [31:0]
#define IRQ1_SET_EN_1		((u32*)(IC_BASE + 0x254)) // Write to Set ARM Core 1 IRQ enable bits [63:32]
#define IRQ1_SET_EN_2		((u32*)(IC_BASE + 0x258)) // Write to Set ARM Core 1 IRQ enable bits[79:64]
#define IRQ1_CLR_EN_0		((u32*)(IC_BASE + 0x260)) // Write to Clear ARM Core 1 IRQ enable bits [31:0]
#define IRQ1_CLR_EN_1		((u32*)(IC_BASE + 0x264)) // Write to Clear ARM Core 1 IRQ enable bits [63:32]
#define IRQ1_CLR_EN_2		((u32*)(IC_BASE + 0x268)) // Write to Clear ARM Core 1 IRQ enable bits [79:64]

#define IRQ2_PENDING0		((u32*)(IC_BASE + 0x280)) // ARM Core 2 IRQ Enabled Interrupt pending bits [31:0]
#define IRQ2_PENDING1		((u32*)(IC_BASE + 0x284)) // ARM Core 2 IRQ Enabled Interrupt pending bits [63:32]
#define IRQ2_PENDING2		((u32*)(IC_BASE + 0x288)) // ARM Core 2 IRQ Enabled Interrupt pending bits [79:64]
#define IRQ2_SET_EN_0		((u32*)(IC_BASE + 0x290)) // Write to Set ARM Core 2 IRQ enable bits [31:0]
#define IRQ2_SET_EN_1		((u32*)(IC_BASE + 0x294)) // Write to Set ARM Core 2 IRQ enable bits [63:32]
#define IRQ2_SET_EN_2		((u32*)(IC_BASE + 0x298)) // Write to Set ARM Core 2 IRQ enable bits[79:64]
#define IRQ2_CLR_EN_0		((u32*)(IC_BASE + 0x2a0)) // Write to Clear ARM Core 2 IRQ enable bits [31:0]
#define IRQ2_CLR_EN_1		((u32*)(IC_BASE + 0x2a4)) // Write to Clear ARM Core 2 IRQ enable bits [63:32]
#define IRQ2_CLR_EN_2		((u32*)(IC_BASE + 0x2a8)) // Write to Clear ARM Core 2 IRQ enable bits [79:64]

#define IRQ3_PENDING0		((u32*)(IC_BASE + 0x2c0)) // ARM Core 3 IRQ Enabled Interrupt pending bits [31:0]
#define IRQ3_PENDING1		((u32*)(IC_BASE + 0x2c4)) // ARM Core 3 IRQ Enabled Interrupt pending bits [63:32]
#define IRQ3_PENDING2		((u32*)(IC_BASE + 0x2c8)) // ARM Core 3 IRQ Enabled Interrupt pending bits [79:64]
#define IRQ3_SET_EN_0		((u32*)(IC_BASE + 0x2d0)) // Write to Set ARM Core 3 IRQ enable bits [31:0]
#define IRQ3_SET_EN_1		((u32*)(IC_BASE + 0x2d4)) // Write to Set ARM Core 3 IRQ enable bits [63:32]
#define IRQ3_SET_EN_2		((u32*)(IC_BASE + 0x2d8)) // Write to Set ARM Core 3 IRQ enable bits[79:64]
#define IRQ3_CLR_EN_0		((u32*)(IC_BASE + 0x2e0)) // Write to Clear ARM Core 3 IRQ enable bits [31:0]
#define IRQ3_CLR_EN_1		((u32*)(IC_BASE + 0x2e4)) // Write to Clear ARM Core 3 IRQ enable bits [63:32]
#define IRQ3_CLR_EN_2		((u32*)(IC_BASE + 0x2e8)) // Write to Clear ARM Core 3 IRQ enable bits [79:64]

#define FIQ0_PENDING0		((u32*)(IC_BASE + 0x300)) // ARM Core 0 FIQ Enabled Interrupt pending bits [31:0]
#define FIQ0_PENDING1		((u32*)(IC_BASE + 0x304)) // ARM Core 0 FIQ Enabled Interrupt pending bits [63:32]
#define FIQ0_PENDING2		((u32*)(IC_BASE + 0x308)) // ARM Core 0 FIQ Enabled Interrupt pending bits [79:64]
#define FIQ0_SET_EN_0		((u32*)(IC_BASE + 0x310)) // Write to Set ARM Core 0 FIQ enable bits [31:0]
#define FIQ0_SET_EN_1		((u32*)(IC_BASE + 0x314)) // Write to Set ARM Core 0 FIQ enable bits [63:32]
#define FIQ0_SET_EN_2		((u32*)(IC_BASE + 0x318)) // Write to Set ARM Core 0 FIQ enable bits[79:64]
#define FIQ0_CLR_EN_0		((u32*)(IC_BASE + 0x320)) // Write to Clear ARM Core 0 FIQ enable bits [31:0]
#define FIQ0_CLR_EN_1		((u32*)(IC_BASE + 0x324)) // Write to Clear ARM Core 0 FIQ enable bits [63:32]
#define FIQ0_CLR_EN_2		((u32*)(IC_BASE + 0x328)) // Write to Clear ARM Core 0 FIQ enable bits [79:64]

#define FIQ1_PENDING0		((u32*)(IC_BASE + 0x340)) // ARM Core 1 FIQ Enabled Interrupt pending bits [31:0]
#define FIQ1_PENDING1		((u32*)(IC_BASE + 0x344)) // ARM Core 1 FIQ Enabled Interrupt pending bits [63:32]
#define FIQ1_PENDING2		((u32*)(IC_BASE + 0x348)) // ARM Core 1 FIQ Enabled Interrupt pending bits [79:64]
#define FIQ1_SET_EN_0		((u32*)(IC_BASE + 0x350)) // Write to Set ARM Core 1 FIQ enable bits [31:0]
#define FIQ1_SET_EN_1		((u32*)(IC_BASE + 0x354)) // Write to Set ARM Core 1 FIQ enable bits [63:32]
#define FIQ1_SET_EN_2		((u32*)(IC_BASE + 0x358)) // Write to Set ARM Core 1 FIQ enable bits[79:64]
#define FIQ1_CLR_EN_0		((u32*)(IC_BASE + 0x360)) // Write to Clear ARM Core 1 FIQ enable bits [31:0]
#define FIQ1_CLR_EN_1		((u32*)(IC_BASE + 0x364)) // Write to Clear ARM Core 1 FIQ enable bits [63:32]
#define FIQ1_CLR_EN_2		((u32*)(IC_BASE + 0x368)) // Write to Clear ARM Core 1 FIQ enable bits [79:64]

#define FIQ2_PENDING0		((u32*)(IC_BASE + 0x380)) // ARM Core 2 FIQ Enabled Interrupt pending bits [31:0]
#define FIQ2_PENDING1		((u32*)(IC_BASE + 0x384)) // ARM Core 2 FIQ Enabled Interrupt pending bits [63:32]
#define FIQ2_PENDING2		((u32*)(IC_BASE + 0x388)) // ARM Core 2 FIQ Enabled Interrupt pending bits [79:64]
#define FIQ2_SET_EN_0		((u32*)(IC_BASE + 0x390)) // Write to Set ARM Core 2 FIQ enable bits [31:0]
#define FIQ2_SET_EN_1		((u32*)(IC_BASE + 0x394)) // Write to Set ARM Core 2 FIQ enable bits [63:32]
#define FIQ2_SET_EN_2		((u32*)(IC_BASE + 0x398)) // Write to Set ARM Core 2 FIQ enable bits[79:64]
#define FIQ2_CLR_EN_0		((u32*)(IC_BASE + 0x3a0)) // Write to Clear ARM Core 2 FIQ enable bits [31:0]
#define FIQ2_CLR_EN_1		((u32*)(IC_BASE + 0x3a4)) // Write to Clear ARM Core 2 FIQ enable bits [63:32]
#define FIQ2_CLR_EN_2		((u32*)(IC_BASE + 0x3a8)) // Write to Clear ARM Core 2 FIQ enable bits [79:64]

#define FIQ3_PENDING0		((u32*)(IC_BASE + 0x3c0)) // ARM Core 3 FIQ Enabled Interrupt pending bits [31:0]
#define FIQ3_PENDING1		((u32*)(IC_BASE + 0x3c4)) // ARM Core 3 FIQ Enabled Interrupt pending bits [63:32]
#define FIQ3_PENDING2		((u32*)(IC_BASE + 0x3c8)) // ARM Core 3 FIQ Enabled Interrupt pending bits [79:64]
#define FIQ3_SET_EN_0		((u32*)(IC_BASE + 0x3d0)) // Write to Set ARM Core 3 FIQ enable bits [31:0]
#define FIQ3_SET_EN_1		((u32*)(IC_BASE + 0x3d4)) // Write to Set ARM Core 3 FIQ enable bits [63:32]
#define FIQ3_SET_EN_2		((u32*)(IC_BASE + 0x3d8)) // Write to Set ARM Core 3 FIQ enable bits[79:64]
#define FIQ3_CLR_EN_0		((u32*)(IC_BASE + 0x3e0)) // Write to Clear ARM Core 3 FIQ enable bits [31:0]
#define FIQ3_CLR_EN_1		((u32*)(IC_BASE + 0x3e4)) // Write to Clear ARM Core 3 FIQ enable bits [63:32]
#define FIQ3_CLR_EN_2		((u32*)(IC_BASE + 0x3e8)) // Write to Clear ARM Core 3 FIQ enable bits [79:64]

#define SWIRQ_SET		    ((u32*)(IC_BASE + 0x3f0)) // Write to Set Software Interrupt sources
#define SWIRQ_CLEAR		    ((u32*)(IC_BASE + 0x3f4)) // Write to Clear Software Interrupt sources

#endif
