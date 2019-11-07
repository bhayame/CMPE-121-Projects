/*
Brandon Hayame
bhayame@ucsc.edu
This program converts an analog signal to digital, and transmits it across a UART to a Rpi,
and waits to receive the same signal back from it.

*/    

#define readDMA_BYTES_PER_BURST 1
#define readDMA_REQUEST_PER_BURST 1
#define readDMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define readDMA_DST_BASE (CYDEV_SRAM_BASE)
#define BLOCK_SIZE 64
#define BUFFER_0 0
#define BUFFER_1 1

#include "project.h"

static char buffer0[BLOCK_SIZE - 1], buffer1[BLOCK_SIZE - 1];
static int writeBuffer = 0, bufferIndex = 0;

CY_ISR(txISR){
    if(writeBuffer == BUFFER_0){
        if(bufferIndex < BLOCK_SIZE){
            while((UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL) == UART_TX_STS_FIFO_NOT_FULL){
                UART_PutChar(buffer1[bufferIndex]);
                bufferIndex++;
            }
        }
    }
    if(writeBuffer == BUFFER_1){
        if(bufferIndex < BLOCK_SIZE){
            while((UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL) == UART_TX_STS_FIFO_NOT_FULL){
                UART_PutChar(buffer0[bufferIndex]);
                bufferIndex++;
            }
        }        
    }
}

CY_ISR(readDMAISR){
    if(writeBuffer == BUFFER_0){
        writeBuffer = BUFFER_1;
    }
    else if(writeBuffer == BUFFER_1){
        writeBuffer = BUFFER_0;
    }
    bufferIndex = 0;
}

int main(void)
{  
    CyGlobalIntEnable;
    UART_Start();
    ADC_Start();
    readDMAInterrupt_StartEx(readDMAISR);
    txInterrupt_StartEx(txISR);

    /* Variable declarations for readDMA */
    uint8 readDMA_Chan;
    uint8 readDMA_TD[2];

    /* DMA Configuration for readDMA */
    readDMA_Chan = readDMA_DmaInitialize(readDMA_BYTES_PER_BURST, readDMA_REQUEST_PER_BURST, HI16(readDMA_SRC_BASE), HI16(readDMA_DST_BASE));
    readDMA_TD[0] = CyDmaTdAllocate();
    readDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(readDMA_TD[0], BLOCK_SIZE, readDMA_TD[1], readDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(readDMA_TD[1], BLOCK_SIZE, readDMA_TD[0], readDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(readDMA_TD[0], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)&buffer0));
    CyDmaTdSetAddress(readDMA_TD[1], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)&buffer1));
    CyDmaChSetInitialTd(readDMA_Chan, readDMA_TD[0]);
    CyDmaChEnable(readDMA_Chan, 1);
   
    for(;;)
    {
        
    }
}