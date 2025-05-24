#include <types.h>
#include <mbox.h>
#include <asm.h>


u8 mbox_call(u32 *mbox, u8 ch) {

    u32 addr = (u32)(u64)mbox;
    addr &= ~0xf;       // clear last 4 bits
    addr |= (ch & 0xf); // last 4 bits = channel

    // wait until mailbox is ready for writing
    do { ASM("nop"); } while (*MBOX0_STATUS & MBOX_FULL);

    *MBOX1_WRITE = addr;

    // wait for responses
    for (;;) {
        do { ASM("nop"); } while (*MBOX0_STATUS & MBOX_EMPTY);

        // check if it is the resonse to the correct mailbox
        if (*MBOX0_READ == addr) {
            return mbox[1] == MBOX_SUCCESS;
        }
    }
    return false;
}
