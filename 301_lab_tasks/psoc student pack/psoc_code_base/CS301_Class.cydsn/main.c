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

volatile int v1L; // allocating storage for measure number of holes (ahead) - LEFT WHEEL
volatile int v2L; // allocating storage for measure number of holes (behind) - LEFT WHEEL
volatile int v1R; // allocating storage for measure number of holes (ahead) - RIGHT WHEEL
volatile int v2R; // allocating storage for measure number of holes (behind) - RIGHT WHEEL
volatile int startCountingL; //start counting for the 180 degree turn
volatile int totalNumRotationsL; //used for calculating when the 180 degree turn is finished
volatile int numRotationsL; //
volatile int numRotationsR; //

volatile int distanceToOne = 0;
volatile int distanceToTwo = 0;
volatile int distanceToThree = 0;
volatile int distanceToFour = 0;
volatile int distanceToFive = 0;

volatile int stopCheckingArray = 0;

volatile int i = 0;
volatile int hitFood = 0;
volatile int hitWall = 0;
volatile int turnComplete = 0;
unsigned char currentInstruction;

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

  unsigned char squares[500][2];
  unsigned char instruction[500];
  unsigned char distance[500];

  CyDelay(4000);
  usbPutString("Starting Pathfind\r\n");
  pathfind(instruction, distance, squares);

  usbPutString("SQUARES\r\n");
  for (int i = 0; i < 500; i++) {
    char squareStr[10];
    snprintf(squareStr, sizeof(squareStr), "%u %u \r\n", (unsigned char) squares[i][0], (unsigned char) squares[i][1]);
    usbPutString(squareStr);
  }

    int index = 0;
    while (instruction[index] == '0') {
        index++;
    }

    // Now 'index' holds the index of the first element in 'instruction' that is not '0'
    usbPutString("Index of the first non-zero element: ");
    char indexChar[10];
    snprintf(indexChar, sizeof(indexChar), "%d\r\n", index);
    usbPutString(indexChar);
    
  usbPutString("INSTRUCTIONS\r\n");
  // Print all instructions
  for (int i = 0; i < 500; i++) {
    char instStr[10];
    snprintf(instStr, sizeof(instStr), "%u \r\n", (unsigned char) instruction[i]);
    usbPutString(instStr);
  }

  usbPutString("DISTANCES\r\n");
  // Print all distances
  for (int i = 0; i < 500; i++) {
    char distanceStr[10]; // Assuming distances are integers and can fit in 10 characters
    snprintf(distanceStr, sizeof(distanceStr), "%u", (unsigned int) distance[i]);
    usbPutString(distanceStr);
    usbPutString("\r\n");
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

        int changeInDots = (((abs(numRotationsR) + abs(numRotationsL))) / 2);
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
          flag = 0;
          ADC_IRQ_Enable();

          //start code functions here

          if (distanceTravelled > MOVE_DISTANCE) {
            stop();
          } else {
            if (currentInstruction == '0') { //stop
              stop();
            } else if (currentInstruction == '1' && turnComplete == 0) { //leftTurn and then go forward until hit a wall
              if (R_INT_BLACK) { //code that senses when the turn is done, can be optimised
                turnComplete = 1;
              } else {
                turnLeft();
              }

              if (turnComplete == 1) {
                if ((highCountRightIntersection > 150 || highCountLeftIntersection > 150) && turnComplete == 1) { //if not reached an intersection
                  driveForward(distanceTravelled, 0);
                } else {
                  stop();
                  currentInstruction = instruction[i + 1];
                  turnComplete = 0;
                }
              }
            } else if (currentInstruction == '2') { //go straight
              if ((highCountRightIntersection > 150 || highCountLeftIntersection > 150) && turnComplete == 1) { //if not reached an intersection
                driveForward(distanceTravelled, 0);
              } else {
                currentInstruction = instruction[i + 1];
              }
            } else if (currentInstruction == '3' && turnComplete == 0) { //rightTurn and then go forward until hit a wall
              if (L_INT_BLACK) { //code that senses when the turn is done, can be optimised
                turnComplete = 1;
              } else {
                turnRight();
              }

              if (turnComplete == 1) {
                if ((highCountRightIntersection > 150 || highCountLeftIntersection > 150) && turnComplete == 1) { //if not reached an intersection
                  driveForward(distanceTravelled, 0);
                } else {
                  stop();
                  currentInstruction = instruction[i + 1];
                  turnComplete = 0;
                }
              }
            } else if (currentInstruction == '4' && turnComplete == 0) { //turn 180 degrees
              if (startCountingL == 1) {
                totalNumRotationsL = 0;
                startCountingL = 0;
              }

              if (totalNumRotationsL > 1500) { //test this to get it exact for the 180 degree turn
                stop();
                currentInstruction = instruction[i + 1];
                totalNumRotationsL = 0; //reset turn count counter
                startCountingL = 1;
                turnComplete = 1;
              } else {
                turnRight(); //turn left or right shouldn't matter
              }

              if (turnComplete == 1) { //travel forward until i reach an intersection
                if ((highCountRightIntersection > 150 || highCountLeftIntersection > 150) && turnComplete == 1) { //if not reached an intersection
                  driveForward(distanceTravelled, 0);
                } else {
                  stop();
                  currentInstruction = instruction[i + 1];
                  turnComplete = 0;
                }
              }

            } else if (currentInstruction == '5' && turnComplete == 0) { //leftTurn and then go forward until distance to food thing is covered
              if (R_INT_BLACK) { //code that senses when the turn is done, can be optimised
                turnComplete = 1;
                distanceTravelled = 0; //reset distance counter
              } else {
                turnLeft();
              }

              if (turnComplete == 1) {
                int targetDistance = 50;
                if ((distanceTravelled < targetDistance) && turnComplete == 1) { //if robot travels far enough, stop it and can 
                  driveForward(distanceTravelled, 0);
                } else {
                  stop();
                  currentInstruction = instruction[i + 1];
                  turnComplete = 0;
                }
              }
            } else if (currentInstruction == '6' && turnComplete == 0) { //go forward until distance to food thing is covered
              int targetDistance = 50;
              if ((distanceTravelled < targetDistance) && turnComplete == 1) { //if robot travels far enough, stop it and can 
                driveForward(distanceTravelled, 0);
              } else {
                stop();
                currentInstruction = instruction[i + 1];
                turnComplete = 0;
              }
            } else if (currentInstruction == '7' && turnComplete == 0) { //leftTurn and then go forward until distance to food thing is covered
              if (R_INT_BLACK) { //code that senses when the turn is done, can be optimised
                turnComplete = 1;
                distanceTravelled = 0; //reset distance counter
              } else {
                turnLeft();
              }

              if (turnComplete == 1) {
                int targetDistance = 50;
                if ((distanceTravelled < targetDistance) && turnComplete == 1) { //if robot travels far enough, stop it and can 
                  driveForward(distanceTravelled, 0);
                } else {
                  stop();
                  currentInstruction = instruction[i + 1];
                  turnComplete = 0;
                }
              }
            } else { //turn 180 degrees and travel a distance
              if (startCountingL == 1) {
                totalNumRotationsL = 0;
                startCountingL = 0;
              }

              if (totalNumRotationsL > 1500) { //test this to get it exact for the 180 degree turn
                stop();
                currentInstruction = instruction[i + 1];
                totalNumRotationsL = 0; //reset turn count counter
                startCountingL = 1;
                turnComplete = 1;
              } else {
                turnRight(); //turn left or right shouldn't matter
              }

              if (turnComplete == 1) {
                int targetDistance = 50;
                if ((distanceTravelled < targetDistance) && turnComplete == 1) { //if robot travels far enough, stop it and can 
                  driveForward(distanceTravelled, 0);
                } else {
                  stop();
                  currentInstruction = instruction[i + 1];
                  turnComplete = 0;
                }
              }
            }
          }
        }
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