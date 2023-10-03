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

#include <stdio.h>

#include "pathfind.h"

#include "map.h"

#define MAX_ROWS 15
#define MAX_COLS 19

int instructionCounter = 0;
int distanceCounter = 0;
int squaresCounter = 0;

int startPos[2] = {
  1,
  1
};

void pathfind(unsigned char * instruction, unsigned char * distance, unsigned char( * squares)[2]) {
  for (int i = 4; i > -1; i--) {
    if (i == 0) {
        startPos[0] = start_pos[0];
        startPos[1] = start_pos[1];
    } else {
        startPos[0] = food_list[i-1][0];
        startPos[1] = food_list[i-1][1];
    }
    bfs(squares, food_list[i]);
  }
  generateInstructionList(instruction, distance, squares);

}

void bfs(unsigned char( * squares)[2], unsigned char targetlocation[2]) {

  int i = 0;

  // Initialize data structures for BFS
  int queue[MAX_ROWS * MAX_COLS][2];
  int front = 0, rear = 0;
  unsigned char visited[15][19]; // Declare the "visited" array with the same dimensions

  // Copy the contents of the "map" array to the "visited" array
  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 19; j++) {
      visited[i][j] = map[i][j];
    }
  }
  int steps[MAX_ROWS][MAX_COLS] = {
    0
  };

  // Initialize parent set
  int parents[MAX_ROWS][MAX_COLS] = {
    0
  };

  // Define possible moves (up, down, left, right)
  //int dr[] = {-1, 1, 0, 0};
  //int dc[] = {0, 0, -1, 1};

  // Enqueue the start location
  queue[rear][0] = startPos[0];
  queue[rear][1] = startPos[1];
  rear++;

  // BFS algorithm
  int finished = 0;

  int current_row = 0; //TODO maybe not needed
  int current_col = 0;

  while (front < rear && !finished) { //TODO slightly different condition
    // Process the current location
    current_row = queue[front][0];
    current_col = queue[front][1];

    front++; //dequeue

    visited[current_row][current_col] = 1;

    // Place a step at the current location 
    // placestep(queue[front - 1], i);
    i++;
    int neighbour_array[4][2];

    neighbour_array[0][0] = current_row + 1;
    neighbour_array[0][1] = current_col;

    neighbour_array[1][0] = current_row - 1;
    neighbour_array[1][1] = current_col;

    neighbour_array[2][0] = current_row;
    neighbour_array[2][1] = current_col + 1;

    neighbour_array[3][0] = current_row;
    neighbour_array[3][1] = current_col - 1;

    // Explore neighboring cells
    for (int n = 0; n < 4; n++) {
      int nRow = neighbour_array[n][0];
      int nCol = neighbour_array[n][1];

      // Check if the new location is within bounds
      if (nRow < 1 || nRow > MAX_ROWS || nCol < 1 || nCol > MAX_COLS) {
          // Continue if out of bounds
        } else if (map[nRow][nCol] == 1) {
          // Continue if neighbour is a wall
        } else if (visited[nRow][nCol] == 1) {
          // Continue if already visited 
        } else {
          // Check if the new location is not a wall and has not been visited

          // Enqueue the new location
          queue[rear][0] = nRow; //TODO may need a plus
          queue[rear][1] = nCol;
          rear++;

          visited[nRow][nCol] = '1';
          steps[nRow][nCol] = steps[current_row][current_col] + 1;

          // Set the parent direction
          if (nRow + 1 == current_row) {
            parents[nRow][nCol] = 1;
          }
          else if(nCol + 1 == current_col) {
            parents[nRow][nCol] = 2;
          }
          else if(nRow - 1 == current_row) {
            parents[nRow][nCol] = 3;
          }
          else if(nCol - 1 == current_col) {
            parents[nRow][nCol] = 4;
          }

        }
      }

      // Check if the target location is reached
      if (current_row == targetlocation[0] && current_col == targetlocation[1]) {
        finished = 1;
        break;
      }
    }

  // Backtrack to find the path and highlight steps
  int fpos1 = current_row;
  int fpos2 = current_col;
    
// Highlight the current location
  squares[squaresCounter][0] = current_row;
  squares[squaresCounter][1] = current_col;
  squaresCounter++;

  while (startPos[0] != fpos1 || startPos[1] != fpos2) {
    int parent_dir = parents[fpos1][fpos2];

    if (parent_dir == 1) {
      fpos1++;
    } else if (parent_dir == 2) {
      fpos2++;
    } else if (parent_dir == 3) {
      fpos1--;
    } else if (parent_dir == 4) {
      fpos2--;
    }

    // Highlight the step
    //highlightstep(fpos1, fpos2);
    squares[squaresCounter][0] = fpos1;
    squares[squaresCounter][1] = fpos2;
    squaresCounter++;
  }
  //highlightstep(current_row, current_col);

}

void generateInstructionList(unsigned char * instruction, unsigned char * distance, unsigned char( * squares)[2]) {
    unsigned char reversedSquares[500][2];  // New array to store reversed and filtered entries
    int count = 0;  // Count of non-matching entries
    
    // Reverse and filter the entries
    for (int i = 499; i >= 0; i--) {
        distance[i] = 70; //TODO remove
        //if (squares[i][0] != 111 || squares[i][1] != 111) {
            // Copy non-matching entry to the new array
            reversedSquares[count][0] = squares[i][0];
            reversedSquares[count][1] = squares[i][1];
            count++;
        //}
    }
    
    for (int i = 0; i < 500; i++) {
        squares[i][0] = reversedSquares[i][0];
        squares[i][1] = reversedSquares[i][1];
    }
    
    // 0 - Stop 
    // 1 - Left
    // 2 - Straight
    // 3 - Right
    // 4 - 180
    // 5 - Left w/distance
    // 6 - Straight w/distance
    // 7 - Right w/distance
    // 8 - 180 w/distance
    
    for (int i = 0; i < 499; i++) {
        
        unsigned char startRow = squares[i][0];
        unsigned char startCol = squares[i][1];
        unsigned char nextMoveRow = squares[i+1][0];
        unsigned char nextMoveCol = squares[i+1][1];
        
        unsigned char travelDir = 55;
        
        // Work out direction of travel
        if ((startRow == nextMoveRow && startCol == nextMoveCol) || (nextMoveRow == 55 && nextMoveCol == 55)) {
            travelDir = 0; // stop
        } else if (startRow - 1 == nextMoveRow) {
            travelDir = 1; // global up
        } else if (startRow + 1 == nextMoveRow) {
            travelDir = 2; // global down
        } else if (startCol + 1 == nextMoveCol) {
            travelDir = 3; // global right
        } else if (startCol - 1 == nextMoveCol) {
            travelDir = 4; // global left
        }
        
        // Work out at what point we need a new instruction. 
        // Either the square is at an intersection or a change in target direction 
        int endOfInstruction = 0;
        while (endOfInstruction == 0) {
            if (startRow == 55) {
                i++;
                break;
            }
            if (travelDir == 3 || travelDir == 4) {
                if (map[nextMoveRow + 1][nextMoveCol] == 0 || map[nextMoveRow - 1][nextMoveCol] == 0 ) {
                    // END OF INSTRUCTION
                    endOfInstruction = 1;
                }
            } else { // doesn't account for stop condition yet
                if (map[nextMoveRow ][nextMoveCol +1] == 0 || map[nextMoveRow][nextMoveCol-1] == 0 ) {
                    // END OF INSTRUCTION
                    endOfInstruction = 1;
                }
            }
            
            if (travelDir == 1 ) {
                if (squares[i][0]  -1!= nextMoveRow){
                    // END OF INSTRUCTION
                    endOfInstruction = 1;
                }
            } else if (travelDir == 2 ) {
                if (squares[i][0] +1!= nextMoveRow){
                    // END OF INSTRUCTION
                    endOfInstruction = 1;
                }
            } else if (travelDir == 3 ) {
                if (squares[i][1]+1 != nextMoveCol){
                    // END OF INSTRUCTION
                    endOfInstruction = 1;
                }
            } else if (travelDir == 4 ) {
                if (squares[i][1] -1!= nextMoveCol ) {
                    // END OF INSTRUCTION
                    endOfInstruction = 1;
                }
            }
            
           if (endOfInstruction == 0) {
                i++;
                nextMoveRow = squares[i+1][0];
                nextMoveCol = squares[i+1][1];
            }
            
        }
        
        // Work out which instruction
        /*if (travelDir == 1 || travelDir == 2 ) {
            instruction[instructionCounter] = stopRow;
        } else {
            instruction[instructionCounter] = stopCol;
        }*/
        instruction[instructionCounter] = travelDir;
        instructionCounter++;
        
        
        // Add info to lists
        
    }

}

/* [] END OF FILE */