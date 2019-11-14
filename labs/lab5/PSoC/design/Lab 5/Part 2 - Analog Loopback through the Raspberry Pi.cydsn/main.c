/*
Brandon Hayame
bhayame@ucsc.edu
This program converts an analog signal to digital, and transmits it across USB to a Rpi,
and waits to receive the same signal back from it.

*/

#include "project.h"

/* Definitions for adcDMA */
#define adcDMA_BYTES_PER_BURST 1
#define adcDMA_REQUEST_PER_BURST 1
#define adcDMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define adcDMA_DST_BASE (CYDEV_SRAM_BASE)


/* Definitions for dacDMA */
#define dacDMA_BYTES_PER_BURST 1
#define dacDMA_REQUEST_PER_BURST 1
#define dacDMA_SRC_BASE (CYDEV_SRAM_BASE)
#define dacDMA_DST_BASE (CYDEV_PERIPH_BASE)

/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)

#define BLOCK_SIZE 64
#define BUFFER_0 0
#define BUFFER_1 1

static int adcBuffer = 0, dacBuffer = 0;
static unsigned char ADCbuffer0[BLOCK_SIZE], ADCbuffer1[BLOCK_SIZE], DACbuffer0[BLOCK_SIZE], DACbuffer1[BLOCK_SIZE];

CY_ISR(adcDMAisr){
    if(adcBuffer == BUFFER_0){
        adcBuffer = BUFFER_1;
    }
    else if(adcBuffer == BUFFER_1){
        adcBuffer = BUFFER_0;
    }
}

CY_ISR(dacDMAisr){
    if(dacBuffer == BUFFER_0){
        dacBuffer = BUFFER_1;
    }
    else if(dacBuffer == BUFFER_1){
        dacBuffer = BUFFER_0;
    }
}

int main(void)
{
    CyGlobalIntEnable;
    ADC_Start();
    ADC_StartConvert();
    VDAC_Start();
    LCD_Char_Start();
    adcDMAInterrupt_StartEx(adcDMAisr);
    dacDMAInterrupt_StartEx(dacDMAisr);

    /* Start USBFS operation with 5V operation. */
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);

    /* Wait until device is enumerated by host. */
    while (!USBFS_GetConfiguration()){};

    /* Enable OUT endpoint to receive data from host. */
    USBFS_EnableOutEP(OUT_EP_NUM);
    
    /* Variable declarations for adcDMA */
    uint8 adcDMA_Chan;
    uint8 adcDMA_TD[2];
    
    /* Variable declarations for dacDMA */
    uint8 dacDMA_Chan;
    uint8 dacDMA_TD[2];

    /* DMA Configuration for adcDMA */
    adcDMA_Chan = adcDMA_DmaInitialize(adcDMA_BYTES_PER_BURST, adcDMA_REQUEST_PER_BURST, HI16(adcDMA_SRC_BASE), HI16(adcDMA_DST_BASE));
    adcDMA_TD[0] = CyDmaTdAllocate();
    adcDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(adcDMA_TD[0], BLOCK_SIZE, adcDMA_TD[1], adcDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(adcDMA_TD[1], BLOCK_SIZE, adcDMA_TD[0], adcDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(adcDMA_TD[0], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)&ADCbuffer0[0]));
    CyDmaTdSetAddress(adcDMA_TD[1], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)&ADCbuffer1[0]));
    CyDmaChSetInitialTd(adcDMA_Chan, adcDMA_TD[0]);
    CyDmaChEnable(adcDMA_Chan, 1);    
    
    /* DMA Configuration for dacDMA */
    dacDMA_Chan = dacDMA_DmaInitialize(dacDMA_BYTES_PER_BURST, dacDMA_REQUEST_PER_BURST, HI16(dacDMA_SRC_BASE), HI16(dacDMA_DST_BASE));
    dacDMA_TD[0] = CyDmaTdAllocate();
    dacDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(dacDMA_TD[0], BLOCK_SIZE, dacDMA_TD[1], dacDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(dacDMA_TD[1], BLOCK_SIZE, dacDMA_TD[0], dacDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetAddress(dacDMA_TD[0], LO16((uint32)&DACbuffer0[0]), LO16((uint32)VDAC_Data_PTR));
    CyDmaTdSetAddress(dacDMA_TD[1], LO16((uint32)&DACbuffer1[0]), LO16((uint32)VDAC_Data_PTR));
    CyDmaChSetInitialTd(dacDMA_Chan, dacDMA_TD[0]);
    CyDmaChEnable(dacDMA_Chan, 1);   

    for(;;)
    {   
        LCD_Char_PrintNumber(ADCbuffer0[0]);
        if (USBFS_IsConfigurationChanged()){
            if (USBFS_GetConfiguration()){
                USBFS_EnableOutEP(OUT_EP_NUM);
            }
        }
        
        /* IN transfer polling */
        if(adcBuffer == BUFFER_0){
            while (USBFS_GetEPState(IN_EP_NUM) != USBFS_IN_BUFFER_EMPTY){}
            USBFS_LoadInEP(IN_EP_NUM, ADCbuffer1, BLOCK_SIZE);
        }
        if(adcBuffer == BUFFER_1){
            while (USBFS_GetEPState(IN_EP_NUM) != USBFS_IN_BUFFER_EMPTY){}
            USBFS_LoadInEP(IN_EP_NUM, ADCbuffer0, BLOCK_SIZE);    
        }
       
        /* OUT transfer polling */
        if(dacBuffer == BUFFER_0){
            if (USBFS_GetEPState(OUT_EP_NUM) == USBFS_OUT_BUFFER_FULL){
                USBFS_ReadOutEP(OUT_EP_NUM, DACbuffer1, BLOCK_SIZE);
                while (USBFS_GetEPState(OUT_EP_NUM) == USBFS_OUT_BUFFER_FULL){}
            }
        }
        if(dacBuffer == BUFFER_1){
            if (USBFS_GetEPState(OUT_EP_NUM) == USBFS_OUT_BUFFER_FULL){
                USBFS_ReadOutEP(OUT_EP_NUM, DACbuffer0, BLOCK_SIZE);
                while (USBFS_GetEPState(OUT_EP_NUM) == USBFS_OUT_BUFFER_FULL){}
            }
        }             
    }
}