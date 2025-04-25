/*
 * pid.h
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */

// pid.h
#ifndef INC_PID_H_
#define INC_PID_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g4xx_hal.h"   // Налаштуйте цей підключення відповідно до вашої родини STM32 | Adjust this include according to your STM32 family
#include <stdint.h>


// Визначте поріг помилки, якщо він не визначений деінде.
// Define the error threshold if it is not defined elsewhere.

// Налаштуйте значення за потребою.
// Adjust the value as needed.

#ifndef ERROR_THRESHOLD
#define ERROR_THRESHOLD 50  // Дозволена похибка | Allowed error
#endif

// Структура змінних PID
// PID variables structure
typedef struct {
    volatile uint32_t Kp;
    volatile uint32_t Ki;
    volatile uint32_t Kd;
    float         error;
    volatile int32_t  setpoint;
    float         previous_error;
    float         Proportional;
    float         Integral;
    float         Derivative;
    volatile int32_t  scale;
    float         Control;
    float         Control_000;
} PID;

// Глобальна змінна для прогальности
// Global variable for duty cycle
extern volatile int32_t duty;

// Оригінальна ініціалізація та обчислення
// Original init and compute
void PID_Init(PID *pid);
void PIDcomputation(TIM_HandleTypeDef *htim,
                    PID *pid,
                    uint32_t measured_value,
                    uint32_t dt,
                    volatile int32_t *duty_cycle,
                    const uint32_t *ChannelToModify);

#ifdef __cplusplus
}
#endif

#endif /* INC_PID_H_ */
