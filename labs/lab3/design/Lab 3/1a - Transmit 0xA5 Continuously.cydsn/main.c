/*Brandon Hayame
bhayame@ucsc.edu
CMPE 121 FALL 19

This Program continuously transmits the byte 0xA5 through a UART with its Tx and Rx inputs tied together.
*/

#include "project.h"

int main(void)
{
    CyGlobalIntEnable;
    UART_Start();   
    
    for(;;)
    {
        UART_PutChar(0xA5);
        CyDelay(10);
    }
}