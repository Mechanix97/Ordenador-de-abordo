#ifndef _GAS_H_
#define  _GAS_H_

#include <stdint.h>

#include "stm32f1xx_hal.h"

/* Board v2: 5 V -> 1N4148 -> 150 R -> fuel sender to GND, read on PA7 (12-bit ADC, 3.3 V).
 * Expected about 0.2 V (full) to 1.7 V (reserve). Calibrate with the raw value shown on the
 * display: tank full and tank on reserve. */
#define GAS_RAW_FULL     250
#define GAS_RAW_RESERVE  2100

extern ADC_HandleTypeDef hadc2;

void GAS_init();
uint32_t GAS_read();
uint8_t GAS_percent(uint32_t raw);

#endif
