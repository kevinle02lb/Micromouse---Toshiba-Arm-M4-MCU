/**
 *************************************************************************
 * @file        timer32A.c
 * @brief       Timer32A driver implementation for TMPM4Ky microcontrollers.
 * @version     V1.0.0
 * @date        11-05-2026
 * 
 * 
 * @details
 *  This driver provides initialization and control functions for the Timer32A
 *  https://toshiba.semicon-storage.com/info/RM-T32A-C_en_20241129.pdf?did=160771
 *  https://toshiba.semicon-storage.com/info/COM_T32A_PPG-ANE_application_note_en_20240716.pdf?did=156401&prodName=TMPM4KNF10AFG
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

/* Inclusions */
#include "timer32A.h"

/*
 *  (Left motor):
 *  - PA3 = IN1 (T32A00OUTA), PA4 = IN2 (T32A00OUTB)
 *  (Right motor):
 * 	- PC2 = IN1 (T32A30OUTA), PC3 = IN2 (T32A30OUTB) 
 * 
 *  - PN0 = ENC0A, PN1 = ENC0B (Left Motor encoders)
 *  - PD3 = ENC2A, PD4 = ENC2B (Right Motor encoders)
 */

void T32A_Init(void)
{
    /* Initialization code for Timer32A */
    // TSB_CG->PROTECT = CG_PROTECT_UNLOCK;    // Unlock protected registers - Automatically sets to 0xC1 after Reset

    // Initialize Timer32A channels with user-defined count and mode
    /* Changeable Period in timer32A.h*/
    T32A0_Init(T32A_CH0_PERIOD);  // Initialize Timer32A channel 0
    T32A3_Init(T32A_CH3_PERIOD);  // Initialize Timer32A channel 3

}


void T32A0_Init(uint16_t period)
{
    /* Initialization code for Timer32A channel 0 */
    // Configure Timer32A channel 0 with specified period and mode
    // 1. Enable the clock for Timer32A Channel 0 - Supply and Stop Register A for fsysm
        /* Dont need to config. CGFSYSENA not using CRC & RAMP ch0 */ 
    TSB_CG->FSYSMENA |= (T32A0_CG_FSYSMENA_IPMENA28);          // Enable clock for Timer32A ch0 - IPMENA28

    // Disable Timer before Coniguration
    TSB_T32A0->RUNA &= ~T32A_RUNx_MASK;                    // Stop Timer32A channel 0
    // Wait until Timer32A channel 0 is not running. 0 - Stop, 1 - Operation. R only
    while ((TSB_T32A0->RUNA & T32A_RUNx_RUNFLGx_MASK) != 0) 
    { ; }   


    // 2. Choose Mode
    /* Mode */
    TSB_T32A0->MOD &= ~T32A_MODE_MASK;                // Clear mode bits
    TSB_T32A0->MOD |= (T32A_MODE32 & T32A_MODE_MASK);
    
    // 3. Config Counter Control Register
    /* Prescale Timer A and B in 16bit - PRSCLx */
    TSB_T32A0->CRA &= ~T32A_CRx_PRSCLC_MASK;          // Clear prescaler bits
    TSB_T32A0->CRB &= ~T32A_CRx_PRSCLC_MASK;          // Clear prescaler bits
    TSB_T32A0->CRA |= T32A0_CRA_PRSCLA;               // Set prescaler for Timer A0
    TSB_T32A0->CRB |= T32A0_CRB_PRSCLB;               // Set prescaler for Timer B0

    /* Counting Direction - UPDNx*/
    TSB_T32A0->CRA &= ~T32A_CRx_UPDNx_MASK;           // Clear counting direction bits
    TSB_T32A0->CRA |= (T32A_UPDNx & T32A_CRx_UPDNx_MASK);

    /* Double Buffering - WBFx*/
    TSB_T32A0->CRA |= T32A_CRx_WBFx_MASK;            // Enable double buffering for Timer A0
    TSB_T32A0->CRB |= T32A_CRx_WBFx_MASK;            // Enable double buffering for Timer B0

    /* ============================================
     * 4. PPG waveform values - FOLLOW THE NOTE! Table 3.1
     * Also: 5.3. Programmable Rectangular Wave Output (PPG) Example Setup
     * ============================================
     * Up counting constraint: 
     * 
     * Setting range of [T32AxRGx1]: [T32AxRGx1] ≥ [T32AxRELDx] +2
     * Setting range of [T32AxRGx0]: [T32AxRELDx] ≤ [T32AxRGx0] ≤ [T32AxRGx1]
     * 
     * Period       : [T32AxRGA1] – [T32AxRELDA] 
     * Duty Ratio   : [T32AxRGA0] – [T32AxRELDA] 
     */

    // Config CRA and CRB for RELDA and RELDB behavior. Make it match between the counter and Timer Register A1 (RGx1) for up counting mode.
    TSB_T32A0->CRA |= T32A_CRx_RELDx_MASK;          // Set RELDA and RELDB to reload value when counter reaches RGx1
    TSB_T32A0->CRB |= T32A_CRx_RELDx_MASK;

    // Set Count/Rolaod value
    TSB_T32A0->RELDA = 0;           // Reload val of 0. Counter will reload to 0 when it reaches RGx1.
    TSB_T32A0->RELDB = 0;

    /* Set Compare values for PPG output
     *
     * Compare value for Timer Register 0 (RGA0/RGB0). (To be Changed over time for PWM duty cycle adjustment). 
     * This sets the duty cycle of the PWM signal. Set to 0 for 0% duty cycle (always low). 
     * 
     * Duty [%] = (RGA1 - RGA0) / (RGA1 - RELDA) * 100
     * or
     * Duty period = Period - (Duty [%] * Period / 100)
     * RGx0 = RGx1 - (Duty [%] * RGx1 / 100)
     * RGx0 = Period * Duty Percentage
     */
    TSB_T32A0->RGA0 = 0;             
    TSB_T32A0->RGB0 = 0;             

    TSB_T32A0->RGA1 = period;        // Compare value for Timer Register 1 . This sets the period of the PWM signal.
    TSB_T32A0->RGB1 = period;        

    // Output Control Register
    TSB_T32A0->OUTCRA0 &= ~T32A_OUTCRx0_ORCx_MASK;   // Clear for No change 
    TSB_T32A0->OUTCRB0 &= ~T32A_OUTCRx0_ORCx_MASK;   // Clear for No change
    
    /* Set output high on compare match with RG0, Clear output on compare match with RG1 */
    TSB_T32A0->OUTCRA1 = (TSB_T32A0->OUTCRA1 & ~T32A_OUTCRx1_MASK) | 
                         (T32A_OUTCRx1_OCRCMPx0_SET | T32A_OUTCRx1_OCRCMPx1_CLEAR);  // Timer A

    TSB_T32A0->OUTCRB1 = (TSB_T32A0->OUTCRB1 & ~T32A_OUTCRx1_MASK) | 
                         (T32A_OUTCRx1_OCRCMPx0_SET | T32A_OUTCRx1_OCRCMPx1_CLEAR);  // Timer B

}


void T32A3_Init(uint16_t period)
{
    /* Initialization code for Timer32A channel 3 */
    // Configure Timer32A channel 3 with specified period and mode
    

    // 1. Enable the clock for Timer32A Channel 3 - Supply and Stop Register A for fsysm
    /* Dont need to config. CGFSYSENA not using CRC & RAMP ch0 */ 
    TSB_CG->FSYSMENA |= (T32A3_CG_FSYSMENA_IPMENA31);            // Enable clock for Timer32A ch3 - IPMENA31

    // Disable Timer before Coniguration
    TSB_T32A3->RUNA &= ~T32A_RUNx_MASK;                      // Stop Timer32A channel 3
    // Wait until Timer32A channel 3 is not running. 0 - Stop, 1 - Operation. R only
    while ((TSB_T32A3->RUNA & T32A_RUNx_RUNFLGx_MASK) != 0) 
    { ; }   


    // 2. Choose Mode
    /* Mode */
    TSB_T32A3->MOD &= ~T32A_MODE_MASK;              // Clear mode bits
    TSB_T32A3->MOD |= (T32A_MODE32 & T32A_MODE_MASK);

    // 3. Config Counter Control Register
    /* Prescale Timer A and B in 16bit - PRSCLx */
    TSB_T32A3->CRA &= ~T32A_CRx_PRSCLC_MASK;          // Clear prescaler bits
    TSB_T32A3->CRB &= ~T32A_CRx_PRSCLC_MASK;          // Clear prescaler bits
    TSB_T32A3->CRA |= T32A3_CRA_PRSCLA;               // Set prescaler for Timer A3
    TSB_T32A3->CRB |= T32A3_CRB_PRSCLB;               // Set prescaler for Timer B3

     /* Counting Direction - UPDNx*/
    TSB_T32A3->CRA &= ~T32A_CRx_UPDNx_MASK;           // Clear counting direction bits
    TSB_T32A3->CRA |= (T32A_UPDNx & T32A_CRx_UPDNx_MASK);

    /* Double Buffering - WBFx*/
    TSB_T32A3->CRA |= T32A_CRx_WBFx_MASK;             // Enable double buffering for Timer A3
    TSB_T32A3->CRB |= T32A_CRx_WBFx_MASK;             // Enable double buffering for Timer B3


    /* ============================================
    * 4. PPG waveform values - FOLLOW THE NOTE! Table 3.1
    * Also: 5.3. Programmable Rectangular Wave Output (PPG) Example Setup
    * ============================================
    * Up counting constraint: 
    * 
    * Setting range of [T32AxRGx1]: [T32AxRGx1] ≥ [T32AxRELDx] +2
    * Setting range of [T32AxRGx0]: [T32AxRELDx] ≤ [T32AxRGx0] ≤ [T32AxRGx1]
    * 
    * Period       : [T32AxRGA1] – [T32AxRELDA] 
    * Duty Ratio   : [T32AxRGA0] – [T32AxRELDA] 
    */
    // Config CRA and CRB for RELDA and RELDB behavior. Make it match between the counter and Timer Register A1 (RGx1) for up counting mode.
    TSB_T32A3->CRA |= T32A_CRx_RELDx_MASK;          // Set RELDA and RELDB to reload value when counter reaches RGx1
    TSB_T32A3->CRB |= T32A_CRx_RELDx_MASK;          

    // Set Count/Rolaod value
    TSB_T32A3->RELDA = 0;           // Reload val of 0. Counter will reload to 0 when it reaches RGx1.
    TSB_T32A3->RELDB = 0;

    TSB_T32A3->RGA0 = 0;             
    TSB_T32A3->RGB0 = 0;       

    TSB_T32A3->RGA1 = period;        // Compare value for Timer Register 1 . This sets the period of the PWM signal.
    TSB_T32A3->RGB1 = period;        
    
    // Output Control Register
    TSB_T32A3->OUTCRA0 &= ~T32A_OUTCRx0_ORCx_MASK;   // Clear for No change 
    TSB_T32A3->OUTCRB0 &= ~T32A_OUTCRx0_ORCx_MASK;   // Clear for No change
    
    /* Set output high on compare match with RG0, Clear output on compare match with RG1 */
    TSB_T32A3->OUTCRA1 = (TSB_T32A3->OUTCRA1 & ~T32A_OUTCRx1_MASK) | 
                         (T32A_OUTCRx1_OCRCMPx0_SET | T32A_OUTCRx1_OCRCMPx1_CLEAR);  // Timer A

    TSB_T32A3->OUTCRB1 = (TSB_T32A3->OUTCRB1 & ~T32A_OUTCRx1_MASK) | 
                         (T32A_OUTCRx1_OCRCMPx0_SET | T32A_OUTCRx1_OCRCMPx1_CLEAR);  // Timer B
}




/*__________________________________________________________________________________
 *
 * Other functions for Timer32A control and operation
 *__________________________________________________________________________________
*/

void T32A0_Start(void)
{
    /* Start Timer32A channel 0 */
    TSB_T32A0->RUNA |= T32A_RUNx_MASK;             // Start Timer32A channel 0
}
void T32A3_Start(void)
{
    /* Start Timer32A channel 3 */
    TSB_T32A3->RUNA |= T32A_RUNx_MASK;             // Start Timer32A channel 3
}
void T32A0_Stop(void)
{
    /* Stop Timer32A channel 0 */
    TSB_T32A0->RUNA &= ~T32A_RUNx_MASK;            // Stop Timer32A channel 0
}
void T32A3_Stop(void)
{
    /* Stop Timer32A channel 3 */
    TSB_T32A3->RUNA &= ~T32A_RUNx_MASK;            // Stop Timer32A channel 3
}



void T32A0_SetTimerA0(uint16_t val)
{
    TSB_T32A0->RGA0 = val;        
}
void T32A0_SetTimerA1(uint16_t val)
{
    TSB_T32A0->RGA1 = val;        
}
void T32A0_SetTimerB0(uint16_t val)
{
    TSB_T32A0->RGB0 = val;
}
void T32A0_SetTimerB1(uint16_t val)
{
    TSB_T32A0->RGB1 = val;
}
void T32A3_SetTimerA0(uint16_t val)
{
    TSB_T32A3->RGA0 = val;
}
void T32A3_SetTimerA1(uint16_t val)
{
    TSB_T32A3->RGA1 = val;
}
void T32A3_SetTimerB0(uint16_t val)
{
    TSB_T32A3->RGB0 = val;
}
void T32A3_SetTimerB1(uint16_t val)
{
    TSB_T32A3->RGB1 = val;
}


void T32A0_SetOutCRA1(uint32_t mode)
{
    /* Set output control for Timer32A channel 0 Timer A */
    TSB_T32A0->OUTCRA1 = (TSB_T32A0->OUTCRA1 & ~T32A_OUTCRx1_MASK) | mode;
}
void T32A0_SetOutCRB1(uint32_t mode)
{
    /* Set output control for Timer32A channel 0 Timer B */
    TSB_T32A0->OUTCRB1 = (TSB_T32A0->OUTCRB1 & ~T32A_OUTCRx1_MASK) | mode;
}

void T32A3_SetOutCRA1(uint32_t mode)
{
    /* Set output control for Timer32A channel 3 Timer A */
    TSB_T32A3->OUTCRA1 = (TSB_T32A3->OUTCRA1 & ~T32A_OUTCRx1_MASK) | mode;
}
void T32A3_SetOutCRB1(uint32_t mode)
{
    /* Set output control for Timer32A channel 3 Timer B */
    TSB_T32A3->OUTCRB1 = (TSB_T32A3->OUTCRB1 & ~T32A_OUTCRx1_MASK) | mode;
}

