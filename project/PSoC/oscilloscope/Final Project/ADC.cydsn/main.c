/*
Brandon Hayame
bhayame@ucsc.edu

This program sends data from two SAR ADCs to a Raspberry Pi via ping-pong buffering to USB bulk transfer
for the purpose of displaying the waveforms on as an oscilloscope display. It also sends the value of two 
potentiometers to a multiplexed Delta Sigma ADC to control phase shift of a waveform.
*/

#include "project.h"

/* Defines for both DMAs */
#define DMA_BYTES_PER_BURST 1
#define DMA_REQUEST_PER_BURST 0
#define DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DST_BASE (CYDEV_SRAM_BASE)

/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)

#define BLOCK_SIZE 64
#define BUFFER_0 0
#define BUFFER_1 1

CY_ISR(Chan_1_DMA_ISR){

}

CY_ISR(Chan_2_DMA_ISR){

}

static unsigned char ADC_Chan_1_Array_0[BLOCK_SIZE], ADC_Chan_1_Array_1[BLOCK_SIZE],
                     ADC_Chan_2_Array_0[BLOCK_SIZE], ADC_Chan_2_Array_1[BLOCK_SIZE];

int main(void)
{   
    CyGlobalIntEnable;
    
    /* START ADCS AND CONVERSION */
    ADC_Chan_1_Start();
    ADC_Chan_2_Start();
    ADC_Chan_1_StartConvert();
    ADC_Chan_2_StartConvert();
    
    /* START INTERRUPTS */
    Chan_1_DMA_Interrupt_StartEx(Chan_1_DMA_ISR);
    Chan_2_DMA_Interrupt_StartEx(Chan_2_DMA_ISR);
    
    /* START USBFS */
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    while (!USBFS_GetConfiguration()){};
    USBFS_EnableOutEP(OUT_EP_NUM);

    /* Variable declarations for Chan_1_DMA */
    uint8 Chan_1_DMA_Chan;
    uint8 Chan_1_DMA_TD[2];
    
    /* Variable declarations for Chan_2_DMA */
    uint8 Chan_2_DMA_Chan;
    uint8 Chan_2_DMA_TD[2];

    /* DMA Configuration for Chan_1_DMA */
    Chan_1_DMA_Chan = Chan_1_DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    Chan_1_DMA_TD[0] = CyDmaTdAllocate();
    Chan_1_DMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(Chan_1_DMA_TD[0], BLOCK_SIZE, Chan_1_DMA_TD[1], Chan_1_DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(Chan_1_DMA_TD[1], BLOCK_SIZE, Chan_1_DMA_TD[0], Chan_1_DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetAddress(Chan_1_DMA_TD[0], LO16((uint32)ADC_Chan_1_SAR_WRK0_PTR), LO16((uint32)ADC_Chan_1_Array_0));
    CyDmaTdSetAddress(Chan_1_DMA_TD[1], LO16((uint32)ADC_Chan_1_SAR_WRK0_PTR), LO16((uint32)ADC_Chan_1_Array_1));
    CyDmaChSetInitialTd(Chan_1_DMA_Chan, Chan_1_DMA_TD[0]);
    CyDmaChEnable(Chan_1_DMA_Chan, 1);

    /* DMA Configuration for Chan_2_DMA */
    Chan_2_DMA_Chan = Chan_2_DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    Chan_2_DMA_TD[0] = CyDmaTdAllocate();
    Chan_2_DMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(Chan_2_DMA_TD[0], BLOCK_SIZE, Chan_2_DMA_TD[1], Chan_2_DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(Chan_2_DMA_TD[1], BLOCK_SIZE, Chan_2_DMA_TD[0], Chan_2_DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetAddress(Chan_2_DMA_TD[0], LO16((uint32)ADC_Chan_2_SAR_WRK0_PTR), LO16((uint32)ADC_Chan_2_Array_0));
    CyDmaTdSetAddress(Chan_2_DMA_TD[1], LO16((uint32)ADC_Chan_2_SAR_WRK0_PTR), LO16((uint32)ADC_Chan_2_Array_1));
    CyDmaChSetInitialTd(Chan_2_DMA_Chan, Chan_2_DMA_TD[0]);
    CyDmaChEnable(Chan_2_DMA_Chan, 1);
    
    for(;;)
    {

    }
}

