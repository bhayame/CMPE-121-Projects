#include "project.h"


CY_ISR(timerInterrupt)
    {
    uint16 echoWidth;
    echoWidth = 65535 - Timer_1_ReadCounter();
    
    if(echoWidth <= 5000){
        redEnable_Write(0); 
        CyDelay(5000);
    }
    
    else{        
        redEnable_Write(1);
        }
    
    timerReset_Write(1);
}

int main(void)
{
    CyGlobalIntEnable;
    timerInterrupt_StartEx(timerInterrupt);
    Counter_1_Start();
    Timer_1_Start();
	
    Clock_1_Start();
    Clock_2_Start();
	Clock_3_Start();
    
    blueEnable_Write(1);
    greenEnable_Write(1);
    
    for(;;)
    {
    counterReset_Write(1);
    triggerOut_Write(1);
	timerReset_Write(0);
    CyDelay(500);
    counterReset_Write(0);
    triggerOut_Write(0);
    }
}