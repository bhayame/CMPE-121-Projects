#include "project.h"
#define BLOCK_SIZE 4096

static uint8 transmitArray[BLOCK_SIZE - 1], receiveArray[BLOCK_SIZE - 1] = {0};
static uint16 transmitCount = 0, receiveCount = 0, txISRcount = 0, rxISRcount = 0;
static int finishedFlag = 0;

CY_ISR(txISR){
    while((UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL) == UART_TX_STS_FIFO_NOT_FULL){
        UART_PutChar(transmitArray[transmitCount]);
        transmitCount++;
    }
    txISRcount++;
}

CY_ISR(rxISR){
    while((UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY) == UART_RX_STS_FIFO_NOTEMPTY){
        receiveArray[receiveCount] = UART_GetChar();
        receiveCount++;
    }
    if(receiveCount == BLOCK_SIZE){
        finishedFlag = 1;
    }
    rxISRcount++;
}

int main(void)
{
    
    int i, j, mismatchCount = 0;
    for (i = 0; i<BLOCK_SIZE; i++){
        transmitArray[i] = i%256;
        }
    
    CyGlobalIntEnable;
    UART_Start();
    txInterrupt_StartEx(txISR);
    rxInterrupt_StartEx(rxISR);
    LCD_Char_Start();
    
    for(;;)
    {
    if(finishedFlag == 1){
        for(j=0; j<BLOCK_SIZE; j++){
			if(receiveArray[j] != transmitArray[j]){
			    mismatchCount++;
			    }
            }
        LCD_Char_PrintString("MISMATCHES:");
        LCD_Char_PrintNumber(mismatchCount);
        LCD_Char_Position(1,0);
        LCD_Char_PrintString("Tx:");
        LCD_Char_PrintNumber(txISRcount);
        LCD_Char_PrintString(" Rx:");
        LCD_Char_PrintNumber(rxISRcount);        
        finishedFlag = 0;
        }
    }
}