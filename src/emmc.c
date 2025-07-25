#include <types.h>
#include <gpio.h>
#include <hw/gpio.h>
#include <dbg.h>
#include <hw/emmc.h>
#include <timer.h>
#include <drive.h>
#include <prop.h>
#include <asm.h>


extern emmc_device_t sd;

bool emmc_cmd(u32 cmd, u32 arg, u32 timeout);


bool emmc_reg_wait(u32 *reg, u32 mask, bool set, u32 timeout) {

    for (u32 cycles = 0; cycles <= timeout; cycles++) {
        if ((*reg & mask) ? set : !set) return true;
    }

    return false;
}


bool emmc_status_wait(u32 mask, u32 ms) {

    s32 count = ms;
    while ((EMMC->status & mask) && 
        !(EMMC->int_flags & INT_FLAGS_ERR_ALL) && count--) sleep_qs(1000);

    return !(count <= 0 || (EMMC->int_flags & INT_FLAGS_ERR_ALL));
}


static u32 get_clock_divider(u32 base_clock, u32 target_rate) {
    
    u32 target_div = 1;

    // calculate divider
    if (target_rate <= base_clock) {
        target_div = base_clock / target_rate;

        if (base_clock % target_rate) {
            target_div = 0;
        }
    }

    int div = -1;
    for (int fb = 31; fb >= 0; fb--) {
        u32 bt = (1 << fb);

        // look for most significant set bit
        if (target_div & bt) {
            div = fb;

            // clear the bottom
            target_div &= ~(bt);

            if (target_div) {
                div++;
            }

            break;
        }
    }

    // handle edge cases and ensure the divider is not out of range
    if (div == -1) {
        div = 31;
    }

    if (div >= 32) {
        div = 31;
    }

    if (div != 0) {
        div = (1 << (div - 1));
    }

    if (div >= 0x400) {
        div = 0x3FF;
    }

    u32 low = div & 0xff;           // 8 lsb
    u32 high = (div >> 8) & 0x3;    // 2 msb

    // prepare for the ctrl1 register
    u32 ret = (low << 8) | (high << 6);

    return ret;
}


bool emmc_setup_clock(void) {

    EMMC->ctrl2 = 0;

    u32 rate = prop_get_clock_rate(CLOCK_EMMC);
    sd.base_clock = rate;

    u32 r = EMMC->ctrl1;
    r |= EMMC_CTRL1_CLK_INT_EN; // enable internal clock
    r |= get_clock_divider(rate, SD_CLOCK_ID);
    r &= ~(0xf << 16); // set data timeout unit to max
    r |= (11 << 16);

    EMMC->ctrl1 = r;

    if (!emmc_reg_wait(&EMMC->ctrl1, EMMC_CTRL1_CLK_STABLE, true, 2000)) {
        dbg_info("EMMC: clock not stable\n");
        return false;
    }

    sleep_qs(30000);

    // enable clock
    EMMC->ctrl1 |= EMMC_CTRL1_CLK_ENABLE;

    sleep_qs(30000);

    return true;
}


bool emmc_change_clock_rate(u32 base_clock, u32 hz) {
    
    u32 divider = get_clock_divider(base_clock, hz);

    while((EMMC->status & (EMMC_STAT_CMD_INHIBIT | EMMC_STAT_DAT_INHIBIT)))
        sleep_qs(1000);

    u32 r = EMMC->ctrl1 & ~EMMC_CTRL1_CLK_ENABLE;

    EMMC->ctrl1 = r;

    sleep_qs(3000);

    EMMC->ctrl1 = (r | divider) & ~0xffe0;

    sleep_qs(3000);

    EMMC->ctrl1 = r | EMMC_CTRL1_CLK_ENABLE;

    sleep_qs(3000);

    return true;
}


static bool emmc_transfer_data(u32 cmd) {

    bool write = !(cmd & CMD_DIRECTION);

    u32 int_flags = write ? INT_FLAGS_WRITE_RDY : INT_FLAGS_READ_RDY;

    u32 *data = (u32*)sd.buf;

    for (int block = 0; block < sd.transfer_blocks; block++) {
        // wait for read/write ready
        if (!emmc_reg_wait(&EMMC->int_flags, 
                           int_flags | INT_FLAGS_ERR, true, 5000000)) {
            dbg_info("EMMC: data transfer timeout\n");
            return false;
        }

        u32 r = EMMC->int_flags;

        if ((r & (INT_FLAGS_ERR_ALL | int_flags)) != int_flags) {
            dbg_info("EMMC: data transfer error %x\n", r);
            return false;
        }

        if (write) {
            for (int i = 0; i < sd.block_size; i += 4)
                EMMC->data = *data++;
        } else {
            for (int i = 0; i < sd.block_size; i += 4)
                *data++ = EMMC->data;
        }

    }

    // stop trans not needed
    if (sd.transfer_blocks == 1) return true;

    if(!emmc_cmd(CMD_STOP_TRANS, 0, 2000)) {
        dbg_info("EMMC: stop transfer error\n");
        return false;
    }

    return true;
}


bool emmc_cmd(u32 cmd, u32 arg, u32 timeout) {

    // send APP command if required
    if (cmd & CMD_NEEDS_APP_CMD) {
        if (!emmc_cmd(CMD_APP_CMD, sd.rca << 16, timeout)) {
            dbg_info("EMMC: could not send APP command\n");
            return false;
        }

        cmd &= ~CMD_NEEDS_APP_CMD;
    }

    if (EMMC->status & EMMC_STAT_CMD_INHIBIT) {
        dbg_info("EMMC: command line still used by previous command\n");
        return false;
    }

    #ifdef DBG_EMMC
    dbg_info("EMMC_CMD: %x arg %x\n", cmd, arg);
    #endif

    EMMC->blksizecnt = (sd.transfer_blocks << 16) | sd.block_size;
    EMMC->int_flags = EMMC->int_flags;
    EMMC->arg1 = arg;
    EMMC->cmdtm = cmd;

    sleep_qs(10000);

    // wait for the command to complete or fail 

    if (!emmc_reg_wait(&EMMC->int_flags, 
                       INT_FLAGS_CMD_DONE | INT_FLAGS_ERR_ALL, true, timeout)) {
        dbg_info("EMMC: cmd int wait failed\n");
        return false;
    }

    switch (CMD_RESP_TYPE(cmd)) {
        case RT48:
        case RT48_BUSY:
            sd.resp[0] = EMMC->resp[0];
            break;

        case RT136:
            sd.resp[0] = EMMC->resp[0];
            sd.resp[1] = EMMC->resp[1];
            sd.resp[2] = EMMC->resp[2];
            sd.resp[3] = EMMC->resp[3];
            break;
    }

    // transfer data if required
    if (cmd & CMD_IS_DATA) {
        if (!emmc_transfer_data(cmd)) return false;
    }

    // wait until the transfer is finished
    if (CMD_RESP_TYPE(cmd) == RT48_BUSY || cmd & CMD_IS_DATA) {

        emmc_reg_wait(&EMMC->int_flags, 0x8002, true, 2000);

        u32 intr_val = EMMC->int_flags;

        EMMC->int_flags = 0xFFFF0002;

        if ((intr_val & 0xFFFF0002) != 2 && (intr_val & 0xFFFF0002) != 0x100002) {
            dbg_info("EMMC: wait for transfer finish error %x\n", intr_val);
            return false;
        }

        EMMC->int_flags = 0xFFFF0002;
    }

    return true;
}

// temporary until i find out why the multiblock won't work
static bool emmc_read_block(void *dest, u32 lba) {

    u32 n_secs = 1;

    if (!sd.sdhc) n_secs *= 512;

    sd.buf = dest;
    sd.transfer_blocks = n_secs;
    sd.block_size = 512;

    u32 cmd = sd.transfer_blocks > 1 ? CMD_READ_MULTI : CMD_READ_SINGLE;

    #ifdef DBG_EMMC
    dbg_info("EMMC: reading %u sectors starting at lba %u into %x\n", 
             n_secs, lba, (u64)dest);
    #endif

    u32 retries = 3;
    while (retries != 0) {
        if (emmc_cmd(cmd, lba, 5000)) break;

        dbg_info("EMMC: warn read failed retry\n");
        
        if (!retries) {
            dbg_info("EMMC: read failed\n");
            return false;
        }
        retries--;
    }

    if (!retries) {
        dbg_info("EMMC: read failed\n");
        return false;
    }

    #ifdef DBG_EMMC
    dbg_info("EMMC: read successful\n");
    #endif

    return true;
}

bool emmc_read_blocks(void *dest, u32 lba, u32 n_secs) {

    for (u32 i = 0; i < n_secs; i++) {
        if (!emmc_read_block(dest + i * 512, lba + i)) return false;
    }

    return true;
}

/*
bool emmc_read_blocks(void *dest, u32 lba, u32 n_secs) {

    if (!sd.sdhc) n_secs *= 512;

    sd.buf = dest;
    sd.transfer_blocks = n_secs;
    sd.block_size = 512;

    u32 cmd = sd.transfer_blocks > 1 ? CMD_READ_MULTI : CMD_READ_SINGLE;

    #ifdef DBG_EMMC
    dbg_info("EMMC: reading %u sectors starting at lba %u into %x\n", 
             n_secs, lba, (u64)dest);
    #endif

    u32 retries = 3;
    while (retries != 0) {
        if (emmc_cmd(cmd, lba, 5000)) break;

        dbg_info("EMMC: warn read failed retry\n");
        
        if (!retries) {
            dbg_info("EMMC: read failed\n");
            return false;
        }
        retries--;
    }

    if (!retries) {
        dbg_info("EMMC: read failed\n");
        return false;
    }

    #ifdef DBG_EMMC
    dbg_info("EMMC: read successful\n");
    #endif

    return true;
}
*/

bool emmc_write_blocks(void *src, u32 lba, u32 n_secs) {

    if (!sd.sdhc) n_secs *= 512;

    sd.buf = src;
    sd.transfer_blocks = n_secs;
    sd.block_size = 512;

    u32 cmd = sd.transfer_blocks > 1 ? CMD_WRITE_MULTI : CMD_WRITE_SINGLE;

    #ifdef DBG_EMMC
    dbg_info("EMMC: writing %u sectors starting at lba %u from %x\n", 
             n_secs, lba, (u64)src);
    #endif

    u32 retries = 3;
    while (retries != 0) {
        if (emmc_cmd(cmd, lba, 5000)) break;

        dbg_info("EMMC: warn write failed retry\n");
        
        if (!retries) {
            dbg_info("EMMC: write failed\n");
            return false;
        }
        retries--;
    }

    #ifdef DBG_EMMC
    dbg_info("EMMC: write successful\n");
    #endif

    return true;
}


bool emmc_init(void) {

    u32 r;

    gpio_fsel(34, INPUT);
    gpio_fsel(35, INPUT);
    gpio_fsel(36, INPUT);
    gpio_fsel(37, INPUT);
    gpio_fsel(38, INPUT);
    gpio_fsel(39, INPUT);
    
    gpio_fsel(48, ALT_3);
    gpio_fsel(49, ALT_3);
    gpio_fsel(50, ALT_3);
    gpio_fsel(51, ALT_3);
    gpio_fsel(52, ALT_3);

    sd.base = (drive_t) {
        DRIVE_SD,
        emmc_init,
        emmc_read_blocks,
        emmc_write_blocks
    };


    sd.rca = 0;
    sd.ocr = 0;
    sd.sdhc = false;

    sd.transfer_blocks = 0;
    sd.block_size = 0;

    // reset the sd card

    EMMC->ctrl0 = 0;
    EMMC->ctrl1 = EMMC_CTRL1_RESET_HOST;

    if (!emmc_reg_wait(&EMMC->ctrl1, EMMC_CTRL1_RESET_HOST, false, 2000)) {
        dbg_info("EMMC: reset timeout\n");
        return false;
    }

    // enable VDD1 bus power for sd card, needed for raspi4.

    r = EMMC->ctrl0;
    r |= 0x0f << 8;
    EMMC->ctrl0 = r;

    sleep_qs(3000);

    if (!emmc_setup_clock()) {
        dbg_info("EMMC: clock setup failed\n");
        return false;
    }

    // disable interrupts
    // use interrupt register polling instead
    EMMC->int_enable = 0;
    EMMC->int_flags = 0xffffffff;
    EMMC->int_mask = 0xffffffff;

    sleep_qs(203000);

    if (!emmc_cmd(CMD_GO_IDLE, 0, 2000)) {
        dbg_info("EMMC: go idle error\n");
        return false;
    }

    if (!emmc_cmd(CMD_SEND_IF_COND, 0x1aa, 2000) || sd.resp[0] != 0x1aa) {
        dbg_info("EMMC: unsupported voltage (if cond)\n");
        return false;
    }

    r = 0;
    for (int i = 0; (i < 5) && !(r & ACMD41_CMD_COMPLETE); i++) {
        sleep_cycles(400);
        if (!emmc_cmd(CMD_SD_SEND_OP_COND, 0x51ff8000, 2000)) {
            dbg_info("EMMC: SEND_OP_COND error\n");
            return false;
        }

        r = sd.resp[0];

        dbg_info("EMMC: SEND_OP_COND -> %x\n", r);
    }

    sd.sdhc = (r & ACMD41_CMD_CCS) != 0;

    if (!emmc_cmd(CMD_ALL_SEND_CID, 0, 2000)) {
        dbg_info("EMMC: send cid error\n");
        return false;
    }

    dbg_info("EMMC: CID=%x%x\n", 
             EMMC->resp[1] | ((u64)EMMC->resp[0] << 32), 
             EMMC->resp[3] | ((u64)EMMC->resp[2] << 32));

    if (!emmc_cmd(CMD_SEND_REL_ADDR, 0, 2000)) {
        dbg_info("EMMC: send cid error\n");
        return false;
    }

    sd.rca = (sd.resp[0] >> 16) & 0xffff;
    dbg_info("EMMD: RCA=%x\n", sd.rca);

    emmc_change_clock_rate(sd.base_clock, SD_CLOCK_NORMAL);

    if (!emmc_cmd(CMD_CARD_SELECT, sd.rca << 16, 2000)) {
        dbg_info("EMMC: card select error\n");
        return false;
    }

    if (!emmc_status_wait(EMMC_STAT_DAT_INHIBIT, 500000)) {
        dbg_info("EMMC: cmd wait timeout\n");
        return false;
    }

    // get scr (transfer 1 x 8 byte block)
    sd.transfer_blocks = 1;
    sd.block_size = 8;
    sd.buf = (void*)&sd.scr;

    if (!emmc_cmd(CMD_SEND_SCR, 0, 2000)) {
        dbg_info("EMMC: send scr error\n");
        return false;
    }

    // change bus width if necessary
    if (sd.scr[0] & SCR_SD_BUS_WIDTH_4) {
        if (!emmc_cmd(CMD_SET_BUS_WIDTH, (sd.rca << 16) | 2, 2000)) {
            dbg_info("EMMC: change bus width error\n");
            return false;
        }

        EMMC->ctrl0 |= EMMC_CTRL0_HCTL_DWIDTH;
    }

    dbg_info("EMMC: SCR=%x\n", sd.scr[0] | ((u64)sd.scr[1] << 32));

    dbg_info("EMMC: card reset successful\n");

    return true;
}
