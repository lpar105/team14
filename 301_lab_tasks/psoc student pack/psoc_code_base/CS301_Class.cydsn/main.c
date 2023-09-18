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

#define ADC_COUNT 600
#define L_INT_BLACK highCountLeftIntersection < 100
#define R_INT_BLACK highCountRightIntersection < 100
#define L_LINE_BLACK highCountLeftLine < 100
#define R_LINE_BLACK highCountRightLine < 100
#define M_LINE_BLACK highCountMiddleLine < 100
#define TC_BLACK highCountTurnComplete < 100
//* ========================================
void usbPutString(char *s);
void usbPutChar(char c);
void printSensorDebug(int highCountLeftIntersection, int highCountLeftLine, int highCountMiddleLine,int highCountRightLine, int highCountRightIntersection, int highCountTurnComplete);
//* ========================================

volatile int flag = 0;
volatile int valuesLeftIntersection[ADC_COUNT];
volatile int valuesLeftLine[ADC_COUNT];
volatile int valuesMiddleLine[ADC_COUNT];
volatile int valuesTurnComplete[ADC_COUNT];
volatile int valuesRightLine[ADC_COUNT];
volatile int valuesRightIntersection[ADC_COUNT];
volatile int count = 0;
volatile int turnCompleted = 1;


CY_ISR(eoc) {
    flag = 1;
}

int main()
{
    

// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;

// ------USB SETUP ----------------    
  
    USBUART_Start(0,USBUART_5V_OPERATION); 
    
    RF_BT_SELECT_Write(0);

    ADC_Start();
    ADC_StartConvert();
    eoc_StartEx(eoc);
    
    initMotors();

    stop();
    

    for(;;)
    {   
        adjustLeft();
        /*LED_PIN_3_Write(0);
        if (flag == 1) {
            
            ADC_IRQ_Disable();
            valuesLeftIntersection[count] = ADC_GetResult16(0);
            valuesLeftLine[count] = ADC_GetResult16(1);
            valuesMiddleLine[count] = ADC_GetResult16(2);
            valuesTurnComplete[count] = ADC_GetResult16(3);
            valuesRightLine[count] = ADC_GetResult16(4);
            valuesRightIntersection[count] = ADC_GetResult16(5);
            
            count++;
            if (count == ADC_COUNT){
                LED_PIN_3_Write(1);
                count = 0;
                int highCountLeftIntersection = 0;
                int highCountLeftLine = 0;
                int highCountMiddleLine = 0;
                int highCountTurnComplete = 0;
                int highCountRightLine = 0;
                int highCountRightIntersection = 0;
                for (int i = 0; i < ADC_COUNT; i++){
                    
                    if(valuesLeftIntersection[i] > 3000) {
                        highCountLeftIntersection++;
                    
                    }
                    
                    if(valuesLeftLine[i] > 3000) {
                        highCountLeftLine++;
                    
                    }
                    
                    if(valuesMiddleLine[i] > 3000) {
                        highCountMiddleLine++;
                    
                    }
                    
                    if(valuesTurnComplete[i] > 3000) {
                        highCountTurnComplete++;
                    
                    }
                    
                    if(valuesRightLine[i] > 3000) {
                        highCountRightLine++;
                    
                    }
                    
                    if(valuesRightIntersection[i] >= 3000) {
                        highCountRightIntersection++;
                    
                    }
                    
                }
                if (turnCompleted == 1) {
                     if (L_LINE_BLACK) {
                        adjustRight();
                    } else if (R_LINE_BLACK) {
                        adjustLeft();
                    } else if (M_LINE_BLACK) {
                        driveForward();
                    } else {
                        stop();
                    }
                } else {
                    if (highCountTurnComplete < 200) {
                        turnCompleted = 1;
                    }
                }
                
                if (R_LINE_BLACK) {
                    LED_PIN_2_Write(0);
                } else {
                    LED_PIN_2_Write(1);
                }
                
                //printSensorDebug(highCountLeftIntersection, highCountLeftLine, highCountMiddleLine,highCountRightLine,highCountRightIntersection,highCountTurnComplete);
                
            }
            
            flag = 0;
            ADC_IRQ_Enable();
        }
        
        
        */ 
       
    } 
}
//* ========================================
void usbPutString(char *s)
{
// !! Assumes that *s is a string with allocated space >=64 chars     
//  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  length to 62 char (63rd char is a '!')

#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    s[63]='\0';
    s[62]='!';
    USBUART_PutData((uint8*)s,strlen(s));
#endif
}
//* ========================================
void usbPutChar(char c)
{
#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);
#endif    
}
//* ========================================

void printSensorDebug(int highCountLeftIntersection, int highCountLeftLine, int highCountMiddleLine,int highCountRightLine, int highCountRightIntersection, int highCountTurnComplete) {
            char leftLineText[11]; 
            char rightIntText[11]; 
            char leftIntText[11]; 
            char rightLineText[11]; 
            char turnCompleteText[11]; 
            char middleLineText[11]; 
                
            if (L_INT_BLACK) {
                //LED_PIN_2_Write(0);
                strcpy(leftLineText, "S1 - BLACK\n");
            } else {
                //LED_PIN_2_Write(1);
                strcpy(leftLineText, "S1 - WHITE\n");
            }
            
            if (R_INT_BLACK) {
                LED_PIN_2_Write(0);
                strcpy(leftLineText, "S5 - BLACK\n");
            } else {
                LED_PIN_2_Write(1);
                strcpy(leftLineText, "S5 - WHITE\n");
            }
            
            if (L_LINE_BLACK) {
                strcpy(leftLineText, "S2 - BLACK\n");
            } else {
                strcpy(leftLineText, "S2 - WHITE\n");
            }
            
            if (R_LINE_BLACK) {
                strcpy(leftLineText, "S4 - BLACK\n");
            } else {
                strcpy(leftLineText, "S4 - WHITE\n");
            }
            
            if (TC_BLACK) {
                strcpy(leftLineText, "S6 - BLACK\n");
            } else {
                strcpy(leftLineText, "S6 - WHITE\n");
            }
            
            if (M_LINE_BLACK) {
                strcpy(leftLineText, "S3 - BLACK\n");
            } else {
                strcpy(leftLineText, "S3 - WHITE\n");
            }
            
            
            usbPutString(leftIntText);
            usbPutString(rightIntText);
            usbPutString(leftLineText);
            usbPutString(rightLineText);
            usbPutString(turnCompleteText);
            usbPutString(middleLineText);
            usbPutString("-------------");
}

/* [] END OF FILE */