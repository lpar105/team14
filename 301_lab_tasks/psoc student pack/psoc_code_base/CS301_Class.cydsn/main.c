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
int restoring = 30;
int turningDirection = 0;
int turningCount = 30;
volatile int shouldUpdate = 1;
#define MOVE_DISTANCE 999999999 //cm
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
volatile int turningLeft = 0;
volatile int turningRight = 0;
volatile int lastVeerDirection = 0;

volatile int motorFlag = 0;
volatile int step = 0; // are we on first measuring step or second step?
volatile int v1L; // allocating storage for measure number of holes (ahead) - LEFT WHEEL
volatile int v2L; // allocating storage for measure number of holes (behind) - LEFT WHEEL
volatile int v1R; // allocating storage for measure number of holes (ahead) - RIGHT WHEEL
volatile int v2R; // allocating storage for measure number of holes (behind) - RIGHT WHEEL
volatile int numRotationsL; //
volatile int numRotationsR; //
volatile int turnCount;
volatile int distanceTravelled = 0;
volatile int distancePerSecondL = 0;
volatile int distancePerSecondR = 0;
volatile int prevTurnState = 0;
volatile int turnState = 0;
volatile int turnEnable = 0;
volatile int turnTimer = 0;
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

  isr_TS_StartEx(MyISR);

  RF_BT_SELECT_Write(1);

  ADC_Start();
  ADC_StartConvert();
  eoc_StartEx(eoc);

  initMotors();

  stop();

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

            if (valuesRightIntersection[i] > 3000) {
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

          shouldUpdate = 0;

          if (L_INT_BLACK && M_LINE_BLACK && R_INT_BLACK && TC_BLACK && L_LINE_BLACK && R_LINE_BLACK) {
            // If all the sensors are under black light, stop the robot
            stop();
          } else if (turningLeft == 1) {
            turnLeft();
            if (L_LINE_BLACK){
              turningLeft = 0;
            } 
            turnEnable = 0;
          } else if (turningRight == 1) {
              turnRight();
              if (R_LINE_BLACK) {
                turningRight = 0;
              }
              turnEnable = 0;
          } else if (L_INT_BLACK && (M_LINE_BLACK || L_LINE_BLACK) && turnEnable == 1) { //initiate the turn left 
            
            turningLeft = 1;
            lastAdjustDirection = 0;

          } else if (R_INT_BLACK && (M_LINE_BLACK || R_LINE_BLACK) && turnEnable == 1) { //initiate the right turn
            lastAdjustDirection = 2;
            turningRight = 1;

          } else if (M_LINE_BLACK && L_LINE_BLACK) {
            adjustLeft();
            turnEnable = 1;
            lastAdjustDirection = 0;
            
          } else if (M_LINE_BLACK && R_LINE_BLACK) {
            adjustRight();
            lastAdjustDirection = 2;
            turnEnable = 1;
            
          } else if (M_LINE_BLACK) {
            shouldUpdate = 1;
            lastAdjustDirection = 1;
            turnEnable = 1;
            driveForward(distancePerSecondL, distancePerSecondR, TARGET_SPEED);
            
          } else if (R_LINE_BLACK) {
            adjustRight();
            turnEnable = 1;
            lastAdjustDirection = 2;
            
          } else if (L_LINE_BLACK) {
            adjustLeft();
            turnEnable = 1;
            lastAdjustDirection = 0;
            
          } else if (L_INT_BLACK){
            hardAdjustLeft();
            
          } else if (R_INT_BLACK){
            hardAdjustRight();
            
        } 
        
        else { // completely lost find way
            if (lastAdjustDirection == 0) {
              restoreLeft();
            } else if (lastAdjustDirection == 1) {
              reverse();
            } else {
              restoreRight();
            }
          }

          }

        flag = 0;
        ADC_IRQ_Enable();
    }
     }
          
      
//  }
//
//}
//// }
////* ========================================
//void usbPutString(char * s) {
//  // !! Assumes that *s is a string with allocated space >=64 chars     
//  //  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  //  length to 62 char (63rd char is a '!')
//
//  #ifdef USE_USB
//  while (USBUART_CDCIsReady() == 0);
//  s[63] = '\0';
//  s[62] = '!';
//  USBUART_PutData((uint8 * ) s, strlen(s));
//  #endif
//}
////* ========================================
//void usbPutChar(char c) {
//  #ifdef USE_USB
//  while (USBUART_CDCIsReady() == 0);
//  USBUART_PutChar(c);
//  #endif
//}
////* ========================================
//
//void printSensorDebug(int highCountLeftIntersection, int highCountLeftLine, int highCountMiddleLine, int highCountRightLine, int highCountRightIntersection, int highCountTurnComplete) {
//  char leftLineText[11];
//  char rightIntText[11];
//  char leftIntText[11];
//  char rightLineText[11];
//  char turnCompleteText[11];
//  char middleLineText[11];
//
//  if (L_INT_BLACK) {
//    //LED_PIN_2_Write(0);
//    strcpy(leftIntText, "S1 - 0\r\n");
//  } else {
//    //LED_PIN_2_Write(1);
//    strcpy(leftIntText, "S1 - 1\r\n");
//  }
//
//  if (R_INT_BLACK) {
//    strcpy(rightIntText, "S5 - 0\r\n");
//  } else {
//    strcpy(rightIntText, "S5 - 1\r\n");
//  }
//
//  if (L_LINE_BLACK) {
//    strcpy(leftLineText, "S2 - 0\r\n");
//  } else {
//    strcpy(leftLineText, "S2 - 1\r\n");
//  }
//
//  if (R_LINE_BLACK) {
//    strcpy(rightLineText, "S4 - 0\r\n");
//  } else {
//    strcpy(rightLineText, "S4 - 1\r\n");
//  }
//
//  if (TC_BLACK) {
//    strcpy(turnCompleteText, "test");
//  } else {
//    strcpy(turnCompleteText, "test");
//  }
//
//  if (M_LINE_BLACK) {
//    strcpy(middleLineText, "S3 - 0\r\n");
//  } else {
//    strcpy(middleLineText, "S3 - 1\r\n");
//  }
//
//  /*usbPutString(leftIntText);
//  usbPutString(leftLineText);
//  usbPutString(middleLineText);
//  usbPutString(rightLineText);
//  usbPutString(rightIntText);*/

  //usbPutString(turnCompleteText);

  //usbPutString("-------------\r\n");
}
}

/* [] END OF FILE */