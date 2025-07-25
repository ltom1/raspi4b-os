#pragma once

#include "drive.h"
#include <types.h>


#define EMMC_CTRL1_RESET_DATA       (1 << 26)
#define EMMC_CTRL1_RESET_CMD        (1 << 25)
#define EMMC_CTRL1_RESET_HOST       (1 << 24)
#define EMMC_CTRL1_RESET_ALL        (EMMC_CTRL1_RESET_DATA | EMMC_CTRL1_RESET_CMD | EMMC_CTRL1_RESET_HOST)

#define EMMC_CTRL1_CLK_GENSEL       (1 << 5)
#define EMMC_CTRL1_CLK_ENABLE       (1 << 2)
#define EMMC_CTRL1_CLK_STABLE       (1 << 1)
#define EMMC_CTRL1_CLK_INT_EN       (1 << 0)

#define EMMC_CTRL0_HCTL_DWIDTH      (1 << 1)

#define EMMC_STAT_CMD_INHIBIT       (1 << 0)
#define EMMC_STAT_DAT_INHIBIT       (1 << 1)
#define EMMC_STAT_READ_RDY          (1 << 9)
#define EMMC_STAT_WRITE_RDY         (1 << 8)

// frequencies
#define SD_CLOCK_ID         400000
#define SD_CLOCK_NORMAL     25000000
#define SD_CLOCK_HIGH       50000000
#define SD_CLOCK_100        100000000
#define SD_CLOCK_208        208000000

// command masks
#define CMD_NEEDS_APP_CMD   (1 << 31)
#define CMD_ERRORS_MASK     0xfff9c004
#define CMD_RCA_MASK        0xffff0000
#define CMD_RESP_TYPE(cmd)  ((cmd >> 16) & 2)
#define CMD_DIRECTION       (1 << 4)
#define CMD_IS_DATA         (1 << 21)

// commands
#define CMD_GO_IDLE         0x00000000
#define CMD_ALL_SEND_CID    0x02010000
#define CMD_IO_SEND_OP_COND 0x05010000
#define CMD_SEND_REL_ADDR   0x03020000
#define CMD_CARD_SELECT     0x07030000
#define CMD_SEND_IF_COND    0x08020000
#define CMD_STOP_TRANS      0x0C030000
#define CMD_READ_SINGLE     0x11220010
#define CMD_READ_MULTI      0x12220032 // needs CMD12
#define CMD_WRITE_SINGLE    0x18220000
#define CMD_WRITE_MULTI     0x19220022 // needs CMD12
#define CMD_SET_BLOCKCNT    0x1700003a
#define CMD_APP_CMD         0x37000000
#define CMD_SET_BUS_WIDTH   (0x06020000 | CMD_NEEDS_APP_CMD)
#define CMD_SD_SEND_OP_COND (0x29020000 | CMD_NEEDS_APP_CMD)
#define CMD_SEND_SCR        (0x33220010 | CMD_NEEDS_APP_CMD)

#define INT_FLAGS_READ_RDY      (1 << 5)
#define INT_FLAGS_WRITE_RDY     (1 << 4)
#define INT_FLAGS_CMD_DONE      (1 << 0)
#define INT_FLAGS_DAT_DONE      (1 << 1)
#define INT_FLAGS_ERR           (1 << 15)
#define INT_FLAGS_DAT_TIMEOUT   (1 << 20)
#define INT_FLAGS_CMD_TIMEOUT   (1 << 16)
#define INT_FLAGS_ERR_ALL       0x17f8000

#define ACMD41_VOLTAGE          0x00ff8000
#define ACMD41_CMD_COMPLETE     0x80000000
#define ACMD41_CMD_CCS          0x40000000
#define ACMD41_ARG_HC           0x51ff8000

#define SCR_SD_BUS_WIDTH_4      0x00000400
#define SCR_SUPP_SET_BLKCNT     0x02000000

#define EMMC_OK             1
#define EMMC_ERROR          2
#define EMMC_TIMEOUT        3


typedef enum {
    RT_NONE,
    RT136,
    RT48,
    RT48_BUSY 
} emmc_cmd_resp_type_t;


typedef struct PACKED {
    u8 resp_a : 1;
    u8 block_count : 1;
    u8 auto_command : 2;
    u8 direction : 1;
    u8 multiblock : 1;
    u16 resp_b : 10;
    u8 resp_type : 2;
    u8 res0 : 1;
    u8 crc_enable : 1;
    u8 idx_enable : 1;
    u8 is_data : 1;
    u8 type : 2;
    u8 index : 6;
    u8 res1 : 2;
} emmc_cmd_t;


typedef struct PACKED {
    u32 arg2;
    u32 blksizecnt;
    u32 arg1;
    u32 cmdtm;
    u32 resp[4];
    u32 data;
    u32 status;
    u32 ctrl0;
    u32 ctrl1;
    u32 int_flags;
    u32 int_mask;
    u32 int_enable;
    u32 ctrl2;
    u32 force_int;
    u32 boot_timeout;
    u32 dbg_sel;
    u32 extfifo_cfg;
    u32 extfifo_enable;
    u32 tune_step;
    u32 tune_steps_std;
    u32 tune_steps_ddr;
    u32 spi_int_support;
    u32 slot_int_status_version;
} emmc_controller_t;


typedef struct {
    drive_t base;
    u32 base_clock;
    u32 rca;
    u32 ocr;
    u32 scr[2];
    bool sdhc;
    u32 transfer_blocks;
    u32 block_size;
    u32 err;
    u32 int_flags;
    u32 resp[4];
    void *buf;
} emmc_device_t;


#define EMMC    ((emmc_controller_t*)EMMC_BASE)
