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
#include "pathfind.h"

#include "vars.h"

#define ADC_COUNT 1000
#define L_INT_BLACK highCountLeftIntersection < 150
#define R_INT_BLACK highCountRightIntersection < 150
#define L_LINE_BLACK highCountLeftLine < 150
#define R_LINE_BLACK highCountRightLine < 150
#define M_LINE_BLACK highCountMiddleLine < 150
#define TC_BLACK highCountTurnComplete < 150
float TARGET_SPEED = 15; //speed in cm^-1
int restoring = 30;
int turningDirection = 0;
int turningCount = 30;
volatile int shouldUpdate = 1;
#define MOVE_DISTANCE 9999999 //cm
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
volatile int distancePerSecondAvg = 0;
volatile int prevTurnState = 0;
volatile int turnState = 0;
volatile int turnEnable = 0;
volatile int turnTimer = 0;
int lastAdjustDirection = 0;
  unsigned char squares[500][2];
  unsigned char instruction[500];
  unsigned char distance[500];

CY_ISR(eoc) {
  flag = 1;
}

CY_ISR(MyISR) {
  motorFlag = 1;
}

int main() {
  CYGlobalIntEnable;

  USBUART_Start(0, USBUART_5V_OPERATION);
  UART_Start();

  QuadDec_M1_Start();
  QuadDec_M2_Start();

  isr_TS_StartEx(MyISR);

  RF_BT_SELECT_Write(1);

  ADC1_Start();
  ADC1_StartConvert();
  eoc_StartEx(eoc);

  initMotors();

  stop();

  isr_TS_StartEx(MyISR);
  Timer_TS_Start();

    // Loop through the arrays and set each element to 55 or 0
    for (int i = 0; i < 500; i++) {
        squares[i][0] = 55;
        squares[i][1] = 55;
        instruction[i] = 0;
        distance[i] = 0;
        distance[i] = 55;
    }

  for (;;) {

    if (motorFlag == 1) { // timer has counted 0.1s 

      isr_TS_Disable(); // disabling the interrupts
      if (step == 0) { //takes the first measurement, sets steps to 2, the next time the timer goes off, it will do the second measurement
        v1L = QuadDec_M1_GetCounter(); // first measurement taken - LEFT
        v1R = QuadDec_M2_GetCounter(); // first measurement taken - RIGHT
        step++;
      } else { // second measure
        step = 0;

        v2L = QuadDec_M1_GetCounter(); // second measurement taken - LEFT
        v2R = QuadDec_M2_GetCounter(); // second measurement taken - RIGHT

        numRotationsL = (v2L - v1L);
        numRotationsR = (v2R - v1R);

        int changeInDots = (((abs(numRotationsR) + abs(numRotationsL)) ) / 2);
        dotsTravelled = changeInDots + dotsTravelled;
        distanceTravelled = (dotsTravelled * (float)(1.217375 / 6.105)); //increase 6.1 to go further, decrease to go shorter
                                                                         //6.2 for low, 6.1 for normal, 6 for full

        QuadDec_M1_SetCounter(0); // set quad counter to 0 to avoid overflow
        QuadDec_M2_SetCounter(0); // set quad counter to 0 to avoid overflow 

      }
      motorFlag = 0; // interrupt flag is back to 0
      isr_TS_Enable(); // interrupt enabled

    }

    if (distanceTravelled > MOVE_DISTANCE) {
      stop();

    } else {

      LED_PIN_4_Write(0);
      if (flag == 1) {

        ADC1_IRQ_Disable();
        valuesLeftIntersection[count] = ADC1_GetResult16(0);
        valuesLeftLine[count] = ADC1_GetResult16(1);
        valuesMiddleLine[count] = ADC1_GetResult16(2);
        valuesTurnComplete[count] = ADC1_GetResult16(3);
        valuesRightLine[count] = ADC1_GetResult16(4);
        valuesRightIntersection[count] = ADC1_GetResult16(5);

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
          turnTimer = 0;

          // If all the sensors are under black light, stop the robot
          if (L_INT_BLACK && M_LINE_BLACK && R_INT_BLACK && TC_BLACK && L_LINE_BLACK && R_LINE_BLACK) {

            stop();

          } else if (turningLeft == 1) { // if robot is turning left
            while (turnTimer != 10000) {
              turnLeft();
              turnTimer++;
            }
            turnEnable = 0;
            turningLeft = 0;

          } else if (turningRight == 1) { // if robot is turning right
            while (turnTimer != 10000) {
              turnRight();
              turnTimer++;
            }
            turnEnable = 0;
            turningRight = 0;

          } else if (L_INT_BLACK && (M_LINE_BLACK || L_LINE_BLACK) && turnEnable == 1) { //initiate the turn left 

            turnTimer++;
            turningLeft = 1;
            lastAdjustDirection = 0;

          } else if (R_INT_BLACK && (M_LINE_BLACK || R_LINE_BLACK) && turnEnable == 1) { //initiate the right turn

            turnTimer++;
            turningRight = 1;
            lastAdjustDirection = 2;

          } else if (R_LINE_BLACK && L_LINE_BLACK) {
            // do nothing for now

          } else if (L_INT_BLACK && R_INT_BLACK) {
            // do nothing for now

          } else if (M_LINE_BLACK && L_LINE_BLACK) { // if robot slightly too far right
            adjustLeft();
            turnEnable = 1;
            lastAdjustDirection = 0;

          } else if (M_LINE_BLACK && R_LINE_BLACK) { //  if robot slightly too far left
            adjustRight();
            lastAdjustDirection = 2;
            turnEnable = 1;

          } else if (M_LINE_BLACK) { // if robot in the center keep moving straight
            shouldUpdate = 1;
            lastAdjustDirection = 1;
            turnEnable = 1;
            driveForward(distanceTravelled, TARGET_SPEED);

          } else if (R_LINE_BLACK) { // if robot too far left
            adjustRight();
            turnEnable = 1;
            lastAdjustDirection = 2;

          } else if (L_LINE_BLACK) { // if robot too far right
            adjustLeft();
            turnEnable = 1;
            lastAdjustDirection = 0;

          } else if (L_INT_BLACK) {
            //hardAdjustLeft();
            turningLeft = 1;
            lastAdjustDirection = 0;

          } else if (R_INT_BLACK) {
            //hardAdjustRight();
            turningRight = 1;
            lastAdjustDirection = 2;
//          } else if (highCountLeftIntersection > 150 && highCountRightIntersection > 150 && highCountLeftLine > 150 && highCountRightLine > 150 && highCountMiddleLine > 150 && highCountTurnComplete > 150){
//            driveForward(distanceTravelled, TARGET_SPEED);
//            
//        }
        }
            else { // completely lost find way
            if (lastAdjustDirection == 0) {
              turnLeft();
            } else if (lastAdjustDirection == 1) { // last movement was forward (middle sensor sensed)
              //hardAdjustLeft();
              turnRight();
              //reverse(3000); // random number 3000
            } else {
              turnRight();
            }
            }
        }

        flag = 0;
        ADC1_IRQ_Enable();
      }
    }
  }
}

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