#include "project.h"

/* Defines for DMA */
#define DMA_BYTES_PER_BURST 1
#define DMA_REQUEST_PER_BURST 0
#define DMA_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_DST_BASE (CYDEV_SRAM_BASE)

volatile uint8 interruptFlag;
uint8 DMA_Chan;
uint8 DMA_TD[5];

CY_ISR(dmaISR){
interruptFlag = 1;
}

int main(void)
{
    CyGlobalIntEnable;
    LCD_Char_Start();
    dmaISR_StartEx(dmaISR);

    int sourceArray[16384]; 
    int destinationArray[16384] = {0};
    int i;
    
    for(i=0; i < 16385; i++){
        sourceArray[i] = i%256;
    }


DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(sourceArray), HI16(destinationArray));
DMA_TD[0] = CyDmaTdAllocate();
DMA_TD[1] = CyDmaTdAllocate();
DMA_TD[2] = CyDmaTdAllocate();
DMA_TD[3] = CyDmaTdAllocate();
DMA_TD[4] = CyDmaTdAllocate();
CyDmaTdSetConfiguration(DMA_TD[0], 4092, DMA_TD[1], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_TD[1], 4092, DMA_TD[2], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_TD[2], 4092, DMA_TD[3], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_TD[3], 4092, DMA_TD[4], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
CyDmaTdSetConfiguration(DMA_TD[4], 16, CY_DMA_DISABLE_TD, CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR);
CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)sourceArray), LO16((uint32)destinationArray));
CyDmaTdSetAddress(DMA_TD[1], LO16((uint32)sourceArray+4092), LO16((uint32)destinationArray+4092));
CyDmaTdSetAddress(DMA_TD[2], LO16((uint32)sourceArray+8184), LO16((uint32)destinationArray+8184));
CyDmaTdSetAddress(DMA_TD[3], LO16((uint32)sourceArray+12276), LO16((uint32)destinationArray+12276));
CyDmaTdSetAddress(DMA_TD[4], LO16((uint32)sourceArray+16368), LO16((uint32)destinationArray+16368));
CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
CyDmaChEnable(DMA_Chan, 0);    

    for(;;)
    {
        if(interruptFlag == 1){
        int j;
        uint16 mismatchCount = 0;
        for(j=0; j<64; j++){
            if(sourceArray[4*j] - destinationArray[4*j] != 3){
	        mismatchCount++;
	            }
            if(sourceArray[1+(4*j)] - destinationArray[1+(4*j)] != 1){
	        mismatchCount++;
	            }
            if(sourceArray[2+(4*j)] - destinationArray[2+(4*j)] != -1){
	        mismatchCount++;
	            }
            if(sourceArray[3+(4*j)] - destinationArray[3+(4*j)] != -3){
	        mismatchCount++;
	            }
            }
      LCD_Char_PrintString("MISMATCHES:");
      LCD_Char_PrintNumber(mismatchCount);  
        }
    }
}