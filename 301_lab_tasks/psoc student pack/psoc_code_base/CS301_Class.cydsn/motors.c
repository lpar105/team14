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

void initMotors() {
    PWM_1_WritePeriod(250);
    PWM_2_WritePeriod(250);
    PWM_1_Start();
    PWM_2_Start();

}

void driveForward() {
    PWM_1_WriteCompare(160);
    PWM_2_WriteCompare(160); 
}

void stop() {
    PWM_1_WriteCompare(125);
    PWM_2_WriteCompare(125);  
}

void adjustLeft() {
    PWM_1_WriteCompare(165);
    PWM_2_WriteCompare(175);
}

void adjustRight() {
    PWM_1_WriteCompare(175);
    PWM_2_WriteCompare(165);
}

void turnLeft() {
    PWM_1_WriteCompare(0);
    PWM_2_WriteCompare(255);
    
}

void turnRight() {
    PWM_1_WriteCompare(0);
    PWM_2_WriteCompare(255);
}
/* [] END OF FILE */