/**
 *************************************************************************
 * @file        dma.c
 * @brief       DMAC-B driver implementation for TMPM4Ky RISC microcontrollers.
 * @version     V1.0.0
 * @date        01-06-2026
 * 
 * 
 * @details
 *  This driver provides initialization and control functions for the ADC-I
 *  https://toshiba.semicon-storage.com/info/TXZP-PINFO-M4K(2)_en_20231225.pdf?did=70854
 *  https://toshiba.semicon-storage.com/info/RM-DMAC-B_en_20241031.pdf?did=160537
 *  https://developer.arm.com/documentation/ddi0417/a/Functional-Overview/Functional-operation/DMA-control
 *  
 *  Target: Burst Mode only availiable for DMA for ADC
 *  ARM PrimeCell DMA (PL230) Architecture
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

/* Inclusions */
#include "dma.h"

/* 3.3.2. Preparation of Channel Control Data
 * 1. Size is 64 entries (32 Primary + 32 Alternate) = 1024 bytes total. 
 * 32 (DMA Channel #) * 2U   ->   64 * 16 bytes (from sturct DMA_ChnlCtrlData_TypeDef) = 1024 bytes
 * 2. Alignment is 1024 bytes (2^10) because CTRLBASEPTR ignores bits [9:0]. Ex: Make it a multiple for 0x400 (1024 bytes)
 */
__attribute__((aligned(1024))) static DMA_ChnlCtrlData_TypeDef DMA_CtrlTable[DMA_CHANNEL_COUNT * 2U];

void DMAC_Init(void)
{
    /* [1] Clock Enable (Section 3.3.1) */
    TSB_CG->FSYSMENB |= DMAC_CG_FSYSMENB_IPMENB17;

    /* [2] Preparation of Channel Control Data (Section 3.3.2 / 4.2.3) */
    /* Tell the DMA controller where your 1024-byte aligned array is located */
    TSB_DMAA->CTRLBASEPTR = (uint32_t)DMA_CtrlTable;

    /* [3] Common Initialization of Register (Section 3.3.3) */
    /* The datasheet mandates these three writes for all units first: */
    TSB_DMAA->CFG = DMAxCfg_Master_enable;                  // Enable DMA master operation
    TSB_DMAA->CHNLREQMASKSET = DMAxChnlReqMaskSet_MASK;     // Mask all requests initially (0xFFFFFFFF)
    TSB_DMAA->CHNLENABLESET = DMAxChnlEnableSet_MASK;       // Set for make the corresponding channel valid.

    /* [4] Un-mask specific channels (Section 3.9) */
    /* Clear mask to allow DMA requests from ADC Unit A (ch16) and Unit C (ch18) */
    TSB_DMAA->CHNLREQMASKCLR = (DMA_CHANNEL_MASK(DMA_ADASLG_DMAREQ) | 
                                DMA_CHANNEL_MASK(DMA_ADCSLG_DMAREQ));     

    /* [5] Enable Single Transmission (Section 4.2.6 / 4.2.7) */
    /* Force burst mode for ADC channels  */
    TSB_DMAA->CHNLUSEBURSTSET = (DMA_CHANNEL_MASK(DMA_ADASLG_DMAREQ)   // Ch 16
                           | DMA_CHANNEL_MASK(DMA_ADCSLG_DMAREQ));     // Ch 18 
}
