/*
 * pid.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */

#include "SIXSTEPHeader/MAL_declaration.h"
#include <stdio.h>

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;


volatile int32_t duty = 300;

void PID_Init(PID *pid) {
    pid->setpoint       = 2200;
    pid->Kp             = 800;
    pid->Ki             = 40;
    pid->Kd             = 10;
    pid->scale          = 1000000;
    pid->previous_error = 0.0f;
}



void PIDcomputation(TIM_HandleTypeDef *htim,
                    PID               *pid,
                    uint32_t           measured_value,
                    uint32_t           dt,
                    volatile int32_t  *duty_cycle,
                    const uint32_t    *ChannelToModify)
{


	// Обчислити помилку та окремі значення ПІД
    // Calculate error and individual PID values
    pid->error        = pid->setpoint - (int32_t)measured_value;
    pid->Proportional = (pid->Kp * pid->error) / pid->scale;

    pid->Integral     = (pid->Ki * pid->error * dt) / pid->scale;
    pid->Derivative   = (pid->Kd * (pid->error - pid->previous_error) / dt) / pid->scale;
    pid->Control      = pid->Proportional + pid->Integral + pid->Derivative;

    // Зменшити для коригування прогальности
    // Scale down for duty adjust
    pid->Control_000  = pid->Control / 10.0f;

    // Зберегти для наступного інтегрування/диференціювання
    // Save for next integral/derivative
    pid->previous_error = pid->error;


    // Only adjust if outside ERROR_THRESHOLD
    if (pid->error <= -ERROR_THRESHOLD || pid->error >= ERROR_THRESHOLD) {
        //*duty_cycle -= (int32_t)pid->Control_000;
    	*duty_cycle = *duty_cycle - pid->Control_000;
    }

    // Обмежити в межах [0...Період]
    // Clamp [0...Period]
    if (*duty_cycle < 0)      *duty_cycle = 0;
    else if (*duty_cycle > htim1.Init.Period) *duty_cycle = htim1.Init.Period;



    // Понови РЗП (Регістр Захоплення/Порівняння)
    // Update the CCR value
    __HAL_TIM_SET_COMPARE(htim, ChannelToModify[0], *duty_cycle);
    __HAL_TIM_SET_COMPARE(htim, ChannelToModify[1], *duty_cycle);
    __HAL_TIM_SET_COMPARE(htim, ChannelToModify[2], *duty_cycle);

}

