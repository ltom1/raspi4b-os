#include <types.h>
#include <dma.h>
#include <timer.h>
#include <hw/dma.h>


void dma_chan_open(u8 chan) {

    *DMA_ENABLE |= (1 << chan);
    sleep_qs(3000);
    while (DMA(chan)->cs & DMA_CS_RESET);
}
