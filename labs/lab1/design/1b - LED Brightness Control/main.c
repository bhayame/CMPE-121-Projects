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
    ADC_DelSig_1_Start();
    PWM_1_Start();
    Clock_1_Start();
    
    int32 potValue;

    for(;;)
    {
        potValue = ADC_DelSig_1_Read32();
        
        if(potValue < 0){
            potValue = 0;
        }
                                            /*Ensure that potValue maintains between 0 - 65535*/
        if(potValue > 65535){
            potValue = 65535;
        }
        
        PWM_1_WriteCompare(potValue / 65);  /*Scale a 0 - 65535 value to a 1 - 999 value*/
    }
}

/* [] END OF FILE */
