/**
 *************************************************************************
 * @file        gpio.c
 * @brief       GPIO driver implementation for TMPM4Ky microcontrollers.
 * @version     V1.0.0
 * @date        11-05-2026
 * 
 * 
 * @details
 *  This driver provides initialization and control functions for the GPIO
 *  https://toshiba.semicon-storage.com/info/TXZP-PORT-M4K(2)_en_20250620.pdf?did=70850
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */


/* Inclusions */
#include "gpio.h"

/*  Notes:
 *  (Left motor):
 *  - PA3 = IN1 (T32A00OUTA), PA4 = IN2 (T32A00OUTB)
 * (Right motor):
 * 	- PC2 = IN1 (T32A30OUTA), PC3 = IN2 (T32A30OUTB) 
 * 
 *  - PN0 = ENC0A, PN1 = ENC0B (Left Motor encoders)
 *  - PD3 = ENC2A, PD4 = ENC2B (Right Motor encoders)
 * 
 *  - PL0 = Far Left IR (AINA16),  PL1 = Left IR  (AINA15)
 *  - PJ1 = Right IR (AINC01), PJ0 = Far Right IR (AINC00)
 * 
 *  - PU1 = Far Left IR Emitter, PU0 = Left IR Emitter (Left IR Emitters)
 *  - PG5 = Right IR Emitter, PG4 = Far Right IR Emitter (Right IR Emitters)
*/

void GPIO_Init(void)
{
    /* Initialization for GPIO pins*/

    /* __________________________________________________________________________________
     *
     * Configure Function Register and CR/IE Registers for each GPIO pin
     * ___________________________________________________________________________________
     */
    PORT_A_Init();
    PORT_C_Init();
    PORT_D_Init();
    PORT_G_Init(); 
    PORT_J_Init();
    PORT_L_Init();
    PORT_N_Init();
    PORT_U_Init();
}





/* ___________________________________________________________________________________
 *
 *  Separate initialization functions for each GPIO port (if needed)
 *  - This allows for more granular control and easier maintenance if different ports have different configurations.
 * ___________________________________________________________________________________ 
 */

void PORT_A_Init(void)
{
    /* Initialization for PORTA pins */
    /* T32A Configuration Port A */

    /* [1.] Enable clock for GPIO Port - Supply and Stop Register A for fsysm */
    TSB_CG->FSYSMENA |= CG_PORTA;            // Enable clock for PORTA

    /* Configure Function Register and CR (Output Control) or IE (Input Control) Registers for each GPIO pin */
    // PA3 - T32A00OUTA - Ouput
    // PA4 - T32A00OUTB - Output
    TSB_PA->CR |=  Px3_MASK | Px4_MASK;             // Set PA3 and PA4 as outputs
    

    /* Function Register Configuration */
    // PA3 - T32A00OUTA - PAFR4
    PA_FRn_Clear(Px3_MASK);         // Clear PA3 function bits
    PA_FRn_Clear(Px4_MASK);         // Clear PA4 function bits

    TSB_PA->FR4 |= Px3_MASK | Px4_MASK;         // Set PA3 and PA4 functions to T32A00OUTA and T32A00OUTB
}
void PORT_C_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTC;            // Enable clock for PORTC

    /* T32A Configuration Port C */
    TSB_PC->CR |= (Px2_MASK | Px3_MASK);             // Set PC2 and PC3 as outputs

    PC_FRn_Clear(Px2_MASK);                          // Clear PC2 function bits
    PC_FRn_Clear(Px3_MASK);                          // Clear PC3 function bits

    TSB_PC->FR5 |= Px2_MASK | Px3_MASK;              // Set PC2 and PC3 functions to T32A30OUTA and T32A30OUTB
}



void PORT_D_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTD;            // Enable clock for PORTD

    /* Encoder initialization for PORTD pins */
    TSB_PD->IE |= (Px3_MASK | Px4_MASK);             // Enable input for PD3 and PD4

    PD_FRn_Clear(Px3_MASK);                          // Clear PD3 function bits
    PD_FRn_Clear(Px4_MASK);                          // Clear PD4 function bits

    TSB_PD->FR6 |= Px3_MASK | Px4_MASK;              // Set PD3 and PD4 functions to ENC2 (ch2) inputs

    TSB_PD->PDN &= ~(Px3_MASK | Px4_MASK);            // Disable pull-down for PD3 and PD4
    TSB_PD->PUP |= (Px3_MASK | Px4_MASK);             // Enable pull-up for PD3 and PD4
}
void PORT_N_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTN;            // Enable clock for PORTN

    /* Encoder initialization for PORTN pins */
    TSB_PN->IE |= (Px0_MASK | Px1_MASK);             // Enable input for PN0 and PN1

    PN_FRn_Clear(Px0_MASK);                          // Clear PN0 function bits
    PN_FRn_Clear(Px1_MASK);                          // Clear PN1 function bits

    TSB_PN->FR6 |= Px0_MASK | Px1_MASK;              // Set PN0 and PN1 functions to ENC0 (ch0) inputs

    TSB_PN->PDN &= ~(Px0_MASK | Px1_MASK);            // Disable pull-down for PN0 and PN1
    TSB_PN->PUP |= (Px0_MASK | Px1_MASK);             // Enable pull-up for PN0 and PN1
}




void PORT_G_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTG;            // Enable clock for PORTG

    PG_FRn_Clear(Px4_MASK);                          // Clear PG4 function bits
    PG_FRn_Clear(Px5_MASK);                          // Clear PG5 function bits

    /* IR Emitter Initialization - Regular GPIO Pin Output w/ Data Register */
    TSB_PG->CR |= (Px4_MASK | Px5_MASK);             // Set PG4 and PG5 as outputs
}
void PORT_U_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTU;            // Enable clock for PORTU

    PU_FRn_Clear(Px0_MASK);                          // Clear PU0 function bits
    PU_FRn_Clear(Px1_MASK);                          // Clear PU1 function bits

    /* IR Emitter Initialization - Regular GPIO Pin Output w/ Data Register */
    TSB_PU->CR |= (Px0_MASK | Px1_MASK);             // Set PU0 and PU1 as outputs
}



void PORT_J_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTJ;            // Enable clock for PORTJ

    /* ADC Input Initialization */

    /* 
     *  Follow 4.2.10. PORT J
     *
     *  Note: When using analog input(AINCx), [PJCR] should be output disable "0",[PJIE] should be input disable "0",
     *  [PJPUP] should be pull-up disable"0" and [PJPDN] should be pull-down disable "0".
     */

    //Disable necessary registers for AINC00/AINC01 to function as ADC inputs
    TSB_PJ->CR &= ~(Px0_MASK | Px1_MASK);             // Clear CR 
    TSB_PJ->IE &= ~(Px0_MASK | Px1_MASK);             // Clear IE 
    TSB_PJ->PUP &= ~(Px0_MASK | Px1_MASK);            // Clear PUP 
    TSB_PJ->PDN &= ~(Px0_MASK | Px1_MASK);            // Clear PDN 
}
void PORT_L_Init(void)
{
    TSB_CG->FSYSMENA |= CG_PORTL;            // Enable clock for PORTL

    /* 
     *  Follow 4.2.12. PORT L
     *
     *  Note: When using analog input(AINAx), [PLCR] should be output disable "0",[PLIE] should be input disable "0",
     *  [PLPUP] should be pull-up disable"0" and [PLPDN] should be pull-down disable "0".
     */

    //Disable necessary registers for AINC00/AINC01 to function as ADC inputs
    TSB_PL->CR &= ~(Px0_MASK | Px1_MASK);             // Clear CR 
    TSB_PL->IE &= ~(Px0_MASK | Px1_MASK);             // Clear IE 
    TSB_PL->PUP &= ~(Px0_MASK | Px1_MASK);            // Clear PUP 
    TSB_PL->PDN &= ~(Px0_MASK | Px1_MASK);            // Clear PDN 
}






/* ___________________________________________________________________________________
 *
 *  Macro/Helper functions for GPIO configuration
 *  - These functions can be used to simplify the process of configuring GPIO pins by abstracting
 * ___________________________________________________________________________________ 
 */


void PA_FRn_Clear(uint32_t pin_mask)
{
    TSB_PA->FR1 &= ~pin_mask;
    TSB_PA->FR4 &= ~pin_mask;
    TSB_PA->FR5 &= ~pin_mask;
    TSB_PA->FR6 &= ~pin_mask;
    TSB_PA->FR7 &= ~pin_mask;
    /* Note: FR2 & FR3 are RESERVED for Port A per TMPM4KNA.h */
}

void PC_FRn_Clear(uint32_t pin_mask)
{
    TSB_PC->FR1 &= ~pin_mask;
    TSB_PC->FR2 &= ~pin_mask;
    TSB_PC->FR3 &= ~pin_mask;
    TSB_PC->FR4 &= ~pin_mask;
    TSB_PC->FR5 &= ~pin_mask;
    TSB_PC->FR6 &= ~pin_mask;
    TSB_PC->FR7 &= ~pin_mask;
}

void PD_FRn_Clear(uint32_t pin_mask)
{
    TSB_PD->FR1 &= ~pin_mask;
    TSB_PD->FR2 &= ~pin_mask;
    TSB_PD->FR3 &= ~pin_mask;
    TSB_PD->FR4 &= ~pin_mask;
    TSB_PD->FR5 &= ~pin_mask;
    TSB_PD->FR6 &= ~pin_mask;
    /* Note: FR7 is RESERVED for Port D per TMPM4KNA.h */
}

void PG_FRn_Clear(uint32_t pin_mask)
{
    TSB_PG->FR1 &= ~pin_mask;
    TSB_PG->FR4 &= ~pin_mask;
    TSB_PG->FR5 &= ~pin_mask;
    /* Note: FR2, FR3, FR6, FR7 are RESERVED for Port G per TMPM4KNA.h */
}

void PN_FRn_Clear(uint32_t pin_mask)
{
    TSB_PN->FR1 &= ~pin_mask;
    TSB_PN->FR2 &= ~pin_mask;
    TSB_PN->FR3 &= ~pin_mask;
    TSB_PN->FR4 &= ~pin_mask;
    TSB_PN->FR5 &= ~pin_mask;
    TSB_PN->FR6 &= ~pin_mask;
    TSB_PN->FR7 &= ~pin_mask;
}

void PU_FRn_Clear(uint32_t pin_mask)
{
    TSB_PU->FR1 &= ~pin_mask;
    TSB_PU->FR2 &= ~pin_mask;
    TSB_PU->FR3 &= ~pin_mask;
    TSB_PU->FR4 &= ~pin_mask;
    TSB_PU->FR5 &= ~pin_mask;
    TSB_PU->FR6 &= ~pin_mask;
    TSB_PU->FR7 &= ~pin_mask;
}
