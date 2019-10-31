/*Brandon Hayame
bhayame@ucsc.edu
CMPE 121 FALL 19

This program continuously transmits a 4096 byte array from one location in memory to another and 
reports the number of errors and elapsed time using polling in the main loop.
*/

#include "project.h"

#define BLOCK_SIZE 4096 

int main(void)
{
    UART_Start();
    LCD_Char_Start();
    Clock_1_Start();
    
    uint8 transmitArray[BLOCK_SIZE - 1], receiveArray[BLOCK_SIZE - 1] = {0};
	int i, j, finishedFlag = 0, mismatchCount = 0;
    uint16 transmitCount = 0, receiveCount = 0;

    
    for (i = 0; i<BLOCK_SIZE; i++){
        transmitArray[i] = i%256;
        }
    
    for(;;)
    {
    
    if((UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL) == UART_TX_STS_FIFO_NOT_FULL){
        if(transmitCount < BLOCK_SIZE){
        UART_PutChar(transmitArray[transmitCount]);		/*Add byte to TX FIFO*/
        transmitCount++;
            }
        }
    
    if((UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY) == UART_RX_STS_FIFO_NOTEMPTY){
        if(receiveCount < BLOCK_SIZE){
        receiveArray[receiveCount] = UART_GetChar();	/*Read from RX FIFO*/
            if(receiveCount == 0){
                Timer_Start();
                }
        receiveCount++;
            }
        }
		
	if((receiveCount == BLOCK_SIZE) && (transmitCount == BLOCK_SIZE) && (finishedFlag == 0)){
        int elapsedTime = Timer_ReadPeriod() - Timer_ReadCounter();
		for(j=0; j<BLOCK_SIZE; j++){
			if(receiveArray[j] != transmitArray[j]){
			mismatchCount++;
			}
		}
        LCD_Char_PrintString("MISMATCHES:");
        LCD_Char_PrintNumber(mismatchCount);
        LCD_Char_Position(1,0);
        LCD_Char_PrintString("TIME:");
        LCD_Char_PrintNumber(elapsedTime);       /*No conversion needed due to 1MHz clock*/
        LCD_Char_PrintString(" usec");
		finishedFlag = 1;
	    }
    }
}