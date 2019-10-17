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
        CyPins_SetPin(Pin_1_0);
        CyPins_SetPin(Pin_2_0);
        CyPins_SetPin(Pin_3_0);
        
        CyDelay(1000);
        
        CyPins_ClearPin(Pin_1_0);
        CyPins_ClearPin(Pin_2_0);
        CyPins_ClearPin(Pin_3_0);
        
        CyDelay(1000);
    }
}

/* [] END OF FILE */
