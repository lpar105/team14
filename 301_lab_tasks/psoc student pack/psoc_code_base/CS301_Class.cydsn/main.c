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
int consecStops = 0;
int started = 0;
int instCounter = 0;
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

int main() {
  CYGlobalIntEnable;
  //USBUART_Start(0, USBUART_5V_OPERATION);
  //UART_Start();

  //QuadDec_M1_Start();
  //QuadDec_M2_Start();

  //isr_TS_StartEx(MyISR);

  //RF_BT_SELECT_Write(1);
    //ADC1_IRQ_Enable();
  ADC1_Start();

  ADC1_StartConvert();
  
  eoc_StartEx(eoc);

  initMotors();

  stop();
  //isr_TS_StartEx(MyISR);
  //Timer_TS_Start();
    

    // Loop through the arrays and set each element to 55 or 0
    for (int i = 0; i < 500; i++) {
        squares[i][0] = 55;
        squares[i][1] = 55;
        instruction[i] = 0;
        distance[i] = 0;
        distance[i] = 55;
    }
    pathfind(instruction, distance, squares);

  for (;;) {
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

         
          shouldUpdate = 0;
          turnTimer = 0;
        if (instruction[instCounter] == 0 && (started == 0 || consecStops >= 6)) {
            //LED_PIN_1_Write(!LED_PIN_1_Read());
            // Look for start using a coarse grain approach, gradually get more fine
            if(instruction[instCounter+248] == 0 && instruction[instCounter+249] == 0&& instruction[instCounter+250] == 0){
                instCounter = instCounter+250;
            } else if(instruction[instCounter+98] == 0 && instruction[instCounter+99] == 0&& instruction[instCounter+100] == 0){
                instCounter = instCounter+100;
            } else if(instruction[instCounter+48] == 0 && instruction[instCounter+49] == 0&& instruction[instCounter+50] == 0){
                instCounter = instCounter+50;
            } else if(instruction[instCounter+23] == 0 && instruction[instCounter+24] == 0&& instruction[instCounter+25] == 0){
                instCounter = instCounter+25;
            } else if(instruction[instCounter+8] == 0 && instruction[instCounter+9] == 0&& instruction[instCounter+10] == 0){
                instCounter = instCounter+10;
            } else if (instruction[instCounter+3] == 0 && instruction[instCounter+4] == 0&& instruction[instCounter+5] == 0){
                instCounter = instCounter+5;
            } else {
                if(instruction[instCounter + 1] == 0) {
                    instCounter++; 
                } else if(instruction[instCounter + 2] == 0) {
                    instCounter = instCounter +2;
                } else if(instruction[instCounter + 3] == 0) {
                    instCounter = instCounter +3;
                } else if(instruction[instCounter + 4] == 0) {
                    instCounter = instCounter +4;
                } 
            }

        } else {
            turnTimer = 0;
            started = 1;
            
            char currentInst = instruction[instCounter];
            if (currentInst == 0) {
                LED_PIN_1_Write(0);
                LED_PIN_6_Write(0);
                // Stop
                
            } else if (currentInst == 1) {
                LED_PIN_1_Write(1);
                LED_PIN_6_Write(0);
                // Complete a left turn, then follow line
               
            } else if (currentInst == 2) {
                LED_PIN_1_Write(1);
                LED_PIN_6_Write(1);
                // Drive straight
            } else if (currentInst == 3) {
                LED_PIN_1_Write(0);
                LED_PIN_6_Write(1);
                // Complete a right turn then follow line
                
            } else if (currentInst == 4) {
                LED_PIN_1_Write(0);
                LED_PIN_6_Write(0);
                // Do a 180, then follow line
            } else if (currentInst == 5) {
                LED_PIN_1_Write(1);
                LED_PIN_6_Write(0);
                // Complete a left turn, then follow line until end of encoder position
            } else if (currentInst == 6) {
                LED_PIN_1_Write(1);
                LED_PIN_6_Write(1);
                // Follow line until end of encoder position
            } else if (currentInst == 7) {
                LED_PIN_1_Write(0);
                LED_PIN_6_Write(1);
                // Complete a right turn, then follow line until end of encoder position
            } else if (currentInst == 8) {
                LED_PIN_1_Write(0);
                LED_PIN_6_Write(0);
                // Complete a 180 turn, then follow line until end of encoder position
            }
            
            
                if (M_LINE_BLACK && L_LINE_BLACK) { // if robot slightly too far right
            adjustLeft();
            
            lastAdjustDirection = 0;

          } else if (M_LINE_BLACK && R_LINE_BLACK) { //  if robot slightly too far left
            adjustRight();
            lastAdjustDirection = 2;
            

          } else if (M_LINE_BLACK) { // if robot in the center keep moving straight
            
            lastAdjustDirection = 1;
            
            driveForward(distanceTravelled, TARGET_SPEED);

          } else if (R_LINE_BLACK) { // if robot too far left
            adjustRight();
            
            lastAdjustDirection = 2;

          } else if (L_LINE_BLACK) { // if robot too far right
            adjustLeft();
            
            lastAdjustDirection = 0;

          } else { // completely lost find way
            if (lastAdjustDirection == 0) {
              turnLeft();
            } else if (lastAdjustDirection == 1) { // last movement was forward (middle sensor sensed)
              //hardAdjustLeft();
              turnRight();
            } else {
              turnRight();
            }
            
        }
            
           
            // UPDATE INSTRUCTION LOGIC GOES HERE this needs dramatic improving
            LED_PIN_2_Write(0);
            LED_PIN_3_Write(0);
            if (L_INT_BLACK || R_INT_BLACK) {
                instCounter ++;
            }
            if (L_INT_BLACK) {
                LED_PIN_2_Write(1);
            }
        
            if (R_INT_BLACK) {
                LED_PIN_3_Write(1);
            }
            
            
            
            /*if (instruction[instCounter] == 0) {
                consecStops++;
            } else {
                consecStops = 0;
            }*/
        }
        }

        flag = 0;
        ADC1_IRQ_Enable();
      }
    
  }
}