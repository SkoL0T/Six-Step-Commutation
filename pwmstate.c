/*
 * pwmstate.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */


#include "main.h"

#include "SIXSTEPHeader/MAL_declaration.h"

extern TIM_HandleTypeDef htim1;

ChannelState sequences[6][3] = {
    {HIGH_SIDE_ON, LOW_SIDE_ON, OFF},
    {HIGH_SIDE_ON, OFF, LOW_SIDE_ON},
    {OFF, HIGH_SIDE_ON, LOW_SIDE_ON},
    {LOW_SIDE_ON, HIGH_SIDE_ON, OFF},
    {LOW_SIDE_ON, OFF, HIGH_SIDE_ON},
    {OFF, LOW_SIDE_ON, HIGH_SIDE_ON},
};

uint32_t lastSideChannel[3] = {0};



void HardSwitching(TIM_HandleTypeDef *htim, uint32_t channel, ChannelState state) {

    switch (state) {

        case HIGH_SIDE_ON:
            HAL_TIM_PWM_Start(htim, channel);
            HAL_TIMEx_PWMN_Stop(htim, channel);
            lastSideChannel[0] = channel;
            break;

        case LOW_SIDE_ON:
            HAL_TIM_PWM_Stop(htim, channel);
            HAL_TIMEx_PWMN_Start(htim, channel);
            lastSideChannel[1] = channel;
            break;

        case OFF:
            HAL_TIM_PWM_Stop(htim, channel);
            HAL_TIMEx_PWMN_Stop(htim, channel);
            lastSideChannel[2] = channel;
            break;

    }
}

void SoftSwitching(TIM_HandleTypeDef *htim, uint32_t channel, ChannelState state) {

    switch (state) {

        case HIGH_SIDE_ON:
            HAL_TIM_PWM_Start(htim, channel);
            HAL_TIMEx_PWMN_Stop(htim, channel);
            lastSideChannel[0] = channel;
            break;

        case LOW_SIDE_ON:
            HAL_TIM_PWM_Stop(htim, channel);
            HAL_TIMEx_PWMN_Start(htim, channel);
            __HAL_TIM_SET_COMPARE(htim, channel, htim->Init.Period);
            lastSideChannel[1] = channel;
            break;

        case OFF:
            HAL_TIM_PWM_Stop(htim, channel);
            HAL_TIMEx_PWMN_Stop(htim, channel);
            __HAL_TIM_SET_COMPARE(htim, channel, 0); // За бажанням, вже вимкнено | Optional it is already OFF
            lastSideChannel[2] = channel;
            break;

    }
}

