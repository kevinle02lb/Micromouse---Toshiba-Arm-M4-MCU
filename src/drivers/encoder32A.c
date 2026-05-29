/**
 *************************************************************************
 * @file        encoder32A.c
 * @brief       A-ENC32 driver implementation for TMPM4Ky microcontrollers.
 * @version     V1.0.0
 * @date        25-05-2026
 * 
 * 
 * @details
 *  This driver provides initialization and control functions for the A-ENC32 hardware encoders
 *  https://toshiba.semicon-storage.com/info/RM-A-ENC32-A_en_20250221.pdf?did=160602
 *  https://toshiba.semicon-storage.com/info/COM_A_ENC32-ANE_application_note_en_20231016.pdf?did=156382&prodName=TMPM4KNF10AFG
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

 /* Notes:
  *  - PN0 = ENC0A, PN1 = ENC0B (Left Motor encoders)
  *  - PD3 = ENC2A, PD4 = ENC2B (Right Motor encoders)
  * 
  * 
  * When setting <ENRUN> = 1, do not change other bits at the same time. Operation settings other than
  * <ENRUN> must be set before setting <ENRUN> = 1.
  */

/* Inclusions */
#include "encoder32A.h"



void ENC32A_Init(void)
{
    /* Initialization code for A-ENC32 hardware encoders */
    ENC0_Init();   // Initialize A-ENC32 channel 0 (Left Motor)
    ENC2_Init();   // Initialize A-ENC32 channel 2 (Right Motor)
}

void ENC0_Init(void)
{
    /* Initialization code for A-ENC32 channel 0 */

    /* [1.] Enable Fsysm clock */
    // Its in CGFSYSMENB, Supply and Stop Register B for fsysm
    TSB_CG->FSYSMENB |= ENC0_CG_FSYSMENB_IPMENB06;            // Enable clock for A-ENC32 ch0 - IPMENB06


    /* [2.] Operation Mode */
    TSB_EN0->TNCR &= ~TNCR_ENRUN_MASK;      // Clear to ensure edit operation settings
    /* 
     * Clear to select Encoder mode.        000: Encoder mode
     * Clear to disable Z input.            0: ENCxZ input disable 
     * Clear to disable Phase 3 input.      0: 2-phase decode
     * 
     * Table 3.1 Operation Modes : Set up Config
     */ 
    TSB_EN0->TNCR &= ~(TNCR_MODE_MASK | TNCR_P3EN_MASK | TNCR_ZEN_MASK);     
    TSB_EN0->TNCR |= TNCR_ENCLR_MASK;                                       // Clear counter

    /* [3.] Input Circuit 3.3.1 Config */
    // Sample Clock Config
    TSB_EN0->CLKCR = CLKCR_SPLCKS_8DIV;             // (Fsys / 8 ) = (80Mhz/8) = 10 Mhz
    // Sampling Mode
    TSB_EN0->INPCR &= ~(INPCR_SYNCSPLEN_MASK | INPCR_SYNCSPLMD_MASK | INPCR_SYNCNCZEN_MASK);   // Clear for no PWM synchronous sampling start - Uses PMD circuit
    // Noise cancellation time - Noise cancellation time: <NCT[6:0]> × Sample clock cycle
    TSB_EN0->INPCR = (TSB_EN0->INPCR & ~INPCR_NCT_MASK) | INPCR_NCT_400ns;                     // 4 x (1/10Mhz) = 400ns 

    /* [4.] Decoder Circuit 3.3.2. */
    // Rotation Edge Detection and Direction Signal Generation - Enabled: Refer to 2. Operation Mode - Select Encoder mode
    // Z Judgment Circuit - Disabled
    // Skip Judgment and Abnormal Input Judgment - Disabled
    // Edge Detection Error Judgment - Disabled
    // Buffer Update Control - Disabled
    // BEMF Detection Control - Disabled
    TSB_EN0->TNCR &= ~TNCR_DECMD_MASK;         // Clear for CW or CCW rotation edge detection


    /* [5.] Counter Circuit 3.3.3*/
    TSB_EN0->RELOAD = RELOAD_MAX;               // Set max val. 

    /* [6.] Interrupts */
    TSB_EN0->INT = 0;                           // No count-match interrupt needed (No IE)
    TSB_EN0->INTCR &= ~INTCR_CLEAR_ALL;         // Clear [5:0]. No Interrupts

    ENC0_Start();
}

void ENC2_Init(void)
{
    /* Initialization code for A-ENC32 channel 2 */

    /* [1.] Enable Fsysm clock*/
    TSB_CG->FSYSMENB |= ENC2_CG_FSYSMENB_IPMENB08;            // Enable clock for A-ENC32 ch2 - IPMENB08

    TSB_EN2->TNCR &= ~TNCR_ENRUN_MASK;          // Clear to ensure edit operation settings
    /* [3.] Operation Mode */
    TSB_EN2->TNCR &= ~(TNCR_MODE_MASK | TNCR_P3EN_MASK | TNCR_ZEN_MASK);     
    TSB_EN2->TNCR |= TNCR_ENCLR_MASK;                                       // Clear counter

    /* [3.] Input Ckt. */
    TSB_EN2->CLKCR = CLKCR_SPLCKS_8DIV;             // (Fsys / 8 ) = (80Mhz/8) = 10 Mhz
    TSB_EN2->INPCR &= ~(INPCR_SYNCSPLEN_MASK | INPCR_SYNCSPLMD_MASK | INPCR_SYNCNCZEN_MASK);   // Clear for no PWM synchronous sampling start - Uses PMD circuit
    TSB_EN2->INPCR = (TSB_EN2->INPCR & ~INPCR_NCT_MASK) | INPCR_NCT_400ns;                     // 4 x (1/10Mhz) = 400ns 

    /* [4.] Decode Ckt. */
    TSB_EN2->TNCR &= ~TNCR_DECMD_MASK;          // Clear for CW or CCW rotation edge detection

    /* [5.] Counter Ckt. */
    TSB_EN2->RELOAD = RELOAD_MAX;               // Set max val.

    /* [6.] Interrupts */
    TSB_EN2->INT = 0;                           // No count-match interrupt needed (No IE)
    TSB_EN2->INTCR &= ~INTCR_CLEAR_ALL;         // Clear [5:0]. No Interrupts

    ENC2_Start();
}


void ENC0_Start(void)
{
    TSB_EN0->TNCR |= TNCR_ENRUN_MASK;
}
void ENC2_Start(void)
{
    TSB_EN2->TNCR |= TNCR_ENRUN_MASK;
}

void ENC0_Stop(void)
{
    TSB_EN0->TNCR &= ~TNCR_ENRUN_MASK;
}
void ENC2_Stop(void)
{
    TSB_EN2->TNCR &= ~TNCR_ENRUN_MASK;
}

void ENC0_ClearCNT(void)
{
    ENC0_Stop();
    TSB_EN0->TNCR |= TNCR_ENCLR_MASK;
    ENC0_Start();
}
void ENC2_ClearCNT(void)
{
    ENC2_Stop();
    TSB_EN2->TNCR |= TNCR_ENCLR_MASK;
    ENC0_Start();
}

bool ENC0_GetStatus(void)
{
    /*
     * 0: CCW direction (Counter-clockwise)
     * 1: CW direction (Clockwise)
     * When a motor rotates in CW direction, this bit is set to "1", and, in
     * CCW direction, cleared to "0".
     * When [ENxTNCR]<ENRUN> = 0, <UD> is always cleared to "0"
     */
    bool UD_Status = (TSB_EN0->STS & STS_UD_MASK) != 0;
    return UD_Status;
}
bool ENC2_GetStatus(void)
{
    bool UD_Status = (TSB_EN2->STS & STS_UD_MASK) != 0;
    return UD_Status;
}


int32_t ENC0_ReadCount(void)
{
    return (int32_t)TSB_EN0->CNT;
}
int32_t ENC2_ReadCount(void)
{
    return (int32_t)TSB_EN2->CNT;
}