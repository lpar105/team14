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

void initMotors();
void driveForward(int dotsTravelled, int targetSpeed);
void updateForwardSpeed(int dotsTravelled, int targetSpeed);
void stop();
void adjustLeft();
void adjustRight();
void turnLeft();
void restoreLeft();
void restoreRight();
void hardAdjustRight();
void hardAdjustLeft();
void reverse();
void turnRight();
int getLeftPWM (void);
int getRightPWM (void);
/* [] END OF FILE */