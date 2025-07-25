#include <types.h>
#include <hw/mbox.h>
#include <asm.h>


bool mbox_call(u32 *mbox, u8 ch) {

    u32 addr = (u32)(u64)mbox;
    addr &= ~0xf;       // clear last 4 bits
    addr |= (ch & 0xf); // last 4 bits = channel

    // wait until mailbox is ready for writing
    do { ASM("nop"); } while (MBOX0->status & MBOX_FULL);

    MBOX1->rw = addr;

    // wait for responses
    for (;;) {
        do { ASM("nop"); } while (MBOX0->status & MBOX_EMPTY);

        // check if it is the resonse to the correct mailbox
        if (MBOX0->rw == addr) {
            return mbox[1] == MBOX_SUCCESS;
        }
    }
    return false;
}
