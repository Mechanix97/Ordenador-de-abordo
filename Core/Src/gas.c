#include "gas.h"

void GAS_init(){
    HAL_ADCEx_Calibration_Start(&hadc2);
}

uint32_t GAS_read(){
    HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2, 1);
    uint32_t AD_RES2 = HAL_ADC_GetValue(&hadc2);
    /* do something */
    return AD_RES2;
}

/* 100 = full, 0 = reserve. The sender voltage rises as the tank empties. */
uint8_t GAS_percent(uint32_t raw){
    if (raw <= GAS_RAW_FULL) {
        return 100;
    }
    if (raw >= GAS_RAW_RESERVE) {
        return 0;
    }
    return 100 - (100 * (raw - GAS_RAW_FULL)) / (GAS_RAW_RESERVE - GAS_RAW_FULL);
}
