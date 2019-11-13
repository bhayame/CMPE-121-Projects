/*
Brandon Hayame
bhayame@ucsc.edu

This program reads a potentiometer value into an ADC, and sends the byte via USB connection to a Raspberry
Pi in order to control the brightness of an LED.

*/

#include "project.h"
/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)

#define BLOCK_SIZE 1

int main(void)
{
    CyGlobalIntEnable;
    ADC_Start();
    
    /* Start USBFS operation with 5V operation. */
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);

    /* Wait until device is enumerated by host. */
    while (!USBFS_GetConfiguration()){};

    /* Enable OUT endpoint to receive data from host. */
    USBFS_EnableOutEP(OUT_EP_NUM);

    for(;;)
    {
        /* Check if configuration is changed. */
        if (USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(OUT_EP_NUM);
            }
        }
        
        while (USBFS_GetEPState(0x1) != USBFS_IN_BUFFER_EMPTY){};
        /*Load ADC value into IN and send to host*/
        uint8 adcValue = ADC_Read16();
        USBFS_LoadInEP(IN_EP_NUM, &adcValue, BLOCK_SIZE);
    }
}