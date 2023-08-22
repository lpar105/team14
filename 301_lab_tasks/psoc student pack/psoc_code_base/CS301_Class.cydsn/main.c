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
#include "vars.h"
//* ========================================
void usbPutString(char *s);
void usbPutChar(char c);
//* ========================================

volatile int flag = 0;
volatile int result[100];
volatile int count = 0;


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
        
    //RF_BT_SELECT_Write(0);

    ADC_Start();
    ADC_StartConvert();
    eoc_StartEx(eoc);

    for(;;)
    {   
        
        if (flag == 1) {
            // goal: store 100 values into an array then display ALL together to solve frequency timing issues we hope
            ADC_IRQ_Disable();
            result[count] = ADC_GetResult8();
            
            //if (num < 0) {
            //    num = num +255;
            //}

            count++;
            if (count == 100){
                count = 0;
                int highCount = 0;
                for (int i = 0; i < 100; i++){
                    
                    if(result[i] > 10) {
                       
                        highCount++;
                    }
           
                //usbPutString(result[i]);
                //usbPutString("\r\n");
                /* Place your application code here. */
                
                
                
                }
                if (highCount > 10) {
                     LED_PIN_Write(1);
                } else {
                    LED_PIN_Write(0);
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
