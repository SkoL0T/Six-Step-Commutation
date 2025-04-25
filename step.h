/*
 * step.h
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */

#ifndef STEP_H
#define STEP_H

#include "main.h"

#include <stdint.h>
#include <stdbool.h>

#include "SIXSTEPHeader/MAL_declaration.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STEP_COUNT 6  // You should match this to your actual motor step table size

typedef struct {
    uint8_t h1;
    uint8_t h2;
    uint8_t h3;
} HallSensorPattern;


typedef enum {
    STEP_0 = 0,
    STEP_1,
    STEP_2,
    STEP_3,
    STEP_4,
    STEP_5
} MotorStep;

typedef void (*PerformDuringStepFunction)(void);
void PerformDynamicDelay(MotorStep step,  PerformDuringStepFunction ToBePerformedDuringStep);
extern uint32_t GetMicros(void);
extern uint32_t measured_time;

#ifdef __cplusplus
}
#endif

#endif // STEP_H

