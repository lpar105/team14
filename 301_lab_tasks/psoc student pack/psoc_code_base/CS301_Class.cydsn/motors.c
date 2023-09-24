/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * Last edited 23/09/2023 by Hira
 * ========================================
*/

#include <project.h>
#include "motors.h"

volatile int currentFLeftSpeed = 169;
volatile int currentFRightSpeed = 169;


void initMotors() {
    PWM_1_WritePeriod(250);
    PWM_2_WritePeriod(250);
    PWM_1_Start();
    PWM_2_Start();

}

int getLeftPWM(void) {
    return currentFLeftSpeed;
}

int getRightPWM(void) {
    return currentFRightSpeed;
}

void updateForwardSpeed(int speedL, int speedR, int targetSpeed) {
    if (abs(speedL - targetSpeed) > 10) {
    } else if (speedL < targetSpeed) {
        currentFLeftSpeed = currentFLeftSpeed + 2 * ((float)1 - (speedL / (float)targetSpeed));
        
    } else if (speedL > targetSpeed) {
        currentFLeftSpeed = currentFLeftSpeed - 2 * ((float)1 - (targetSpeed/(float)speedL));
        
    }
    if (speedR < targetSpeed) {
        currentFRightSpeed = currentFRightSpeed + 2 * ((float)1 - (speedR / (float)targetSpeed));
    } else if (speedR > targetSpeed) {
        currentFRightSpeed = currentFRightSpeed - 2 * ((float)1 - (targetSpeed/(float)speedR));
    } 
}

void driveForward(int speedL, int speedR, int targetSpeed) {
    
    PWM_1_WriteCompare(currentFLeftSpeed); 
    PWM_2_WriteCompare(currentFRightSpeed); 
}

void stop() {
    PWM_1_WriteCompare(125);
    PWM_2_WriteCompare(125);  
}

void adjustLeft() {
    PWM_1_WriteCompare(currentFLeftSpeed - 3);
    PWM_2_WriteCompare(currentFRightSpeed + 3);
}

void adjustRight() {
    PWM_1_WriteCompare(currentFLeftSpeed + 3);
    PWM_2_WriteCompare(currentFRightSpeed - 3);
}
void hardAdjustLeft() {
    PWM_1_WriteCompare(currentFLeftSpeed - 6);
    PWM_2_WriteCompare(currentFRightSpeed + 6);
}

void hardAdjustRight() {
    PWM_1_WriteCompare(currentFLeftSpeed + 6);
    PWM_2_WriteCompare(currentFRightSpeed - 6);
}


void restoreLeft() {
    // 125 - currentFLeftSpeed - 125
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125) - 1);
    PWM_2_WriteCompare(125 - (currentFLeftSpeed - 125) + 1);
}

void restoreRight() {
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125) + 1);
    PWM_2_WriteCompare(125 - (currentFLeftSpeed - 125) - 1);
}

void turnLeft() {
   
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125)+ 11); 
    PWM_2_WriteCompare(currentFRightSpeed - 11);
}
//193 144
void turnRight() { 
    PWM_1_WriteCompare(currentFLeftSpeed - 11);  
    PWM_2_WriteCompare(125 - (currentFRightSpeed - 125) +11 );
}

void reverse() {
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125));
    PWM_2_WriteCompare(125 - (currentFRightSpeed - 125));
}
/* [] END OF FILE */