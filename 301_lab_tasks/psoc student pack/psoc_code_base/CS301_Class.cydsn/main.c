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
#define L_INT_BLACK highCountLeftIntersection < 75
#define R_INT_BLACK highCountRightIntersection < 75
#define L_LINE_BLACK highCountLeftLine < 75
#define R_LINE_BLACK highCountRightLine < 75
#define M_LINE_BLACK highCountMiddleLine < 75
#define TC_BLACK highCountTurnComplete < 75

int consecStops = 0;
int started = 0;
int instCounter = 0;
//* ========================================
void usbPutString(char * s);
void usbPutChar(char c);
//* ========================================

volatile int flag = 0;
volatile int valuesLeftIntersection[ADC_COUNT];
volatile int valuesLeftLine[ADC_COUNT];
volatile int valuesMiddleLine[ADC_COUNT];
volatile int valuesTurnComplete[ADC_COUNT];
volatile int valuesRightLine[ADC_COUNT];
volatile int valuesRightIntersection[ADC_COUNT];
volatile int count = 0;
volatile int motorUpdateCount = 0;
volatile int lastVeerDirection = 0;

volatile int v1M1;
volatile int v1M2;
volatile int v2M1;
volatile int v2M2;
volatile int turnComplete = -1;
int pulsesTravelled = 0;
volatile int checkDistance = 0;
volatile int step = 0;
volatile int interruptFlag = 0;

int lastAdjustDirection = 0;
unsigned char squares[500][2];
unsigned char instruction[500];
unsigned char distance[500];

CY_ISR(eoc) {
    flag = 1;
}

int main() {
    CYGlobalIntEnable;
    USBUART_Start(0, USBUART_5V_OPERATION);
    //UART_Start();

    QuadDec_M1_Start();
    QuadDec_M2_Start();

    //isr_TS_StartEx(MyISR);

    RF_BT_SELECT_Write(1);
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

    //CyDelay(2000);
    for (;;) {

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

                count = 0;
                motorUpdateCount++;
                int highCountLeftIntersection = 0;
                int highCountLeftLine = 0;
                int highCountMiddleLine = 0;
                int highCountTurnComplete = 0;
                int highCountRightLine = 0;
                int highCountRightIntersection = 0;
                for (int i = 0; i < ADC_COUNT; i++) {
                    step = 0;
                    char result1[24];
                    char result2[24];
                    if (motorUpdateCount == 1) {
                        if (turnComplete == -1 || turnComplete == 1) {
                        v2M1 = QuadDec_M1_GetCounter() * -1;
                        v2M2 = QuadDec_M2_GetCounter() * -1;
                        pulsesTravelled = pulsesTravelled + v2M1 + v2M2;
                        itoa((v2M1 - v1M1), result1, 10);
                        itoa((v2M2 - v1M2), result2, 10);
                        }
                        QuadDec_M1_SetCounter(0);
                        QuadDec_M2_SetCounter(0);
                        motorUpdateCount = 0;
                    }
                    if (valuesLeftIntersection[i] > 2000) {
                        highCountLeftIntersection++;

                    }

                    if (valuesLeftLine[i] > 2000) {
                        highCountLeftLine++;

                    }

                    if (valuesMiddleLine[i] > 2000) {
                        highCountMiddleLine++;

                    }

                    if (valuesTurnComplete[i] > 2000) {
                        highCountTurnComplete++;

                    }

                    if (valuesRightLine[i] > 2000) {
                        highCountRightLine++;

                    }

                    if (valuesRightIntersection[i] > 2000) {
                        highCountRightIntersection++;

                    }

                }

                if (started == 0) {
                    //LED_PIN_1_Write(!LED_PIN_1_Read());
                    // Look for start using a coarse grain approach, gradually get more fine
                    /*if (instruction[instCounter + 23] == 0 && instruction[instCounter + 24] == 0 && instruction[instCounter + 25] == 0 && instCounter + 25 < 500) {
                      instCounter = instCounter + 25;
                    } else if (instruction[instCounter + 8] == 0 && instruction[instCounter + 9] == 0 && instruction[instCounter + 10] == 0 && instCounter + 10 < 500) {
                      instCounter = instCounter + 10;
                    } else if (instruction[instCounter + 3] == 0 && instruction[instCounter + 4] == 0 && instruction[instCounter + 5] == 0 && instCounter + 5 < 500) {
                      instCounter = instCounter + 5;
                    } else {
                      if (instruction[instCounter + 1] == 0) {
                        instCounter++;
                      } else if (instruction[instCounter + 2] == 0) {
                        instCounter = instCounter + 2;
                      } else if (instruction[instCounter + 3] == 0) {
                        instCounter = instCounter + 3;
                      } else if (instruction[instCounter + 4] == 0) {
                        instCounter = instCounter + 4;
                      }
                    }*/
                    int i, firstNonZeroIndex = -1;
                    for (i = 0; i < 500; i++) {
                        if (instruction[i] != 0) {
                            firstNonZeroIndex = i;
                            break;
                        }
                    }

                    if (firstNonZeroIndex != -1) {
                        // Shift the array to start with the first non-zero element
                        for (i = 0; i < 500 - firstNonZeroIndex; i++) {
                            instruction[i] = instruction[i + firstNonZeroIndex];
                            distance[i] = distance[i + firstNonZeroIndex];
                        }

                        // Zero out the remaining elements
                        for (i = 500 - firstNonZeroIndex; i < 500; i++) {
                            instruction[i] = 0;
                            distance[i] = 0;
                        }

                    } else {
                        // The array contains all zeros

                    }
                    for (int i = 0; i < 500; i++) {

                        char instStr[24];

                        //snprintf(instStr, sizeof(instStr), "%u - %u \r\n", (unsigned char) instruction[i], (unsigned char) distance[i]);

                        //usbPutString(instStr);
                    }
                    started = 1;
                    turnComplete = -1; //let the robot travel forward first
                    checkDistance = 0; //robot checks/stops distance when this is on
                } else {

                    started = 1;
                    

                    char currentInst = instruction[instCounter];
                    char nextInst = instruction[instCounter + 1];
                    if (currentInst == 0) {
                        LED_PIN_1_Write(0);
                        LED_PIN_6_Write(0);
                        stop();
                        // Stop
                        CyDelay(1000);
                    } else if (currentInst == 1 && turnComplete == -1) {
                        LED_PIN_1_Write(1);
                        LED_PIN_6_Write(0);
                        turnComplete = 0;
                        turnLeft();
                        LED_PIN_4_Write(0);
                        CyDelay(150);
                        pulsesTravelled = 0;
                        // Complete a left turn, then follow line

                    } else if (currentInst == 1 && turnComplete == 0){
                        lastAdjustDirection = 0;
                        if (L_LINE_BLACK) { //code that senses when the turn is done, can be optimised
                            turnComplete = 1;
                            turnRight();
                            CyDelay(200);
                        }
                        pulsesTravelled = 0;
                    } else if (currentInst == 2) {
                        LED_PIN_1_Write(1);
                        LED_PIN_6_Write(1);
                        // Drive straight
                    } else if (currentInst == 3 && turnComplete == -1) { 
                        LED_PIN_1_Write(0);
                        LED_PIN_6_Write(1);
                        turnComplete = 0;
                        turnRight();
                        LED_PIN_4_Write(0);
                        CyDelay(150);
                        pulsesTravelled = 0;
                        
                    } else if (currentInst == 3 && turnComplete == 0){
                        lastAdjustDirection = 2;
                        if (R_LINE_BLACK) { //code that senses when the turn is done, can be optimised
                            turnComplete = 1;
                            turnLeft();
                            CyDelay(200);
                        }
                        pulsesTravelled = 0;
                    }  else if (currentInst == 4) {
                        LED_PIN_1_Write(0);
                        LED_PIN_6_Write(0);
                        // Do a 180, then follow line
                    } else if (currentInst == 5) {
                        LED_PIN_1_Write(1);
                        LED_PIN_6_Write(0);
                        // Complete a left turn, then follow line until end of encoder position
                        if (turnComplete == 0) { 
                            turnLeft();
                        }
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

                    //only allow code to straighten if turn is complete
                    if (turnComplete != 0) {
                    if ((M_LINE_BLACK && L_LINE_BLACK)) { // if robot slightly too far right
                        adjustLeft();
                        lastAdjustDirection = 0;

                    } else if (M_LINE_BLACK && R_LINE_BLACK) { //  if robot slightly too far left
                        adjustRight();
                        lastAdjustDirection = 2;

                    } else if (M_LINE_BLACK) { // if robot in the center keep moving straight
                        lastAdjustDirection = 1;
                        driveForward(0, 0);

                    } else if (R_LINE_BLACK) { // if robot too far left
                        adjustRight();
                        lastAdjustDirection = 2;

                    } else if (L_LINE_BLACK) { // if robot too far right
                        adjustLeft();
                        lastAdjustDirection = 0;
                    }
                    else {
                    }
                    

                    /*} else { // completely lost find way
                        if (lastAdjustDirection == 0) {
                            turnLeft();
                        } else if (lastAdjustDirection == 1) { // last movement was forward (middle sensor sensed)
                            //hardAdjustLeft();
                            turnRight();
                        } else {
                            turnRight();
                        }

                    }
                    */

                    // UPDATE INSTRUCTION LOGIC GOES HERE this needs dramatic improving
                    LED_PIN_2_Write(0);
                    LED_PIN_3_Write(0);
                    LED_PIN_4_Write(0);
                    char dist[24];
                    char pulses[24];
                    char first[24];
                    char second[24];
                    //itoa(distance[instCounter] * 9.13333, first, 10);
                    //itoa((pulsesTravelled), pulses, 10);
                    //itoa(pulsesTravelled * 20.42 / 57 / 2, second, 10);
                    //itoa(distance[instCounter], dist, 10);
                    /*usbPutString(dist);
                    usbPutString(" - ");
                    usbPutString(pulses);
                    usbPutString("       ");
                    usbPutString(first);
                    usbPutString(" <= ");
                    usbPutString(second);
                    usbPutString("\r\n");*/
                    //if (distance[instCounter] * 13.25 * 228 <= (pulsesTravelled) * 20.42 * 0.5) {
                    //57 pulses on 1 motor = 20.42 cm
                    char instStr[24];
                    char cVal[24];

                  //  itoa(instCounter, cVal, 10);
                  //  if ((distance[instCounter] * 9.13333 <= pulsesTravelled * 20.42 / 57 / 2) && (turnComplete == 1)) { //turn complete and distance has been reached */
                 //       if ((R_INT_BLACK || L_INT_BLACK)) { //reached an intersection
                 //           LED_PIN_4_Write(1);
                 //           instCounter++;
                 //           turnComplete = -1;
                  //          pulsesTravelled = 0;
                 //       }
//                        if (checkDistance == 1) {
//                            if (distance[instCounter] * 9.13333 <= pulsesTravelled * 20.42 / 57 / 2) {
//                                LED_PIN_4_Write(1);
//                                instCounter++;
//                                turnComplete = 0;
//                                pulsesTravelled = 0;
//                            }
//                        }
                        
                    //}
                    
                    //if distance has been covered and ready to switch
                    if (turnComplete != 0 &&(distance[instCounter] * 9.13333 <= pulsesTravelled * 20.42 / 57 / 2)) {
                        if (currentInst == 0) {
                            LED_PIN_4_Write(1);
                            instCounter++;
                            turnComplete = -1;
//                            //snprintf(instStr, sizeof(instStr), "%u - %u c:%s\r\n", (unsigned char) instruction[instCounter], (unsigned char) distance[instCounter], cVal);
//                            //usbPutString("INSTCHANGE - ");
//                            //usbPutString(instStr);
                            pulsesTravelled = 0;
//
                        } else if (R_INT_BLACK && (nextInst == 3 || nextInst == 2 || nextInst == 4 || nextInst == 7 || nextInst == 6 || nextInst == 8)) { //reached an intersection and turning right next time
//
                            LED_PIN_4_Write(1);
                            instCounter++;
                            turnComplete = -1;
//                            //snprintf(instStr, sizeof(instStr), "%u - %u c:%s\r\n", (unsigned char) instruction[instCounter], (unsigned char) distance[instCounter], cVal);
//                            //usbPutString("INSTCHANGE - ");
//                            //usbPutString(instStr);
                            pulsesTravelled = 0;
                        }
                        else if (L_INT_BLACK && (nextInst == 1 || nextInst == 2 || nextInst == 5|| nextInst == 4 ||nextInst == 6 ||nextInst == 8)) { //reached an intersection and turning left next time
                            LED_PIN_4_Write(1);
                            instCounter++;
                            turnComplete = -1;
//                            //snprintf(instStr, sizeof(instStr), "%u - %u c:%s\r\n", (unsigned char) instruction[instCounter], (unsigned char) distance[instCounter], cVal);
//                            //usbPutString("INSTCHANGE - ");
//                            //usbPutString(instStr);
                            pulsesTravelled = 0;
                        }
                        else {
                            
                        }
                    }
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