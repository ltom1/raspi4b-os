#include <types.h>
#include <mbox.h>
#include <hw/mbox.h>
#include <dbg.h>
#include <std.h>


u64 prop_get_serial(void) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = GET_SERIAL;
    mbox[3] = 8;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (mbox_call(mbox, PROP0)) {
        return ((u64)mbox[6] << 32) + mbox[5];
    } 

    dbg_info("Could not get serial\n");
    return 0;
}


bool prop_get_mac(u8 *buf) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = GET_MAC_ADDR;
    mbox[3] = 6;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer (6 for mac + 2 bytes padding)
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (!mbox_call(mbox, PROP0)) {
        dbg_info("Could not get mac address\n");
        return false;
    }

    // copy the mac address into the input buffer
    mem_cpy(buf, (u8*)&mbox[5], 6);
    return true;
}


u64 prop_get_mem(u32 tag) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = tag;
    mbox[3] = 8;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer (4 bytes off / 4 bytes size)
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (mbox_call(mbox, PROP0)) {
        return (u64)mbox[6];
    }

    dbg_info("Could not get memory size\n");
    return 0;
}


u32 prop_get_clock_rate(u32 clock) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = GET_CLOCK_RATE;
    mbox[3] = 8;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer (4 bytes clock / 4 bytes hz)
    mbox[5] = clock;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (mbox_call(mbox, PROP0)) {
        return mbox[6];
    }

    dbg_info("Could not get ARM memory\n");
    return 0;
}


u32 prop_get_board_rev(void) {

    u32 ALIGNED(16) mbox[7];

    mbox[0] = 7 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = GET_BOARD_REV;
    mbox[3] = 4;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer (4 bytes off)
    mbox[5] = 0;

    mbox[6] = MBOX_END;

    if (mbox_call(mbox, PROP0)) {
        return mbox[5];
    }

    dbg_info("Could not get board revision\n");
    return 0;
}
