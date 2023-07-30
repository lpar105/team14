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
char result[RESULT_BUFFER_SIZE];


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
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
    isr_1_StartEx(eoc);
    
    for(;;)
    {   
        
        if (flag == 1) {
            
            ADC_SAR_1_IRQ_Disable();
            result = itoa(ADC_SAR_1_GetResult8 , result, 10);
            usbPutString(result);
            usbPutString("\r\n");
            /* Place your application code here. */
            if (flag_KB_string == 1)
            {
                usbPutString(line);
                flag_KB_string = 0;
                
            }
            flag = 0;
            
            ADC_SAR_1_IRQ_Enable();
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
