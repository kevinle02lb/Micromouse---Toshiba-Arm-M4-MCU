/**
 *************************************************************************
 * @file        encoder32A.c
 * @brief       A-ENC32 driver implementation for TMPM4Ky microcontrollers.
 * @version     V1.0.0
 * @date        25-05-2026
 * 
 * 
 * @details
 *  This driver provides functions to configure and control the A-ENC32 hardware encoders
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

#ifndef ENCODER32A_H
#define ENCODER32A_H

/* Inclusions */
#include "TMPM4KyA.h"
#include <stdint.h>
#include <stdbool.h>



/* Defines */

/* [FSYSMENx] Clock Gate (CG) */
#define ENC0_CG_FSYSMENB_IPMENB06               ((uint32_t)0x00000001 << 6U)       /*!< CG FSYSMENB A-ENC32 ch0 mask */
#define ENC2_CG_FSYSMENB_IPMENB08               ((uint32_t)0x00000001 << 8U)       /*!< CG FSYSMENB A-ENC32 ch2 mask */

/* [TNCR] Operation Modes */
#define TNCR_MODE_MASK                          ((uint32_t)0x07 << 17U)            /*!< TNCR mode mask */
#define TNCR_ZEN_MASK                           ((uint32_t)0x01 << 7U)             /*!< TNCR Z input enable mask */
#define TNCR_P3EN_MASK                          ((uint32_t)0x01 << 16U)            /*!< TNCR Phase 3 input enable mask */
#define TNCR_ENRUN_MASK                         ((uint32_t)0x01 << 6U)             /*!< TNCR Encoder input circuit enable */
#define TNCR_ENCLR_MASK                         ((uint32_t)0x01 << 10U)            /*!< TNCR Encoder Counter Clear */
#define TNCR_DECMD_MASK                         ((uint32_t)0x03 << 22U)            /*!< TNCR Selection of Decoder detection direction */

/* [ENxINPCR] (Input Procedure Control Register) */
#define INPCR_SYNCSPLEN_MASK                    ((uint32_t)0x01 << 0U)             /*!< INPCR PWM synchronous sampling enable */
#define INPCR_SYNCSPLMD_MASK                    ((uint32_t)0x01 << 1U)             /*!< INPCR PWM synchronous sampling selection */ 
#define INPCR_SYNCNCZEN_MASK                    ((uint32_t)0x01 << 2U)             /*!< INPCR Noise cancellation counter control at PWM-on period sampling */ 
#define INPCR_NCT_MASK                          ((uint32_t)0x7F << 8U)             /*!< INPCR Noise cancellation time */
#define INPCR_NCT_400ns                         ((uint32_t)0x04 << 8U)             /* Noise cancellation time: <NCT[6:0]> × Sample clock cycle */


/* [ENxCLKCR] (Sample Clock Control Register) */
#define CLKCR_SPLCKS_8DIV                       ((uint32_t)0x03 << 0U)             /*!< CLKCR Sampling frequency  */

/* [ENxSTS] (Status Register) */
#define STS_UD_MASK                             ((uint32_t)0x01 << 13U)            /*!< STS UD CW/CCW Rotation direction judgment */

/* [ENxRELOAD] (RELOAD Comparison Register) */
#define RELOAD_MAX                              ((uint32_t)0xFFFFFFFF)             /* [Encoder mode] Sets the maximum value of the counter */

/* [ENxINTCR] (Interrupt Control Register) */
#define INTCR_CLEAR_ALL                         ((uint32_t)0x3F << 0U)             /*!< INTCR Interrupt Enables*/

/* Function Prototypes */
void ENC32A_Init(void);
void ENC0_Init(void);
void ENC2_Init(void);
void ENC0_Start(void);
void ENC2_Start(void);
void ENC0_Stop(void);
void ENC2_Stop(void);
void ENC0_ClearCNT(void);
void ENC2_ClearCNT(void);

bool ENC0_GetStatus(void);
bool ENC2_GetStatus(void);

int32_t ENC0_ReadCount(void);
int32_t ENC2_ReadCount(void);

#endif /* ENCODER32A_H */