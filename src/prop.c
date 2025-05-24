#include <types.h>
#include <mbox.h>
#include <dbg.h>
#include <std.h>


u64 prop_get_serial(void) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_GET_SERIAL;
    mbox[3] = 8;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (mbox_call(mbox, MBOX_CH_PROP0)) {
        return ((u64)mbox[6] << 32) + mbox[5];
    } 

    dbg_info("Could not get serial\n");
    return 0;
}


bool prop_get_mac(u8 *buf) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_GET_MAC_ADDR;
    mbox[3] = 6;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer (6 for mac + 2 bytes padding)
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (!mbox_call(mbox, MBOX_CH_PROP0)) {
        dbg_info("Could not get mac address\n");
        return false;
    }

    // copy the mac address into the input buffer
    mem_cpy(buf, (u8*)&mbox[5], 6);
    return true;
}


u64 prop_get_arm_mem(void) {

    u32 ALIGNED(16) mbox[8];

    mbox[0] = 8 * 4;    // mbox size
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_GET_ARM_MEM;
    mbox[3] = 8;        // buffer size
    mbox[4] = 0;        // status
    
    // clear buffer (6 for mac + 2 bytes padding)
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_END;

    if (mbox_call(mbox, MBOX_CH_PROP0)) {
        return (u64)mbox[6];
    }

    dbg_info("Could not get ARM memory\n");
    return 0;
}

