/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
 */
#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <project.h>
 //* ========================================

#include "defines.h"

#include "motors.h"

#include "vars.h"

#define ADC_COUNT 1000
#define L_INT_BLACK highCountLeftIntersection < 150
#define R_INT_BLACK highCountRightIntersection < 150
#define L_LINE_BLACK highCountLeftLine < 150
#define R_LINE_BLACK highCountRightLine < 150
#define M_LINE_BLACK highCountMiddleLine < 150
#define TC_BLACK highCountTurnComplete < 150
int TARGET_SPEED = 10;
int turningDirection = 0;
int turningCount = 30;
volatile int shouldUpdate = 1;
#define MOVE_DISTANCE 9999976 //cm
volatile int dotsTravelled = 0;
//* ========================================
void usbPutString(char * s);
void usbPutChar(char c);
void printSensorDebug(int highCountLeftIntersection, int highCountLeftLine, int highCountMiddleLine, int highCountRightLine, int highCountRightIntersection, int highCountTurnComplete);
//* ========================================

volatile int flag = 0;
volatile int valuesLeftIntersection[ADC_COUNT];
volatile int valuesLeftLine[ADC_COUNT];
volatile int valuesMiddleLine[ADC_COUNT];
volatile int valuesTurnComplete[ADC_COUNT];
volatile int valuesRightLine[ADC_COUNT];
volatile int valuesRightIntersection[ADC_COUNT];
volatile int count = 0;
volatile int lastVeerDirection = 0;

volatile int motorFlag = 0;
volatile int step = 0; // are we on first measuring step or second step?
volatile int v1L; // allocating storage for measure number of holes (ahead) - LEFT WHEEL
volatile int v2L; // allocating storage for measure number of holes (behind) - LEFT WHEEL
volatile int v1R; // allocating storage for measure number of holes (ahead) - RIGHT WHEEL
volatile int v2R; // allocating storage for measure number of holes (behind) - RIGHT WHEEL
volatile int numRotationsL; //
volatile int numRotationsR; //
volatile int distanceTravelled = 0;
volatile int distancePerSecondL = 0;
volatile int distancePerSecondR = 0;
int lastAdjustDirection = 0;

CY_ISR(eoc) {
  flag = 1;
}

CY_ISR(MyISR) {
  motorFlag = 1;
}

int main() {

  // --------------------------------    
  // ----- INITIALIZATIONS ----------
  CYGlobalIntEnable;

  // ------USB SETUP ----------------    

  USBUART_Start(0, USBUART_5V_OPERATION);
  UART_Start();

  QuadDec_M1_Start();
  QuadDec_M2_Start();

  RF_BT_SELECT_Write(1);

  ADC_Start();
  ADC_StartConvert();
  eoc_StartEx(eoc);

  initMotors();

  stop();

  // NOTE FOR GROUP -- I've changed the timer to go off every 500ms. Ideally we want to lower it even further
  isr_TS_StartEx(MyISR);
  Timer_TS_Start();

  for (;;) {

    if (distanceTravelled > MOVE_DISTANCE) {
      stop();
    } else {
      LED_PIN_4_Write(0);
      if (flag == 1) {

        ADC_IRQ_Disable();
        valuesLeftIntersection[count] = ADC_GetResult16(0);
        valuesLeftLine[count] = ADC_GetResult16(1);
        valuesMiddleLine[count] = ADC_GetResult16(2);
        valuesTurnComplete[count] = ADC_GetResult16(3);
        valuesRightLine[count] = ADC_GetResult16(4);
        valuesRightIntersection[count] = ADC_GetResult16(5);

        count++;
        if (count == ADC_COUNT) {
          LED_PIN_4_Write(1);
          count = 0;
          int highCountLeftIntersection = 0;
          int highCountLeftLine = 0;
          int highCountMiddleLine = 0;
          int highCountTurnComplete = 0;
          int highCountRightLine = 0;
          int highCountRightIntersection = 0;
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

            if (valuesRightIntersection[i] >= 3000) {
              highCountRightIntersection++;

            }

          }

          if (L_LINE_BLACK) {
            LED_PIN_1_Write(0);
          } else {
            LED_PIN_1_Write(1);
          }

          if (M_LINE_BLACK) {
            LED_PIN_2_Write(0);
          } else {
            LED_PIN_2_Write(1);
          }

          if (R_LINE_BLACK) {
            LED_PIN_3_Write(0);
          } else {
            LED_PIN_3_Write(1);
          }
        
        if (turningCount > 7 && turningCount <= 10) {
            turningCount++;
            driveForward(distancePerSecondL, distancePerSecondR, TARGET_SPEED);
        } else if (turningCount > 10) {
            if (L_LINE_BLACK && L_INT_BLACK && M_LINE_BLACK) {
              turningCount = 0;
              turningDirection = 0;
              turnLeft();
            } else if (R_LINE_BLACK && R_INT_BLACK && M_LINE_BLACK) {
                turningDirection = 1;
              turnRight();
              turningCount = 0;
            } else if (L_LINE_BLACK) {
              shouldUpdate = 0;
              adjustLeft();
              lastAdjustDirection = 0;
            } else if (R_LINE_BLACK) {
              shouldUpdate = 0;
              lastAdjustDirection = 2;
              adjustRight();
            } else if (M_LINE_BLACK) {
              shouldUpdate = 1;
              lastAdjustDirection = 1;

              driveForward(distancePerSecondL, distancePerSecondR, TARGET_SPEED);
            } else {
              shouldUpdate = 0;
              if (lastAdjustDirection == 0) {
                restoreLeft();
              } else if (lastAdjustDirection == 1) {
                stop();
              } else {
                restoreRight();
              }
            }
          } else {
            turningCount++;
          }

          printSensorDebug(highCountLeftIntersection, highCountLeftLine, highCountMiddleLine, highCountRightLine, highCountRightIntersection, highCountTurnComplete);

        }

        flag = 0;
        ADC_IRQ_Enable();
      }
      //    
      if (motorFlag == 1) { // timer has counted 1s 

        //        char distanceTravelledChar[8];
        //        itoa(distanceTravelled, distanceTravelledChar,10);
        //        usbPutString(distanceTravelledChar);
        //        usbPutString("\r\n");

        isr_TS_Disable(); // disabling the interrupts
        if (step == 0) { //takes the first measurement, sets steps to 2, the next time the timer goes off, it will do the second measurement
          v1L = QuadDec_M1_GetCounter(); // first measurement taken - LEFT
          v1R = QuadDec_M2_GetCounter(); // first measurement taken - RIGHT
          step++;
          //LED_PIN_4_Write(1);
        } else { // second measure and sending out to UART
          step = 0;
          //LED_PIN_4_Write(0);
          // Allocating space for debugging and speed values
          char numRotationsLText[24];
          char numRotationsRText[24];

          v2L = QuadDec_M1_GetCounter(); // second measurement taken - LEFT
          v2R = QuadDec_M2_GetCounter(); // second measurement taken - RIGHT

          numRotationsL = (v2L - v1L);
          numRotationsR = (v2R - v1R);

          itoa(v2L, numRotationsLText, 10);
          itoa(v2L, numRotationsRText, 10);

          dotsTravelled = abs(numRotationsL) + abs(numRotationsR) + dotsTravelled;

          distanceTravelled = dotsTravelled / 2 * 0.358;
          distancePerSecondL = (abs(numRotationsL) * 5) * 0.358 / 3; // averaged the shafts because timer is currently 00ms
          distancePerSecondR = (abs(numRotationsR) * 5) * 0.358 / 3; // averaged the shafts because timer is currently 200ms

          // integer to string conversion, base 10, storing, hardwaretask2 sheet for more info
          // converting to rad/s (v2-v1/360)*(360/16) for the left wheel

          char distancePerSecondLChar[8];
          char distancePerSecondRChar[8];
          itoa(distancePerSecondR, distancePerSecondRChar, 10);
          itoa(distancePerSecondL, distancePerSecondLChar, 10);

          char leftPWMChar[8];
          char rightPWMChar[8];
          itoa(getLeftPWM(), leftPWMChar, 10);
          itoa(getRightPWM(), rightPWMChar, 10);

          /*usbPutString(distancePerSecondLChar);
          usbPutString("  <LeftCount  RightCount> ");
          usbPutString(distancePerSecondRChar);
          usbPutString("\r\n");
          usbPutString(leftPWMChar);
          usbPutString("  <LeftPWM  Right PWM> ");
          usbPutString(rightPWMChar);
          usbPutString("\r\n\r\n");*/

          QuadDec_M1_SetCounter(0); // set quad counter to 0 to avoid overflow
          QuadDec_M2_SetCounter(0); // set quad counter to 0 to avoid overflow 
          if (shouldUpdate) {
            updateForwardSpeed(distancePerSecondL, distancePerSecondR, TARGET_SPEED);
          }

        }
        motorFlag = 0; // interrupt flag is back to 0
        isr_TS_Enable(); // interrupt enabled

      }
    }
  }

}
// }
//* ========================================
void usbPutString(char * s) {
  // !! Assumes that *s is a string with allocated space >=64 chars     
  //  Since USB implementation retricts data packets to 64 chars, this function truncates the
  //  length to 62 char (63rd char is a '!')

  #ifdef USE_USB
  while (USBUART_CDCIsReady() == 0);
  s[63] = '\0';
  s[62] = '!';
  USBUART_PutData((uint8 * ) s, strlen(s));
  #endif
}
//* ========================================
void usbPutChar(char c) {
  #ifdef USE_USB
  while (USBUART_CDCIsReady() == 0);
  USBUART_PutChar(c);
  #endif
}
//* ========================================

void printSensorDebug(int highCountLeftIntersection, int highCountLeftLine, int highCountMiddleLine, int highCountRightLine, int highCountRightIntersection, int highCountTurnComplete) {
  char leftLineText[11];
  char rightIntText[11];
  char leftIntText[11];
  char rightLineText[11];
  char turnCompleteText[11];
  char middleLineText[11];

  if (L_INT_BLACK) {
    //LED_PIN_2_Write(0);
    strcpy(leftIntText, "S1 - 0\r\n");
  } else {
    //LED_PIN_2_Write(1);
    strcpy(leftIntText, "S1 - 1\r\n");
  }

  if (R_INT_BLACK) {
    strcpy(rightIntText, "S5 - 0\r\n");
  } else {
    strcpy(rightIntText, "S5 - 1\r\n");
  }

  if (L_LINE_BLACK) {
    strcpy(leftLineText, "S2 - 0\r\n");
  } else {
    strcpy(leftLineText, "S2 - 1\r\n");
  }

  if (R_LINE_BLACK) {
    strcpy(rightLineText, "S4 - 0\r\n");
  } else {
    strcpy(rightLineText, "S4 - 1\r\n");
  }

  if (TC_BLACK) {
    strcpy(turnCompleteText, "test");
  } else {
    strcpy(turnCompleteText, "test");
  }

  if (M_LINE_BLACK) {
    strcpy(middleLineText, "S3 - 0\r\n");
  } else {
    strcpy(middleLineText, "S3 - 1\r\n");
  }

  /*usbPutString(leftIntText);
  usbPutString(leftLineText);
  usbPutString(middleLineText);
  usbPutString(rightLineText);
  usbPutString(rightIntText);*/

  //usbPutString(turnCompleteText);

  //usbPutString("-------------\r\n");
}

/* [] END OF FILE */