/* Set duty cycle for Timer32A channel 3 PWM output */
    // Duty [%] = (RGx1 - RGx0) / (RGx1 - RELDx) * 100
    // RGx0 = RGx1 - (Duty [%] * RGx1 / 100)