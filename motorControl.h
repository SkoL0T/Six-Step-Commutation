/*
 * motorControl.h
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */

#ifndef INC_MOTORCONTROL_H_
#define INC_MOTORCONTROL_H_


#include "stm32g4xx_hal.h"  // Налаштуйте відповідно до вашої серії STM32 | Adjust according to your STM32 series

#include "SIXSTEPHeader/motorControl.h"
#include "SIXSTEPHeader/pid.h"
#include "SIXSTEPHeader/pwmstate.h"
#include "SIXSTEPHeader/step.h"

typedef void (*SwitchFunction)(TIM_HandleTypeDef *htim, uint32_t channel, ChannelState state);
typedef void (*PerformDuringStepFunction)(void);
void MAL_Run(SwitchFunction SwitchType, TIM_HandleTypeDef *htim, PerformDuringStepFunction ToBePerformedDuringStep);

#endif /* INC_MOTORCONTROL_H_ */
