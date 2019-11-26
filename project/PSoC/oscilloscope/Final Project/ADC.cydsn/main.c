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
#define CHAN_1_EP_NUM     (1u)
#define CHAN_2_EP_NUM    (2u)

/* I2C Buffer defines */
#define RD_BUFFER_SIZE        (2u)
#define WR_BUFFER_SIZE        (1u)

/* General defines */
#define BLOCK_SIZE 64
#define ARRAY_0 0
#define ARRAY_1 1
#define CHANNEL_1 0
#define CHANNEL_2 1

static unsigned char ADC_Chan_1_Array_0[BLOCK_SIZE], ADC_Chan_1_Array_1[BLOCK_SIZE], 
                     ADC_Chan_2_Array_0[BLOCK_SIZE], ADC_Chan_2_Array_1[BLOCK_SIZE];                  
static int ADC_Chan_1_CurrentArray = 0, ADC_Chan_2_CurrentArray = 0;                    

CY_ISR(Chan_1_DMA_ISR){
    if(ADC_Chan_1_CurrentArray == ARRAY_0){
        ADC_Chan_1_CurrentArray = ARRAY_1;    
    }
    else if(ADC_Chan_1_CurrentArray == ARRAY_1){
        ADC_Chan_1_CurrentArray = ARRAY_0;        
    }
}

CY_ISR(Chan_2_DMA_ISR){
    if(ADC_Chan_2_CurrentArray == ARRAY_0){
        ADC_Chan_2_CurrentArray = ARRAY_1;    
    }
    else if(ADC_Chan_2_CurrentArray == ARRAY_1){
        ADC_Chan_2_CurrentArray = ARRAY_0;        
    }
}            
                    
int main(void)
{   
    
    /* Variable declarations for Chan_1_DMA */
    uint8 Chan_1_DMA_Chan;
    uint8 Chan_1_DMA_TD[2];
    
    /* Variable declarations for Chan_2_DMA */
    uint8 Chan_2_DMA_Chan;
    uint8 Chan_2_DMA_TD[2];
    
    /* I2C VARIABLE DECLARATIONS */
    uint8 read_buffer[RD_BUFFER_SIZE] = {0};
    uint8 write_buffer[WR_BUFFER_SIZE] = {0};
    uint8 command_reg = 0, readCount = 0;
    
    CyGlobalIntEnable;
    
    /* START ADCS AND CONVERSION */
    ADC_Chan_1_Start();
    ADC_Chan_1_StartConvert();
    ADC_Chan_2_Start();
    ADC_Chan_2_StartConvert();
    ADC_DelSig_Start();
    AMux_Start();
    
    
    /* START INTERRUPTS */
    Chan_1_DMA_Interrupt_StartEx(Chan_1_DMA_ISR);
    Chan_2_DMA_Interrupt_StartEx(Chan_2_DMA_ISR);
    
    /* START USBFS */
    /*USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    while (!USBFS_GetConfiguration()){};*/
    
    /* START I2C */
    I2C_SlaveInitWriteBuf((uint8 *) write_buffer, WR_BUFFER_SIZE);   
    I2C_SlaveInitReadBuf((uint8 *) read_buffer, RD_BUFFER_SIZE);
    I2C_Start();

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
        /* IN Transfer Polling *//*
        if(ADC_Chan_1_CurrentArray == ARRAY_0){
            while (USBFS_GetEPState(CHAN_1_EP_NUM) != USBFS_IN_BUFFER_EMPTY){}
            USBFS_LoadInEP(CHAN_1_EP_NUM, ADC_Chan_1_Array_1, BLOCK_SIZE);
        }
        if(ADC_Chan_1_CurrentArray == ARRAY_1){
            while (USBFS_GetEPState(CHAN_1_EP_NUM) != USBFS_IN_BUFFER_EMPTY){}
            USBFS_LoadInEP(CHAN_1_EP_NUM, ADC_Chan_1_Array_0, BLOCK_SIZE);
        }
        if(ADC_Chan_2_CurrentArray == ARRAY_0){
            while (USBFS_GetEPState(CHAN_2_EP_NUM) != USBFS_IN_BUFFER_EMPTY){}
            USBFS_LoadInEP(CHAN_2_EP_NUM, ADC_Chan_2_Array_1, BLOCK_SIZE);
        }
        if(ADC_Chan_2_CurrentArray == ARRAY_1){
            while (USBFS_GetEPState(CHAN_2_EP_NUM) != USBFS_IN_BUFFER_EMPTY){}
            USBFS_LoadInEP(CHAN_2_EP_NUM, ADC_Chan_2_Array_0, BLOCK_SIZE);
        }
        */
        /* Poll if RPi has written to buffer */
        if(I2C_SlaveStatus() & I2C_SSTAT_WR_CMPLT){
            /* Read from buffer and write to command register */ 
            command_reg = write_buffer[0];
            
            /* Clear status */
            I2C_SlaveClearWriteStatus();
            I2C_SlaveClearWriteBuf();
            
            /* CODE HERE TO INTERPRET command_reg AND TAKE ACTION */
        }
        
        /* Poll if RPi has read from buffer */
        if(I2C_SlaveStatus() & I2C_SSTAT_RD_CMPLT){
            readCount = I2C_SlaveGetReadBufSize();
            if(readCount == RD_BUFFER_SIZE){
                /* Clear status */
                I2C_SlaveClearReadStatus();
                I2C_SlaveClearReadBuf();
            
                CyGlobalIntDisable;
            
                /* Send both potentiometer ADC values */
                AMux_FastSelect(CHANNEL_1);
                read_buffer[0] = ADC_DelSig_Read16();
                AMux_FastSelect(CHANNEL_2);
                read_buffer[1] = ADC_DelSig_Read16();
            
                CyGlobalIntEnable;
            }
        }
    }
}