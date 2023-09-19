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

volatile int currentFLeftSpeed = 170;
volatile int currentFRightSpeed = 170;


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
    if (speedL == 0 || speedR == 0) {
    }
    else if (speedL < targetSpeed) {
        currentFLeftSpeed = currentFLeftSpeed + 10 * ((double)1 - (speedL / (double)targetSpeed));
        
    } else if (speedL > targetSpeed) {
        currentFLeftSpeed = currentFLeftSpeed - 10 * ((double)1 - (targetSpeed/(double)speedL));
        
    } else {
        
    }
    
    if (speedL == 0 || speedR == 0) {
    }
    else if (speedR < targetSpeed) {
        currentFRightSpeed = currentFRightSpeed + 10 * ((double)1 - (speedR / (double)targetSpeed));
    } else if (speedR > targetSpeed) {
        currentFRightSpeed = currentFRightSpeed - 10 * ((double)1 - (targetSpeed/(double)speedR));
    } else {
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
    PWM_1_WriteCompare(85);
    PWM_2_WriteCompare(165);
    
}

void turnRight() {
    PWM_1_WriteCompare(165); 
    PWM_2_WriteCompare(85);
}

void reverse() {
    PWM_1_WriteCompare(125);
    PWM_2_WriteCompare(125);
    //PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125));
    //PWM_2_WriteCompare(125 - (currentFRightSpeed - 125));
}
/* [] END OF FILE */