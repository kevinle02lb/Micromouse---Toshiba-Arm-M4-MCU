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

#include "irSensor.h"
#include "drivers/adc.h"
#include "drivers/dma.h"
#include "drivers/gpio.h"

static IR_SensorData g_irData;


/* ==========================================================================
 *   Initialization
 * ========================================================================== */
/**
 * @brief  Initializes ADC & GPIO for Emitters
 * @note   Make sure DMAC is initialized before this is called
 */
 void IR_Init(void)
{
    ADC_Init();
    PORT_U_Init();       /* Left Emitters  */
    PORT_G_Init();       /* Right Emitters */
}

/* ==========================================================================
 *   IR Reiever 
 * ========================================================================== */

void IR_EmitterOn(IR_Channel ch)
{
    switch(ch) 
    {
        case IR_FAR_LEFT:  FarLeftEmitterOn();   break;
        case IR_LEFT:      LeftEmitterOn();      break;
        case IR_RIGHT:     RightEmitterOn();     break;
        case IR_FAR_RIGHT: FarRightEmitterOn();  break;
        default: break;
    }
}

void IR_EmitterOff(IR_Channel ch)
{
    switch(ch) 
    {
        case IR_FAR_LEFT:  FarLeftEmitterOff();   break;
        case IR_LEFT:      LeftEmitterOff();      break;
        case IR_RIGHT:     RightEmitterOff();     break;
        case IR_FAR_RIGHT: FarRightEmitterOff();  break;
        default: break;
    }
}

void IR_AllEmittersOff(void)
{
    GPIO_U_ClrData(Px0_MASK | Px1_MASK);
    GPIO_G_ClrData(Px4_MASK | Px5_MASK);
}


/**
 * @brief  Samples IR Sensors
 * @note   
 */
void IR_SampleAll(void)
{
    volatile uint16_t *bufA, *bufC;
    
    /* --- PHASE 1: Ambient reading (all emitters OFF) --- */
    IR_AllEmittersOff();
    SysTick_us(50);              // Wait for previous IR to decay (~50µs)
    
    Start_ADC();                 // Trigger both ADC units
    // Wait for DMA completion (poll or use interrupt)
    // For now: small delay sufficient for 2 conversions @ 40MHz
    SysTick_us(20);              // ~2×(0.96µs sample + 12-bit conversion)
    
    bufA = DMA_GetADCABuffer();
    bufC = DMA_GetADCCBuffer();
    
    g_irData.ambient[IR_FAR_LEFT]  = bufA[0];  // AINA16
    g_irData.ambient[IR_LEFT]      = bufA[1];  // AINA15
    g_irData.ambient[IR_RIGHT]     = bufC[0];  // AINC01
    g_irData.ambient[IR_FAR_RIGHT] = bufC[1];  // AINC00
    
    /* --- PHASE 2: Reflected reading (all emitters ON) --- */
    FarLeftEmitterOn();
    LeftEmitterOn();
    RightEmitterOn();
    FarRightEmitterOn();
    SysTick_us(100);             // Allow IR to reach wall and reflect back
    
    Start_ADC();                 // Re-arm DMA + trigger
    
    SysTick_us(20);              // Wait for conversion
    
    g_irData.raw[IR_FAR_LEFT]  = bufA[0];
    g_irData.raw[IR_LEFT]      = bufA[1];
    g_irData.raw[IR_RIGHT]     = bufC[0];
    g_irData.raw[IR_FAR_RIGHT] = bufC[1];
    
    /* --- PHASE 3: Calculate reflected values --- */
    for (int i = 0; i < IR_COUNT; i++) 
    {
        g_irData.reflected[i] = (g_irData.raw[i] > g_irData.ambient[i]) 
                              ? (g_irData.raw[i] - g_irData.ambient[i]) 
                              : 0;
    }
    
    /* Optional: turn off emitters to save power / reduce interference */
    IR_AllEmittersOff();
}


const IR_SensorData* IR_GetData(void) { return &g_irData; }

uint16_t IR_GetReflected(IR_Channel ch) { return g_irData.reflected[ch]; }


/* ==========================================================================
 *   Control Calls for IR Emitters
 * ========================================================================== */

/**
 * @brief  Control functions to toggle Data outpins for IR emiiters
 */
void FarLeftEmitterOn(void)     { GPIO_U_SetData((uint8_t)Px1_MASK); }
void LeftEmitterOn(void)        { GPIO_U_SetData((uint8_t)Px0_MASK); }
void RightEmitterOn(void)       { GPIO_G_SetData((uint8_t)Px5_MASK); }
void FarRightEmitterOn(void)    { GPIO_G_SetData((uint8_t)Px4_MASK); }

void FarLeftEmitterOff(void)    { GPIO_U_ClrData((uint8_t)Px1_MASK); }
void LeftEmitterOff(void)       { GPIO_U_ClrData((uint8_t)Px0_MASK); }
void RightEmitterOff(void)      { GPIO_G_ClrData((uint8_t)Px5_MASK); }
void FarRightEmitterOff(void)   { GPIO_G_ClrData((uint8_t)Px4_MASK); }

void FarLeftEmitterToggle(void) { GPIO_U_ToggleData((uint8_t)Px1_MASK); }
void LeftEmitterToggle(void)    { GPIO_U_ToggleData((uint8_t)Px0_MASK); }
void RightEmitterToggle(void)   { GPIO_G_ToggleData((uint8_t)Px5_MASK); }
void FarRightEmitterToggle(void){ GPIO_G_ToggleData((uint8_t)Px4_MASK); }