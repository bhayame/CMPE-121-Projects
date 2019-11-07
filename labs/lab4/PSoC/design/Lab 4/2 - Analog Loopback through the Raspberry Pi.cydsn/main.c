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

#define writeDMA_BYTES_PER_BURST 1
#define writeDMA_REQUEST_PER_BURST 1
#define writeDMA_SRC_BASE (CYDEV_SRAM_BASE)
#define writeDMA_DST_BASE (CYDEV_PERIPH_BASE)

#define BLOCK_SIZE 64
#define BUFFER_0 0
#define BUFFER_1 1

#include "project.h"

static char buffer0[BLOCK_SIZE], buffer1[BLOCK_SIZE], dacbuffer0[BLOCK_SIZE], dacbuffer1[BLOCK_SIZE];
static int writeBuffer = 0, bufferIndex = 0, readBuffer = 0, readbufferIndex;

CY_ISR(txISR){
    if(writeBuffer == BUFFER_0){
        if(bufferIndex < BLOCK_SIZE){
            //while(UART_ReadTxStatus() & UART_TX_STS_FIFO_EMPTY){
                UART_PutChar(buffer1[bufferIndex]);
                bufferIndex++;
            //}
        }
    }
    if(writeBuffer == BUFFER_1){
        if(bufferIndex < BLOCK_SIZE){
            //while(UART_ReadTxStatus() & UART_TX_STS_FIFO_EMPTY){
                UART_PutChar(buffer0[bufferIndex]);
                bufferIndex++;
            //}
        }        
    }
    if(bufferIndex == BLOCK_SIZE){
        bufferIndex = 0;
    }
}

CY_ISR(rxISR){
    if(readBuffer == BUFFER_0){
        if(readbufferIndex < BLOCK_SIZE){
           // while(UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
                dacbuffer1[readbufferIndex] = UART_GetChar();
                readbufferIndex++;
            //}
        }
    }
    if(readBuffer == BUFFER_1){
        if(readbufferIndex < BLOCK_SIZE){
            //while(UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
                dacbuffer0[readbufferIndex] = UART_GetChar();
                readbufferIndex++;
            //}
        }        
    }
    if(readbufferIndex == BLOCK_SIZE){
        readbufferIndex = 0;
    }
}

CY_ISR(readDMAISR){
    if(writeBuffer == BUFFER_0){
        writeBuffer = BUFFER_1;
    }
    else if(writeBuffer == BUFFER_1){
        writeBuffer = BUFFER_0;
    }
}

CY_ISR(writeDMAISR){
    if(readBuffer == BUFFER_0){
        readBuffer = BUFFER_1;
    }
    else if(readBuffer == BUFFER_1){
        readBuffer = BUFFER_0;
    }
}

int main(void)
{  
    CyGlobalIntEnable;
    UART_Start();
    ADC_Start();
    readDMAInterrupt_StartEx(readDMAISR);
    txInterrupt_StartEx(txISR);
    rxInterrupt_StartEx(rxISR);
    writeDMAInterrupt_StartEx(writeDMAISR);
    LCD_Char_Start();
    VDAC8_Start();
    
    /* Variable declarations for readDMA */
    uint8 readDMA_Chan;
    uint8 readDMA_TD[2];
    
    /* Variable declarations for writeDMA */
    uint8 writeDMA_Chan;
    uint8 writeDMA_TD[2];

    /* DMA Configuration for readDMA */
    readDMA_Chan = readDMA_DmaInitialize(readDMA_BYTES_PER_BURST, readDMA_REQUEST_PER_BURST, HI16(readDMA_SRC_BASE), HI16(readDMA_DST_BASE));
    readDMA_TD[0] = CyDmaTdAllocate();
    readDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(readDMA_TD[0], BLOCK_SIZE, readDMA_TD[1], readDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(readDMA_TD[1], BLOCK_SIZE, readDMA_TD[0], readDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(readDMA_TD[0], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)&buffer0[0]));
    CyDmaTdSetAddress(readDMA_TD[1], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)&buffer1[0]));
    CyDmaChSetInitialTd(readDMA_Chan, readDMA_TD[0]);
    CyDmaChEnable(readDMA_Chan, 1);
   
    /* DMA Configuration for writeDMA */
    writeDMA_Chan = writeDMA_DmaInitialize(writeDMA_BYTES_PER_BURST, writeDMA_REQUEST_PER_BURST, HI16(writeDMA_SRC_BASE), HI16(writeDMA_DST_BASE));
    writeDMA_TD[0] = CyDmaTdAllocate();
    writeDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(writeDMA_TD[0], BLOCK_SIZE, writeDMA_TD[1], writeDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(writeDMA_TD[1], BLOCK_SIZE, writeDMA_TD[0], writeDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetAddress(writeDMA_TD[0], LO16((uint32)&dacbuffer0[0]), LO16((uint32)VDAC8_Data_PTR));
    CyDmaTdSetAddress(writeDMA_TD[1], LO16((uint32)&dacbuffer1[0]), LO16((uint32)VDAC8_Data_PTR));
    CyDmaChSetInitialTd(writeDMA_Chan, writeDMA_TD[0]);
    CyDmaChEnable(writeDMA_Chan, 1);

    for(;;)
    {
        ADC_Read16();
    }
}