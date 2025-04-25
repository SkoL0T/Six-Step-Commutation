/*
 * motorControl.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */

#include <string.h>
#include <stdio.h>

#include "SIXSTEPHeader/motorControl.h"
#include "SIXSTEPHeader/pid.h"
#include "SIXSTEPHeader/pwmstate.h"
#include "SIXSTEPHeader/step.h"


extern TIM_HandleTypeDef htim1;

extern uint8_t sequences[6][3];

void MAL_Run(SwitchFunction SwitchType, TIM_HandleTypeDef *htim, PerformDuringStepFunction ToBePerformedDuringStep) {

    uint32_t channels[] = {TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3};

    for (int step = 0; step < 6; step++) {
        // Встановимо стан каналів ШІМ для поточного кроку
    	// Set the PWM channels state for the current step
        for (int ch = 0; ch < 3; ch++) {
        	SwitchType(htim, channels[ch], sequences[step][ch]);
        }
        // Виконаємо динамічну затримку для поточного кроку
        // Perform dynamic delay for the current step
        PerformDynamicDelay(step, ToBePerformedDuringStep);
    }


}


