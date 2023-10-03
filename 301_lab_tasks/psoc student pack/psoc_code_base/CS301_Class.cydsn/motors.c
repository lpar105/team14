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
volatile int currentFRightSpeed = 165;

int getDistance(int i) {
    if (i == 4) { //whatever 'i' point corresponds to the first distance from the turn to where it stops
        return 50; //returns how far it should travel from the corner to the food particle
    }
    if (i == 5) { //whatever 'i' point corresponds to the second distance from the turn to where it stops
        return 50; //returns how far it should travel from the corner to the food particle
    }
    if (i == 7) { //whatever 'i' point corresponds to the third distance from the turn to where it stops
        return 50; //returns how far it should travel from the corner to the food particle
    }
    if (i == 69) { //whatever 'i' point corresponds to the fourth distance from the turn to where it stops
        return 50; //returns how far it should travel from the corner to the food particle
    }
    if (i == 420) { //whatever 'i' point corresponds to the fifth distance from the turn to where it stops
        return 50; //returns how far it should travel from the corner to the food particle
    }
    else {
        return 10;
    }
}

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

void adjustLeft() {
    PWM_1_WriteCompare(currentFLeftSpeed - 2);
    PWM_2_WriteCompare(currentFRightSpeed + 2);
}

void adjustRight() {
    PWM_1_WriteCompare(currentFLeftSpeed + 2);
    PWM_2_WriteCompare(currentFRightSpeed - 2);
}

void findTrack() {
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125));
    PWM_2_WriteCompare(currentFLeftSpeed);
}

// MOTOR FUNCTIONS IN CODE

void stop() {  //INDEXED BY 0
    PWM_1_WriteCompare(125);
    PWM_2_WriteCompare(125);  
}

void turnLeft() { //INDEXED BY 1
    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125) + 7); //was 11
    PWM_2_WriteCompare(currentFRightSpeed - 7); //was 11
}

void driveForward() {  //INDEXED BY 2. This function means to go forward until I hit a wall
    PWM_1_WriteCompare(currentFLeftSpeed); 
    PWM_2_WriteCompare(currentFRightSpeed); 
}

void turnRight() { //INDEXED BY 3
    PWM_1_WriteCompare(currentFLeftSpeed - 7);  //was 11
    PWM_2_WriteCompare(125 - (currentFRightSpeed - 125) + 7 ); //was 11
}

void turnAround() { //INDEXED BY 4
    PWM_1_WriteCompare(currentFLeftSpeed - 7);  //was 11
    PWM_2_WriteCompare(125 - (currentFRightSpeed - 125) + 7 ); //was 11
}

void reverse(int reverseCount) { //not used yet
    while (reverseCount !=0){
        PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125));
        PWM_2_WriteCompare(125 - (currentFRightSpeed - 125));
        reverseCount--;
    }
}

// UNUSED FUNCTIONS:


//void restoreLeft() {
//    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125) - 1);
//    PWM_2_WriteCompare(125 - (currentFLeftSpeed - 125) + 1);
//}
//
//void restoreRight() {
//    PWM_1_WriteCompare(125 - (currentFLeftSpeed - 125) + 1);
//    PWM_2_WriteCompare(125 - (currentFLeftSpeed - 125) - 1);
//    
    //void hardAdjustLeft() {
//    PWM_1_WriteCompare(currentFLeftSpeed - 5); //was 6, changed it as was too drastic
//    PWM_2_WriteCompare(currentFRightSpeed + 5);
//}
//
//void hardAdjustRight() {
//    PWM_1_WriteCompare(currentFLeftSpeed + 5);
//    PWM_2_WriteCompare(currentFRightSpeed - 5);
//}
/* [] END OF FILE */