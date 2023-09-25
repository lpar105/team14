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

//void updateForwardSpeed(int currentSpeed, int targetSpeed) {
//    if (currentSpeed > targetSpeed) {
//        currentFLeftSpeed = currentFLeftSpeed + 1;
//        currentFRightSpeed = currentFRightSpeed + 1;
//    }
//    else {
//        currentFLeftSpeed = currentFLeftSpeed - 1;
//        currentFRightSpeed = currentFRightSpeed - 1;
//    }
//}

void driveForward(int distanceTravelled, int targetSpeed) {  
        PWM_1_WriteCompare(currentFLeftSpeed); 
        PWM_2_WriteCompare(currentFRightSpeed); 
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
void hardAdjustLeft() {
    PWM_1_WriteCompare(currentFLeftSpeed - 5); //was 6, changed it as was too drastic
    PWM_2_WriteCompare(currentFRightSpeed + 5);
}

void hardAdjustRight() {
    PWM_1_WriteCompare(currentFLeftSpeed + 5);
    PWM_2_WriteCompare(currentFRightSpeed - 5);
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

void findTrack() {
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125));
    PWM_2_WriteCompare(currentFLeftSpeed);
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

void reverse(int reverseCount) {
    while (reverseCount !=0){
        PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125));
        PWM_2_WriteCompare(125 - (currentFRightSpeed - 125));
        reverseCount--;
    }
}
/* [] END OF FILE */