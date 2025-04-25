/*
 * pwmstate.h
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */

#ifndef PWMSTATE_H
#define PWMSTATE_H

#include "stm32g4xx_hal.h"

typedef enum {
    HIGH_SIDE_ON,
    LOW_SIDE_ON,
    OFF,
} ChannelState;

extern ChannelState sequences[6][3];

extern uint32_t lastSideChannel[3];


void SetPWMState(TIM_HandleTypeDef *htim, uint32_t channel, ChannelState state);
void HardSwitching(TIM_HandleTypeDef *htim, uint32_t channel, ChannelState state);
void SoftSwitching(TIM_HandleTypeDef *htim, uint32_t channel, ChannelState state);

#endif // PWMSTATE_H
