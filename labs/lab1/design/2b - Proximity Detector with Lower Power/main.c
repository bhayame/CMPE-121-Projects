#include "project.h"

 CY_ISR(isr_1)
{
    SleepTimer_1_GetStatus();
}

CY_ISR(timerInterrupt)
    {
    uint16 echoWidth;
    echoWidth = 65535 - Timer_1_ReadCounter();
    
    if(echoWidth <= 5000){
        redEnable_Write(0);    
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
	
    isr_1_StartEx(isr_1);
    SleepTimer_1_Start();
	
    Clock_1_Start();
    Clock_2_Start();
	Clock_3_Start();
    
    blueEnable_Write(1);
    greenEnable_Write(1);
    
    for(;;)
    {
    CyPmSaveClocks();
    /*
    Timer_1_Sleep();
    Counter_1_Sleep();*/
    CyPmSleep(PM_SLEEP_TIME_NONE, PM_SLEEP_SRC_CTW);
    /*
    Timer_1_Wakeup();
    Counter_1_Wakeup();
    */
    CyPmRestoreClocks();
 
    counterReset_Write(1);
    triggerOut_Write(1);
	timerReset_Write(0);
    CyDelay(500);
    counterReset_Write(0);
    triggerOut_Write(0);
	
    }
}


