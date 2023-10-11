/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <project.h>

#include <stdio.h>

#include "adcc.h"

void updateHighCounts(int valuesLeftIntersection[1000], int valuesLeftLine[1000], int valuesMiddleLine[1000], int valuesTurnComplete[1000], int valuesRightLine[1000], int valuesRightIntersection[1000], int* highCountLeftIntersection,  int* highCountLeftLine,  int* highCountMiddleLine,  int* highCountTurnComplete,  int* highCountRightLine,  int* highCountRightIntersection,  int ADC_COUNT) {
    LED_PIN_6_Write(1);
    for (int i = 0; i < ADC_COUNT; i++) {
            
            if (valuesLeftIntersection[i] > 3000) {
              highCountLeftIntersection++;

            }

            if (valuesLeftLine[i] > 3000) {
              highCountLeftLine++;

            }

            if (valuesMiddleLine[i] > 3000) {
              highCountMiddleLine++;

            }

            if (valuesTurnComplete[i] > 3000) {
              highCountTurnComplete++;

            }

            if (valuesRightLine[i] > 3000) {
              highCountRightLine++;

            }

            if (valuesRightIntersection[i] > 3000) {
              highCountRightIntersection++;

            }
LED_PIN_6_Write(0);
          }
}

/* [] END OF FILE */
