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