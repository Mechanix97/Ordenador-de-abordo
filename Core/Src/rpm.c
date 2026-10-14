#include "rpm.h"

/* Board v2: the coil drives a PC817; its output pulls PA6 low on every spark.
 * TIM3_CH1 captures the falling edges with a 100 kHz time base (72 MHz / 720). */
#define RPM_TIMER_HZ       100000UL
/* Ignition ringing: edges closer than 2 ms to the last spark are ignored (limit: 30000 sparks/min) */
#define RPM_BLANKING_TICKS (RPM_TIMER_HZ * 2 / 1000)
/* No spark for this long = engine stopped. Also below the 655 ms counter wrap-around. */
#define RPM_TIMEOUT_MS     500

static volatile uint16_t last_capture;
static volatile uint32_t last_spark_ms;
static volatile uint32_t period_ticks;  /* 0 = no valid period yet */

void RPM_init() {
    period_ticks = 0;
    last_spark_ms = HAL_GetTick();
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
}

uint32_t RPM_read() {
    __disable_irq();
    uint32_t period = period_ticks;
    uint32_t since = HAL_GetTick() - last_spark_ms;
    __enable_irq();

    if (period == 0 || since > RPM_TIMEOUT_MS) {
        return 0;
    }
    return (120UL * RPM_TIMER_HZ) / (period * RPM_SPARKS_PER_2_REV);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance != TIM3 || htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1) {
        return;
    }
    uint16_t capture = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    uint16_t ticks = capture - last_capture;  /* modulo 2^16 */
    uint32_t now = HAL_GetTick();

    if (now - last_spark_ms > RPM_TIMEOUT_MS) {
        /* First spark after a stop: the counter may have wrapped, no valid period yet */
        period_ticks = 0;
    } else if (ticks < RPM_BLANKING_TICKS) {
        return;
    } else {
        period_ticks = ticks;
    }
    last_capture = capture;
    last_spark_ms = now;
}
