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

extern 
void pathfind(unsigned char* instructions, unsigned char* distances,  unsigned char (*squares)[2]);
void bfs(unsigned char (*squares)[2], unsigned char targetlocation[2]);
void generateInstructionList(unsigned char * instruction, unsigned char * distance, unsigned char( * squares)[2]);
/* [] END OF FILE */
