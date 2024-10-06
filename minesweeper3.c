#include "cmsis_os.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLCD.h"
#include "KBD.h"


#define EMPTY 'E'
#define PLAYER_X 'X'
#define PLAYER_O 'O'

#define __FI 1 

char board[3][3] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY}
};

uint32_t joy;

void delays(){
	int x = 0;
	int i;
	for(i =0; i<100000; i++){
		x = x * x;
	}
	
}

void drawBoard() {
	  
	int i,j;
    for ( i = 0; i < 3; ++i) {
        for ( j = 0; j < 3; ++j) {
            if (board[i][j] == PLAYER_X) {
                GLCD_DisplayChar(j+5, i+8,__FI, 'X');
            } else if (board[i][j] == PLAYER_O) {
                GLCD_DisplayChar(j+5, i+8,__FI, 'O');
            }else{
							GLCD_DisplayChar(j+5, i+8,__FI, 'E');
						}
        }
    }
}

int isMoveValid(int row, int col) {
    return (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == EMPTY);
}

int isGameOver() {
    // Check rows and columns
	  int i,j;
    for ( i = 0; i < 3; ++i) {
		// Row Win Condition
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return 1;  
        }
		// Column Win Condition
        if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return 1;  
        }
    }

    // Diagonal Win Condition
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return 1;  
    }
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return 1;  
    }

  
	// Checks if cells are filled
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            if (board[i][j] == EMPTY) {
                return 0;  
            }
        }
    }

    return 2;  // Tie
}

void makeMove(int row, int col, char player) {
    board[row][col] = player;
}


int play() {
    int currentPlayer;
    int row, col, result;
		char r[10], c[10];
	  GLCD_Clear(White);
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(White);
	  GLCD_DisplayString(0,0,__FI,"     Tic Tac Toe    ");
	  GLCD_DisplayString(1,0,__FI,"Position:           ");
	  currentPlayer = 1;  // Player 1 starts

	  row = 0;
	  col = 0;
	  result = 0;
	
	  // right = down
	  // down = right
	  // up = right

    while (result != 1 || result != 2) {
        drawBoard();

				sprintf(r, "%d", row+1);

				sprintf(c, "%d", col+1);
			
			  GLCD_SetBackColor(White);
			
				GLCD_DisplayString(2,  3, __FI,  "ROW");
				GLCD_DisplayString(2,  8, __FI,  (unsigned char *)c);
				GLCD_SetTextColor(Black);
			
				GLCD_DisplayString(3,  3, __FI,  "COL");
				GLCD_DisplayString(3,  8, __FI,  (unsigned char *)r);
				GLCD_SetTextColor(Black);
			
			  joy = get_button();
			  //delays();
				switch (joy) {
						case KBD_UP:
								if (col > 0) {
										col--;
								}
								break;

						case KBD_DOWN:
								//if (row < 2) {
								//		row++;
								//}
						    if(col == 2){
									col = 1;
								} else{
									col++;
								}
								break;

						case KBD_RIGHT:
//								if (col < 2) {
	//									col++;
	//							}
						
						
						    if(row == 2){
									row = 1;
								} else{
									row++;
								}
								break;

						case KBD_LEFT:
								if (row > 0) {
										row--;
								}
								break;

						case KBD_SELECT:
								if (isMoveValid(row, col)) {
										char playerSymbol = (currentPlayer == 1) ? PLAYER_X : PLAYER_O; // Switches Player Control from X to O
										makeMove(row, col, playerSymbol);
										currentPlayer = (currentPlayer == 1) ? 2 : 1;
								}
								break;

						default:
								break;
				}
        drawBoard();
				result = isGameOver();
				if(result == 1){
					delays();
					GLCD_DisplayString(8,0,__FI,"WINNER!");
					break;
				} else if(result == 2){
					delays();
					GLCD_DisplayString(8,0,__FI,"DRAW!");
					break;
				}
    }

    return 0;
}
