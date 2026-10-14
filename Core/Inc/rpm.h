#ifndef _RPM_H_
#define _RPM_H_

#include <stdint.h>

#include "stm32f1xx_hal.h"

/* Sparks every two crankshaft revolutions (one 4-stroke cycle).
 * 2: the CDI fires on every revolution (wasted spark, usual on CG-type engines like the RX 150).
 * 1: the CDI only fires on the compression stroke.
 * Check it against the bike's tachometer at a steady RPM. */
#define RPM_SPARKS_PER_2_REV 2

extern TIM_HandleTypeDef htim3;

void RPM_init();
uint32_t RPM_read();

#endif
