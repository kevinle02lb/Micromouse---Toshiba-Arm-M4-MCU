/**
 *************************************************************************
 * @file        systick.c
 * @brief       Systick driver implementation for TMPM4Ky RISC microcontrollers.
 * @version     V1.0.0
 * @date        29-05-2026
 * 
 * 
 * @details
 *  This driver provides initialization and control functions for the Systick delays.
 *  No Need for ISR handlers.
 *  https://developer.arm.com/documentation/ddi0403/ee
 *  
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

/* Inclusions */
#include "systick.h"



void SysTick_us(uint32_t val)
{
    if (val == 0U) { return; }
    // Clamp
    if (val > (SysTick_LOAD_RELOAD_Msk / 160U))
    {
        val = SysTick_LOAD_RELOAD_Msk / 160U;
    }

    /* [1.] Reload Val
     *  Ex:
     *  System clock: fc = 160[Mhz] -> 1uS * 160[Mhz] = 160 Cycles for 1uS
     *  Load = (Val * 160 Cycles) - 1
     */
    SysTick->LOAD = (uint32_t)((val * 160) - 1U);

    /* [2.] Clear Current Value Reg - Any write to the register clears the register to zero */
    SysTick->VAL = 0;

    /* [3.] Config SysTick Control Reg 
     *
     *  CLKSOURCE, bit[2]   - 1   :   SysTick uses the processor clock. 
     *                        0   :   SysTick uses the IMPLEMENTATION DEFINED external reference clock.
     * 
     *  TICKINT, bit[1]     - 1   :   Count to 0 changes the SysTick exception status to pending. 
     *                        0   :   Count to 0 does not affect the SysTick exception status
     * 
     *  ENABLE, bit[0]      - 1   :   Counter is operating.
     *                        0   :   Counter is disabled.
     * 
     * App:
     *  CLKSOURCE   = 1
     *  TICKINT     = 0
     *  ENABLE      = 1
     */
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);

    /* [4.] Poll COUNTFLAG */
    while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) ) { ; }

    /* [5.] Disable SysTick */
    SysTick->CTRL = 0;              // Clears Flag and everything for next call

}


void SysTick_ms(uint32_t val)
{
    /* Max LOAD = 0xFFFFFF = 16,777,215 cycles
     * At 160MHz: max single delay = 16,777,215/160 = ~104ms
     * For longer delays, loop 
     */
    for (uint32_t i = 0U; i < val; i++)
    {
        Systick_us(1000U);
    }
}