#include "project.h"
#define BLOCK_SIZE 4096

static uint8 transmitArray[BLOCK_SIZE - 1], receiveArray[BLOCK_SIZE - 1] = {0};
static uint16 transmitCount = 0, receiveCount = 0, errorCount = 0, finishedFlag = 0;

CY_ISR(txISR){
    uint8 txStatus = UART_ReadTxStatus();
    if(transmitCount == BLOCK_SIZE){
            CyDelay(1);
            finishedFlag = 1;
    }
    if(txStatus && UART_TX_STS_FIFO_NOT_FULL == UART_TX_STS_FIFO_NOT_FULL){
        UART_PutChar(transmitArray[transmitCount]);
        transmitCount++;      
    }
}

CY_ISR(timerISR){
    uint8 rxStatus = UART_ReadRxStatus();
    if(rxStatus && UART_RX_STS_OVERRUN == UART_RX_STS_OVERRUN){
        errorCount++;
    }
    while(UART_ReadRxStatus() && UART_RX_STS_FIFO_NOTEMPTY == UART_RX_STS_FIFO_NOTEMPTY){
        receiveArray[receiveCount] = UART_ReadRxData();
        receiveCount++;
    }
}

int main(void)
{
    int i, j, mismatchCount = 0;
    for (i = 0; i<BLOCK_SIZE; i++){
        transmitArray[i] = i%256;
    }
    
    CyGlobalIntEnable;
    Timer_Start();
    LCD_Char_Start();
    txInterrupt_StartEx(txISR);
    timerInterrupt_StartEx(timerISR);
    UART_Start();

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
            LCD_Char_PrintString("ERRORS:");
            LCD_Char_PrintNumber(errorCount);
            finishedFlag = 0;
        }
    }
}