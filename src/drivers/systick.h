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
 *  Main ARM documentation instead of Toshiba's
 *  https://developer.arm.com/documentation/ddi0403/ee
 *  
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */


#ifndef SYSTICK_H
#define SYSTICK_H

/*Inclusions*/
#include "TMPM4KyA.h"                   // Includes core_cm4.h for Systick
#include <stdint.h> 


void SysTick_us(uint32_t val);
void SysTick_ms(uint32_t val);

#endif /* SYSTICK_H */