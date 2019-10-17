/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        Pin_1_Write(0);
        Pin_2_Write(0);
        Pin_3_Write(0);
    
        CyDelay(1000);
    
        Pin_1_Write(1);
        Pin_2_Write(1);
        Pin_3_Write(1);
    
        CyDelay(1000);    
    }
}

/* [] END OF FILE */