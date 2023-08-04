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
void usbPutString(char * s);
void usbPutChar(char c);
void handle_usb();
//* ========================================

volatile int v1; 
volatile int v2;
volatile int step = 0;
volatile int interruptFlag = 0;
CY_ISR(MyISR) {
  interruptFlag = 1;
}

int main() {

  // --------------------------------    
  // ----- INITIALIZATIONS ----------
  CYGlobalIntEnable;

  // ------USB SETUP ----------------    
  #ifdef USE_USB
  USBUART_Start(0, USBUART_5V_OPERATION);
  #endif

  RF_BT_SELECT_Write(0);

  PWM_2_Start();
  PWM_2_WriteCompare(255); // the fastest the motor could go, can mess around with it 
  PWM_2_WritePeriod(255);

  isr_TS_StartEx(MyISR);
  Timer_TS_Start();

  QuadDec_M2_Start();

  usbPutString(displaystring);
  for (;;) {
    if (interruptFlag == 1) { // timer has counted 1s 
      isr_TS_Disable(); // disabling the interrupts
      if (step == 0) { //takes the first measurement, sets steps to 2, the next time the timer goes off, it will do the second measurement
        v1 = QuadDec_M2_GetCounter();
        step++;
      } else {
        step = 0;
        char result[24];
        char result1[24];
        char result2[24];
        v2 = QuadDec_M2_GetCounter();
        itoa((v2 - v1), result, 10);
        itoa(v2, result1, 10);
        itoa(v1, result2, 10);
        usbPutString(result);
        usbPutString(":");
        usbPutString(result1);
        usbPutString(":");
        usbPutString(result2);
        usbPutString("\r\n");
        QuadDec_M2_SetCounter(0);
      }
     
      interruptFlag = 0;
      isr_TS_Enable();
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
//* ========================================
void handle_usb() {
  // handles input at terminal, echos it back to the terminal
  // turn echo OFF, key emulation: only CR
  // entered string is made available in 'line' and 'flag_KB_string' is set

  static uint8 usbStarted = FALSE;
  static uint16 usbBufCount = 0;
  uint8 c;

  if (!usbStarted) {
    if (USBUART_GetConfiguration()) {
      USBUART_CDC_Init();
      usbStarted = TRUE;
    }
  } else {
    if (USBUART_DataIsReady() != 0) {
      c = USBUART_GetChar();

      if ((c == 13) || (c == 10)) {
        //                if (usbBufCount > 0)
        {
          entry[usbBufCount] = '\0';
          strcpy(line, entry);
          usbBufCount = 0;
          flag_KB_string = 1;
        }
      } else {
        if (((c == CHAR_BACKSP) || (c == CHAR_DEL)) && (usbBufCount > 0))
          usbBufCount--;
        else {
          if (usbBufCount > (BUF_SIZE - 2)) // one less else strtok triggers a crash
          {
            USBUART_PutChar('!');
          } else
            entry[usbBufCount++] = c;
        }
      }
    }
  }
}