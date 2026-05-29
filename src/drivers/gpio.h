/**
 *************************************************************************
 * @file        gpio.h
 * @brief       GPIO driver implementation for TMPM4Ky microcontrollers.
 * @version     V1.0.0
 * @date        11-05-2026
 * 
 * 
 * @details
 *  This driver provides functions to configure and control the GPIO peripheral
 * 
 * Copyright (c) [Kevin Le] 2026
 **************************************************************************
 */

#ifndef GPIO_H
#define GPIO_H

 /* Inclusions */
#include "TMPM4KyA.h"
#include <stdint.h>


/* Defines */

/* Port X Defines Mask Bit */
#define Px0_MASK            ((uint32_t)0x01 << 0U)    /*!< Px0 mask */
#define Px1_MASK            ((uint32_t)0x01 << 1U)    /*!< Px1 mask */
#define Px2_MASK            ((uint32_t)0x01 << 2U)    /*!< Px2 mask */
#define Px3_MASK            ((uint32_t)0x01 << 3U)    /*!< Px3 mask */
#define Px4_MASK            ((uint32_t)0x01 << 4U)    /*!< Px4 mask */
#define Px5_MASK            ((uint32_t)0x01 << 5U)    /*!< Px5 mask */

#define PxFRn_CLEAR         ((uint32_t)0x00)          /*!< Clear Function Register n bits */

/* Clock Gating Masks for Each GPIO Port - CG*/
#define CG_PORTA            ((uint32_t)0x01 << 0U)    /*!< CG PORTA mask */
#define CG_PORTC            ((uint32_t)0x01 << 2U)    /*!< CG PORTC mask */
#define CG_PORTD            ((uint32_t)0x01 << 3U)    /*!< CG PORTD mask */
#define CG_PORTG            ((uint32_t)0x01 << 6U)    /*!< CG PORTG mask */
#define CG_PORTJ            ((uint32_t)0x01 << 8U)    /*!< CG PORTJ mask */
#define CG_PORTL            ((uint32_t)0x01 << 10U)   /*!< CG PORTL mask */
#define CG_PORTN            ((uint32_t)0x01 << 12U)   /*!< CG PORTN mask */
#define CG_PORTU            ((uint32_t)0x01 << 16U)   /*!< CG PORTU mask */



/* Function Prototypes */
void GPIO_Init(void);
void PORT_A_Init(void);
void PORT_C_Init(void);
void PORT_D_Init(void);
void PORT_G_Init(void);
void PORT_J_Init(void);
void PORT_L_Init(void);
void PORT_N_Init(void);
void PORT_U_Init(void);

void PA_FRn_Clear(uint32_t pin_mask);
void PC_FRn_Clear(uint32_t pin_mask);
void PD_FRn_Clear(uint32_t pin_mask);
void PG_FRn_Clear(uint32_t pin_mask);
void PN_FRn_Clear(uint32_t pin_mask);
void PU_FRn_Clear(uint32_t pin_mask);




#endif /* GPIO_H */