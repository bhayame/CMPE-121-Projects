/*
Brandon Hayame
bhayame@ucsc.edu
This program reads the value of a potentiometer into an ADC every millisecond and transmits the byte
through a UART to a Raspberry Pi to control the brightness of an LED
*/

#include "project.h"

CY_ISR(txISR){
    while((UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL) == UART_TX_STS_FIFO_NOT_FULL){
        UART_PutChar(ADC_DelSig_Read16());
    }
}

int main(void)
{
    CyGlobalIntEnable;
    UART_Start();
    ADC_DelSig_Start();
    txInterrupt_StartEx(txISR);
    
    for(;;)
    { 
    }
}

