
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

volatile int v1L; // allocating storage for measure number of holes (ahead) - LEFT WHEEL
volatile int v2L; // allocating storage for measure number of holes (behind) - LEFT WHEEL

volatile int v1R; // allocating storage for measure number of holes (ahead) - RIGHT WHEEL
volatile int v2R; // allocating storage for measure number of holes (behind) - RIGHT WHEEL

volatile int step = 0; // are we on first measuring step or second step?
volatile int shaftSpeedL = 0; // shaft speed to convert to rads/s
volatile int shaftSpeedR = 0; // shaft speed to convert to rad/s
volatile int interruptFlag = 0; // interrupt flag

CY_ISR(MyISR) {
  interruptFlag = 1;
}

int main() {

  // --------------------------------    
  // ----- INITIALIZATIONS ----------
  CYGlobalIntEnable;

  // ------USB SETUP ----------------    
  #ifdef USE_USB
  USBUART_Start(0, USBUART_5V_OPERATION); // datasheet
  #endif

  RF_BT_SELECT_Write(0);

 // to set speed, little component will look at duty cycle of signal coming in
 // if duty cycle close to 100, then 100% of speed 
 // setting duty cycle to 100% (255)

  QuadDec_M1_Start(); // this is the counter for the left wheel
  QuadDec_M2_Start(); // this is the counter for the right wheel

 // left wheel
  PWM_1_Start();
  PWM_1_WriteCompare(200); // slowed down this motor to check difference
  PWM_1_WritePeriod(200);

// right wheel
  PWM_2_Start();
  PWM_2_WriteCompare(200); // 255 is the fastest the motor could go, can mess around with it 
  PWM_2_WritePeriod(200);

  isr_TS_StartEx(MyISR); // starting interrupt (linked to timer)(1 second period)
  Timer_TS_Start(); // starting  timer

  usbPutString(displaystring); // CS301 in putty 

  for (;;) { // ;; forever
    if (interruptFlag == 1) { // timer has counted 1s 
      isr_TS_Disable(); // disabling the interrupts
      if (step == 0) { //takes the first measurement, sets steps to 2, the next time the timer goes off, it will do the second measurement
        
        v1L = QuadDec_M1_GetCounter(); // first measurement taken - LEFT
        v1R = QuadDec_M2_GetCounter(); // first measurement taken - RIGHT
        
        step++;
      } else { // second measure and sending out to UART
        step = 0;
        // Allocating space for debugging and speed values
        char speedL[24];
        char initSpeedL[24];
        char finalSpeedL[24];
        
        char speedR[24];
        char initSpeedR[24];
        char finalSpeedR[24];
        
        v2L = QuadDec_M1_GetCounter(); // second measurement taken - LEFT
        v2R = QuadDec_M2_GetCounter(); // second measurement taken - RIGHT
        // integer to string conversion, base 10, storing, hardwaretask2 sheet for more info
        
        // converting to rad/s (v2-v1/360)*(360/16) for the left wheel
        shaftSpeedL= (((v2L - v1L)/16)*6.28319); // rads/sec
        itoa(shaftSpeedL, speedL, 10);
        itoa(v1L, initSpeedL, 10);
        itoa(v2L, finalSpeedL, 10);
        
        // converting to rad/s (v2-v1/360)*(360/16) for the right wheel
        shaftSpeedR = (((v2R - v1R)/16)*6.28319); // rads/sec
        itoa(shaftSpeedR, speedR, 10);
        itoa(v1R, initSpeedR, 10);
        itoa(v2R, finalSpeedR, 10);
        
        // printing the results on putty
        usbPutString(speedL);
        usbPutString(":");                                                                                                                                                                                     
        usbPutString(initSpeedL);
        usbPutString(":");
        usbPutString(finalSpeedL);
        
        usbPutString("    < LEFT      RIGHT >    ");
        
        usbPutString(speedR);
        usbPutString(":");                                                                                                                                                                                     
        usbPutString(initSpeedR);
        usbPutString(":");
        usbPutString(finalSpeedR);
        usbPutString("\r\n");
        usbPutString("\r\n");
        usbPutString("Distance travelled");
        
        
        QuadDec_M1_SetCounter(0); // set quad counter to 0 to avoid overflow
        QuadDec_M2_SetCounter(0); // set quad counter to 0 to avoid overflow
      }
     
      interruptFlag = 0; // interrupt flag is back to 0
      isr_TS_Enable(); // interrupt enabled
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