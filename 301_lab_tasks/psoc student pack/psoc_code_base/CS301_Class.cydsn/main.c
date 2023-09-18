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
//* ========================================
void usbPutString(char *s);
void usbPutChar(char c);
//* ========================================

volatile int flag = 0;
volatile int valuesLeftIntersection[2000];
volatile int valuesLeftLine[2000];
volatile int valuesMiddleLine[2000];
volatile int valuesTurnComplete[2000];
volatile int valuesRightLine[2000];
volatile int valuesRightIntersection[2000];
volatile int count = 0;
volatile int turnCompleted = 1;


CY_ISR(eoc) {
    // Some Logic
    flag = 1;
}

int main()
{
    

// --------------------------------    
// ----- INITIALIZATIONS ----------
    CYGlobalIntEnable;

// ------USB SETUP ----------------    
  
    USBUART_Start(0,USBUART_5V_OPERATION);     

    ADC_Start();
    ADC_StartConvert();
    eoc_StartEx(eoc);
    
    initMotors();
    
    force_g_Write(0);

    stop();
    

    for(;;)
    {   
        if (flag == 1) {
            ADC_IRQ_Disable();
            valuesLeftIntersection[count] = ADC_GetResult16(0);
            valuesLeftLine[count] = ADC_GetResult16(1);
            valuesMiddleLine[count] = ADC_GetResult16(2);
            valuesTurnComplete[count] = ADC_GetResult16(3);
            valuesRightLine[count] = ADC_GetResult16(4);
            valuesRightIntersection[count] = ADC_GetResult16(5);
            
            count++;
            if (count == 3000){
                count = 0;
                int highCountLeftIntersection = 0;
                int highCountLeftLine = 0;
                int highCountMiddleLine = 0;
                int highCountTurnComplete = 0;
                int highCountRightLine = 0;
                int highCountRightIntersection = 0;
                for (int i = 0; i < 3000; i++){
                    
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
                    
                    if(valuesRightIntersection[i] > 3000) {
                        highCountRightIntersection++;
                    
                    }
                    
                }
                if (turnCompleted == 1) {
                    /*if (highCountLeftIntersection < 200) {
                        LED_PIN_1_Write(1);
                        LED_PIN_2_Write(0);
                        LED_PIN_3_Write(0);
                        LED_PIN_4_Write(0);
                        turnLeft();
                        turnCompleted = 0;
                    } else  if (highCountRightIntersection < 200) {
                        LED_PIN_1_Write(0);
                        LED_PIN_2_Write(1);
                        LED_PIN_3_Write(0);
                        LED_PIN_4_Write(0);
                        turnRight();
                        turnCompleted = 0;
                    } else */ if (highCountLeftLine < 200) {
                        LED_PIN_1_Write(0);
                        LED_PIN_2_Write(0);
                        LED_PIN_3_Write(1);
                        LED_PIN_4_Write(0);
                        adjustRight();
                    } else if (highCountRightLine < 200) {
                        LED_PIN_1_Write(0);
                        LED_PIN_2_Write(0);
                        LED_PIN_3_Write(0);
                        LED_PIN_4_Write(0);
                        adjustLeft();
                    } else if (highCountMiddleLine < 200) {
                        LED_PIN_1_Write(0);
                        LED_PIN_2_Write(0);
                        LED_PIN_3_Write(0);
                        LED_PIN_4_Write(0);
                        driveForward();
                    } else {
                        stop();
                    }
                } else {
                    LED_PIN_1_Write(1);
                    LED_PIN_2_Write(1);
                    LED_PIN_3_Write(1);
                    LED_PIN_4_Write(1);
                    if (highCountTurnComplete < 200) {
                        turnCompleted = 1;
                    }
                }
                
                
                
                
            }
            
            flag = 0;
            
            ADC_IRQ_Enable();
        }
        
        
        
       
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


/* [] END OF FILE */