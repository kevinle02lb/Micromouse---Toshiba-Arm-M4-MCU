/**
 * @file        irSensor.c
 * @brief       IR Sensor control to emit and read
 * @version     V1.0.0
 * @date        11-06-2026
 *
 * @details
 *   Pin Map:
 *      - PU1 = Far Left IR Emitter, PU0 = Left IR Emitter (Left IR Emitters)
 *      - PG5 = Right IR Emitter, PG4 = Far Right IR Emitter (Right IR Emitters)
 * 
 *      - Unit A: PL0 (AINA16), PL1 (AINA15) — Left IR sensors
 *      - Unit C: PJ0 (AINC00), PJ1 (AINC01) — Right IR sensors
 *      - PL0 = Far Left IR, PL1 = Left IR (Left ADC)
 *      - PJ1 = Right IR, PJ0 = Far Right IR (Right ADC)
 *  
 * @note
 *
 *   File structure and Doxygen formatting assisted by AI.
 *
 * Copyright (c) [Kevin Le] 2026
 */


#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <stdint.h>


typedef enum
{
    IR_FAR_LEFT = 0,   // AINA16 @ PL0
    IR_LEFT,           // AINA15 @ PL1
    IR_RIGHT,          // AINC01 @ PJ1
    IR_FAR_RIGHT,      // AINC00 @ PJ0
    IR_COUNT
}IR_Channel;

typedef struct 
{
    uint16_t raw[IR_COUNT];             // Latest ADC reading
    uint16_t ambient[IR_COUNT];         // Background light (emitter OFF)
    uint16_t reflected[IR_COUNT];       // True wall reflection (ON - OFF)
    bool     wallDetected[IR_COUNT];
} IR_SensorData;



/* ==========================================================================
 *   Initialization
 * ========================================================================== */

void IR_Init(void);


/* Sequenced measurement (call this from your control tick) */
void IR_SampleAll(void);         // Full ON/OFF sampling cycle

/* Data access */
const IR_SensorData* IR_GetData(void);
uint16_t IR_GetRaw(IR_Channel ch);
uint16_t IR_GetReflected(IR_Channel ch);
bool IR_IsWallDetected(IR_Channel ch, uint16_t threshold);

/* Low-level emitter control (existing) */
void IR_EmitterOn(IR_Channel ch);
void IR_EmitterOff(IR_Channel ch);
void IR_AllEmittersOn(void);
void IR_AllEmittersOff(void);


/* ==========================================================================
 *   Low Level IR Emitter Function Calls
 * ========================================================================== */

void FarLeftEmitterOn(void);
void LeftEmitterOn(void);
void RightEmitterOn(void);
void FarRightEmitterOn(void);

void FarLeftEmitterOff(void);
void LeftEmitterOff(void);
void RightEmitterOff(void);
void FarRightEmitterOff(void);

void FarLeftEmitterToggle(void);
void LeftEmitterToggle(void);
void RightEmitterToggle(void);
void FarRightEmitterToggle(void);


#endif