/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <project.h>
#include "motors.h"

volatile int currentFLeftSpeed = 165;
volatile int currentFRightSpeed = 165;// fresh 163, normal 165, old 167


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

void driveForward(int distanceTravelled, int targetSpeed) {  
        PWM_1_WriteCompare(165); 
        PWM_2_WriteCompare(165); 
}

void stop() {
    PWM_1_WriteCompare(125);
    PWM_2_WriteCompare(125);  
}

void adjustLeft() {
    PWM_1_WriteCompare(currentFLeftSpeed - 2);
    PWM_2_WriteCompare(currentFRightSpeed + 2);
}

void adjustRight() {
    PWM_1_WriteCompare(currentFLeftSpeed + 2);
    PWM_2_WriteCompare(currentFRightSpeed - 2);
}

void turnLeft() { 
    
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125)+ 7); //was 11, changed as I think the value becomes too small to move the motor
    PWM_2_WriteCompare(currentFRightSpeed - 7); //was 11, changed as I think the value becomes too small to move the motor
}
//193 144
void turnRight() { 
    PWM_1_WriteCompare(currentFLeftSpeed - 7);  //was 11, changed as I think the value becomes too small to move the motor
    PWM_2_WriteCompare(125 - (currentFRightSpeed - 125) + 7 ); //was 11, changed as I think the value becomes too small to move the motor
}

/* [] END OF FILE */