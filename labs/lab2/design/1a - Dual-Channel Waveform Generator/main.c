#include "project.h"
#include "stdlib.h"

CYCODE const uint8 sineTable[128] =        /*Taken from AN52705, Example 3*/
{
	128, 134, 140, 147, 153, 159, 165, 171,
	177, 183, 188, 194, 199, 204, 209, 214,
	218, 223, 227, 231, 234, 238, 241, 244,
	246, 248, 250, 252, 253, 254, 255, 255,
	255, 255, 255, 254, 253, 252, 250, 248,
	246, 244, 241, 238, 234, 231, 227, 223,
	218, 214, 209, 204, 199, 194, 188, 183, 
	177, 171, 165, 159, 153, 147, 140, 134, 
	128, 122, 115, 109, 103,  97,  91,  85,
	 79,  73,  68,  62,  57,  52,  47,  42,
	 37,  33,  29,  25,  22,  18,  15,  12,
	 10,   7,   6,   4,   2,   1,   1,   0,
	  0,   0,   1,   1,   2,   4,   6,   7,
	 10,  12,  15,  18,  22,  25,  29,  33,
	 37,  42,  47,  52,  57,  62,  68,  73,
	 79,  85,  91,  97, 103, 109, 115, 122
};

CYCODE const uint8 sineTable1[256] =  
{
	128, 134, 140, 147, 153, 159, 165, 171,
	177, 183, 188, 194, 199, 204, 209, 214,
	218, 223, 227, 231, 234, 238, 241, 244,
	246, 248, 250, 252, 253, 254, 255, 255,
	255, 255, 255, 254, 253, 252, 250, 248,
	246, 244, 241, 238, 234, 231, 227, 223,
	218, 214, 209, 204, 199, 194, 188, 183, 
	177, 171, 165, 159, 153, 147, 140, 134, 
	128, 122, 115, 109, 103,  97,  91,  85,
	 79,  73,  68,  62,  57,  52,  47,  42,
	 37,  33,  29,  25,  22,  18,  15,  12,
	 10,   7,   6,   4,   2,   1,   1,   0,
	  0,   0,   1,   1,   2,   4,   6,   7,
	 10,  12,  15,  18,  22,  25,  29,  33,
	 37,  42,  47,  52,  57,  62,  68,  73,
	 79,  85,  91,  97, 103, 109, 115, 122,
    128, 134, 140, 147, 153, 159, 165, 171,
	177, 183, 188, 194, 199, 204, 209, 214,
	218, 223, 227, 231, 234, 238, 241, 244,
	246, 248, 250, 252, 253, 254, 255, 255,
	255, 255, 255, 254, 253, 252, 250, 248,
	246, 244, 241, 238, 234, 231, 227, 223,
	218, 214, 209, 204, 199, 194, 188, 183, 
	177, 171, 165, 159, 153, 147, 140, 134, 
	128, 122, 115, 109, 103,  97,  91,  85,
	 79,  73,  68,  62,  57,  52,  47,  42,
	 37,  33,  29,  25,  22,  18,  15,  12,
	 10,   7,   6,   4,   2,   1,   1,   0,
	  0,   0,   1,   1,   2,   4,   6,   7,
	 10,  12,  15,  18,  22,  25,  29,  33,
	 37,  42,  47,  52,  57,  62,  68,  73,
	 79,  85,  91,  97, 103, 109, 115, 122
};

     
uint8 DMA_Chan;
uint8 DMA_1_Chan;
uint8 DMA_TD[1];
uint8 DMA_1_TD[1];
int main(void)
{
    
    /* Defines for DMA */
    #define DMA_BYTES_PER_BURST 1
    #define DMA_REQUEST_PER_BURST 1
    #define DMA_SRC_BASE (&sineTable[0])
    #define DMA_DST_BASE (CYDEV_PERIPH_BASE)
	#define DMA_1_BYTES_PER_BURST 1
	#define DMA_1_REQUEST_PER_BURST 1
	#define DMA_1_SRC_BASE (&sineTable1[0])
	#define DMA_1_DST_BASE (CYDEV_PERIPH_BASE)

    uint16 sineFreq = 1872; /*For 100 Hz - 10kHz, use a value between 18 to 1872*/
 
    CyGlobalIntEnable;
    Timer_Start();
    Timer_WritePeriod(sineFreq);
    VDAC8_Start();
    VDAC8_1_Start();
    ADC_DelSig_Start();
    LCD_Char_Start();
    int32 potValue;

    DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE) );
    DMA_TD[0] = CyDmaTdAllocate();  								 
	CyDmaTdSetConfiguration(DMA_TD[0], 128, DMA_TD[0], TD_INC_SRC_ADR); 	 
    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)sineTable), LO16((uint32)VDAC8_Data_PTR) );
    CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]); 	
    CyDmaChEnable(DMA_Chan, 1); 

	DMA_1_Chan = DMA_1_DmaInitialize(DMA_1_BYTES_PER_BURST, DMA_1_REQUEST_PER_BURST, HI16(DMA_1_SRC_BASE), HI16(DMA_1_DST_BASE));
	DMA_1_TD[0] = CyDmaTdAllocate();
	CyDmaTdSetConfiguration(DMA_1_TD[0], 128, DMA_1_TD[0], TD_INC_SRC_ADR);
	CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)&sineTable1), LO16((uint32)VDAC8_1_Data_PTR));
	CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]);
	CyDmaChEnable(DMA_1_Chan, 1);
    
	for(;;)
    {
        LCD_Char_ClearDisplay();
        potValue = ADC_DelSig_Read32();
        LCD_Char_PrintString("PHASE SHIFT:");
        LCD_Char_PrintNumber(potValue/180);
        potValue = potValue / 512;
        CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)&sineTable1[potValue]), LO16((uint32)VDAC8_1_Data_PTR));
        CyDelay(10);
    }
}
