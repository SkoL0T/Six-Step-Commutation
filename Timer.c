/*
 * Timer.c
 *
 *  Created on: Apr 24, 2025
 *      Author: Yehor Borsa
 */


#include "main.h"

extern TIM_HandleTypeDef htim2;


uint32_t GetMicros(void) {
 return __HAL_TIM_GET_COUNTER(&htim2); // Беремо час годинника | Get the clock time
}
