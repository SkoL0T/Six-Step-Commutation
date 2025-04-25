/*
 * step.c
 *
 *  Created on: Feb 10, 2025
 *      Author: Yehor Borsa
 */


#define STEP_COUNT 6
#include "main.h"

#include <string.h>
#include <stdio.h>

#include "stm32g4xx_hal.h"    // Замість цього підключіть потрібний заголовок для вашого мікроконтролера | Replace with the appropriate header for your microcontroller

#include "SIXSTEPHeader/MAL_declaration.h"


extern TIM_HandleTypeDef htim1;
extern uint32_t GetMicros(void); // Функція, що повертає поточний час у мікросекундах | Function returning current time in microseconds
extern UART_HandleTypeDef huart2;


// Функція зчитування поточного стану датчиків Холла
// Function to read the current state of Hall sensors
static HallSensorPattern GetHallPattern(void) {
    HallSensorPattern pattern;
    pattern.h1 = HAL_GPIO_ReadPin(GPIOB, A__H1_Pin);
    pattern.h2 = HAL_GPIO_ReadPin(GPIOB, B__H2_Pin);
    pattern.h3 = HAL_GPIO_ReadPin(GPIOB, C__H3_Pin);
    return pattern;
}

// Допоміжна функція для порівняння двох шаблонів
// Helper function for comparing two templates
static inline int PatternsEqual(HallSensorPattern p1, HallSensorPattern p2) {
    return (p1.h1 == p2.h1 && p1.h2 == p2.h2 && p1.h3 == p2.h3);
}
//==========================================================================================
// Функція ідентифікації калібрувальної таблиці.
// Приймає масив з 3 вимірювань: [0] повинно бути {1,1,1}, [1] і [2] – наступні вимірювання.
// Повертає індекс знайденої таблиці або -1, якщо співпадіння не знайдено.
//==========================================================================================
// Function for identifying the calibration table.
// Takes an array of 3 measurements: [0] should be {1,1,1}, [1] and [2] – next measurements.
// Returns the index of the found table or -1 if no match is found.
//==========================================================================================


// Глобальна змінна для збереження затримки з попереднього кроку – повинна бути ініціалізована відповідним чином.
// Global variable to hold delay from previous step – it should be initialized appropriately.
uint32_t previousMeasuredTime = 0; // Приклад значення за замовчуванням у мікросекундах | Example default value in microseconds

void PerformDynamicDelay(MotorStep step,  PerformDuringStepFunction ToBePerformedDuringStep) {
    PID pid;
    PID_Init(&pid);


    const uint32_t MAX_DELAY_US = 4000;
    uint32_t start_time = GetMicros();
    uint32_t elapsed_time = 0;
    int sensorChanged = 0;


    // Зчитуємо початковий стан датчиків
    // Read the initial state of the sensors
    HallSensorPattern initial = GetHallPattern();

    // Визначаємо, чи всі датчики активні {1, 1, 1}
    // Check if all sensors are active {1, 1, 1}
    bool isAllOn = (initial.h1 == 1 && initial.h2 == 1 && initial.h3 == 1);

    // Обчислення зсуву та ефективного індексу
    // Calculate the shift and effective index
    int shift = isAllOn ? step : 0;
    int effectiveIndex = (step + (isAllOn ? 0 : 0)) % STEP_COUNT;

    // Визначаємо початковий індекс у незсунутої таблиці
    // Determine the initial index in the unshifted table
    int originalIndex = shift ? (effectiveIndex - shift + STEP_COUNT) % STEP_COUNT : effectiveIndex;

    // Визначення, чи це дубльований крок
    // Determine if this is a duplicate step
    bool isDuplicatedStep = (originalIndex == 2 || originalIndex == 5);

    // Динамічна затримка з урахуванням типу кроку
    // Dynamic delay considering the step type
    while ((elapsed_time = (GetMicros() - start_time)) < MAX_DELAY_US) {
        HallSensorPattern current = GetHallPattern();

        // Фіксація зміни стану
        if (!PatternsEqual(current, initial)) {
            sensorChanged = 1;
        }

        // Логіка виходу з циклу затримки
        if (!isDuplicatedStep && sensorChanged) {
            break;
        }
        if (isDuplicatedStep && elapsed_time >= previousMeasuredTime) {
            break;
        }
    }

    // Зберігаємо час затримки
    // Save the delay time
    previousMeasuredTime = elapsed_time;




    // ПІД-корекція
    // PID correction
    PIDcomputation(&htim1, &pid, elapsed_time, elapsed_time, &duty, lastSideChannel);


}
