/**
 *************************************************************************
 * @file        timer32A.h
 * @brief       Timer32A driver implementation for TMPM4Ky microcontrollers.
 * @version     V1.0.0
 * @date        11-05-2026
 * 
 * 
 * @details
 *  This driver provides functions to configure and control the Timer32A peripheral
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

#ifndef TIMER32A_H
#define TIMER32A_H

#include "TMPM4KyA.h"
#include <stdint.h>

/* Timer32A mode definitions */
#define CG_PROTECT_UNLOCK           ((uint32_t)0xC1)

/* User-defined values */
#define T32A_CH0_PERIOD              2000               // 80Mhz / 1:1 / 2000 = 40kHz PWM frequency
#define T32A_CH3_PERIOD              2000               // 80Mhz / 1:1 / 2000 = 40kHz PWM frequency

/*! Timer32A 32/16-bit mode : 0 - 16b, 1 - 32b */
#define T32A_MODE_SEL           0                                  
/*!< Timer32A prescaler selection Division Ratio 
Division Ratio (3'b):
000 - 1:1 - 0'd
001 - 1:2 - 1'd
010 - 1:8 - 2'd
011 - 1:32 - 3'd
100 - 1:128 - 4'd
101 - 1:256 - 5'd
110 - 1:512 - 6'd
111 - 1:1024 - 7'd */
#define T32A_PRSCL_SEL          0           /* Decimal value */                                  

/* _________________________________________________________________________________
 *
 *  Other Definitions
 *__________________________________________________________________________________
 */

/* CG Control */
#define T32A0_CG_FSYSMENA_IPMENA28      ((uint32_t)0x00000001 << 28U)      /*!< CG FSYSMENA Timer32A ch0 mask */   
#define T32A3_CG_FSYSMENA_IPMENA31      ((uint32_t)0x00000001 << 31U)      /*!< CG FSYSMENA Timer32A ch3 mask */   

/* Mode Register */
#define T32A_MODE_MASK              ((uint32_t)0x00000003)             /*!< Timer32A mode mask */
#define T32A_MODE32                 ((uint32_t)T32A_MODE_SEL)          /*!< Timer32A 32/16-bit mode : 0 - 16b, 1 - 32b */ 
#define T32A_HALT_MODE              ((uint32_t)0x00000002)             /*!< Timer32A halt mode */

/* Run Register */
#define T32A_RUNx_RUNFLGx_MASK      ((uint32_t)0x01 << 4U)              /*!< Timer32A channel x run flag mask */
#define T32A_RUNx_MASK              ((uint32_t)0x01 << 0U)              /*!< Timer32A channel x run control mask */

/* Counter Control Register */
#define T32A_CRx_PRSCLC_MASK        ((uint32_t)0x07 << 28U)            /*!< Timer32A channel 0 Timer xprescaler mask */
#define T32A0_CRA_PRSCLA            ((uint32_t)T32A_PRSCL_SEL << 28U)  /*!< Timer32A channel 0 Timer A prescaler selection */
#define T32A0_CRB_PRSCLB            ((uint32_t)T32A_PRSCL_SEL << 28U)  /*!< Timer32A channel 0 Timer B prescaler selection */
#define T32A3_CRA_PRSCLA            ((uint32_t)T32A_PRSCL_SEL << 28U)  /*!< Timer32A channel 3 Timer A prescaler selection */
#define T32A3_CRB_PRSCLB            ((uint32_t)T32A_PRSCL_SEL << 28U)  /*!< Timer32A channel 3 Timer B prescaler selection */

#define T32A_CRx_RELDx_MASK         ((uint32_t)0x03 << 8U)             /*!< Timer32A channel RELDA[2:0] mask for CRx Register*/

#define T32A_CRx_UPDNx_MASK         ((uint32_t)0x00000003 << 16U)      /*!< Timer32A Counting Direction */
#define T32A_UPDNx                  ((uint32_t)0x00000000 << 16U)      /* 00 - Up, 01 - Down, 10 - Up/Down , 11 - Reserved */

#define T32A_CRx_WBFx_MASK          ((uint32_t)0x01 << 20U)            /*!< Timer32A Timer x WBF bit mask. 0 - Disabled, 1 - Enabled */


/* Output Control Register */
#define T32A_OUTCRx0_ORCx_MASK      ((uint32_t)0x00000003 << 0U)       /*!< Timer32A Timer x output control mask */

#define T32A_OUTCRx1_MASK           ((uint32_t)0x000000FF << 0U)       /*!< Timer32A Timer x output control mask for bits 7:0 */
#define T32A_OUTCRx1_OCRCMPx0_MASK  ((uint32_t)0x00000003 << 0U)       /*!< Timer32A Timer x output control compare match output control for compare register 0 */
#define T32A_OUTCRx1_OCRCMPx1_MASK  ((uint32_t)0x00000003 << 2U)       /*!< Timer32A Timer x output control compare match output control for compare register 1 */

#define OUTCRxx_FTN_INVALID         0x00
#define OUTCRxx_FTN_SET             0x01
#define OUTCRxx_FTN_CLEAR           0x02
#define OUTCRxx_FTN_REVERSED        0x03
#define T32A_OUTCRx1_OCRCMPx0_SET   (OUTCRxx_FTN_SET << 0U)           /*!< Timer32A Timer x output control compare match output control for compare register 0 set */
#define T32A_OUTCRx1_OCRCMPx0_CLEAR (OUTCRxx_FTN_CLEAR << 0U)         /*!< Timer32A Timer x output control compare match output control for compare register 0 clear */
#define T32A_OUTCRx1_OCRCMPx1_SET   (OUTCRxx_FTN_SET << 2U)           /*!< Timer32A Timer x output control compare match output control for compare register 1 set */
#define T32A_OUTCRx1_OCRCMPx1_CLEAR (OUTCRxx_FTN_CLEAR << 2U)         /*!< Timer32A Timer x output control compare match output control for compare register 1 clear */

// Output Control Register 1 (OUTCRx1) Preset Modes
#define T32A_OUTPUT_HIGH        ((uint32_t)(T32A_OUTCRx1_OCRCMPx0_SET | T32A_OUTCRx1_OCRCMPx1_SET))     /*!< Force output HIGH */
#define T32A_OUTPUT_LOW         ((uint32_t)(T32A_OUTCRx1_OCRCMPx0_CLEAR | T32A_OUTCRx1_OCRCMPx1_CLEAR)) /*!< Force output LOW */
#define T32A_OUTPUT_PPG         ((uint32_t)(T32A_OUTCRx1_OCRCMPx0_SET | T32A_OUTCRx1_OCRCMPx1_CLEAR))   /*!< PPG: SET on CMP0, CLEAR on CMP1 */
#define T32A_OUTPUT_INVERTED    ((uint32_t)(T32A_OUTCRx1_OCRCMPx0_CLEAR | T32A_OUTCRx1_OCRCMPx1_SET))   /*!< PPG: CLEAR on CMP0, SET on CMP1 */

/* Function prototypes */
void T32A_Init(void);
void T32A0_Init(uint16_t period);
void T32A3_Init(uint16_t period);
void T32A0_Start(void);
void T32A3_Start(void);
void T32A0_Stop(void);
void T32A3_Stop(void);

/*  Timer Registers A/B (16-bit)  */

void T32A0_SetTimerA0(uint16_t val);          // RGA0
void T32A0_SetTimerA1(uint16_t val);          // RGA1
void T32A0_SetTimerB0(uint16_t val);          // RGB0
void T32A0_SetTimerB1(uint16_t val);          // RGB1
void T32A3_SetTimerA0(uint16_t val);
void T32A3_SetTimerA1(uint16_t val);
void T32A3_SetTimerB0(uint16_t val);
void T32A3_SetTimerB1(uint16_t val);

/*  Output Control Registers  */
void T32A0_SetOutCRA1(uint32_t mode);         // OUTCRA1
void T32A0_SetOutCRB1(uint32_t mode);         // OUTCRB1
void T32A3_SetOutCRA1(uint32_t mode);
void T32A3_SetOutCRB1(uint32_t mode);


#endif /* TIMER32A_H */