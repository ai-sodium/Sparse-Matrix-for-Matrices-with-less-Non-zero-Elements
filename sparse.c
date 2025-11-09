#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define CELL_WIDTH 4
#define MATRIX_START_Y 2

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printMatrix(int matrix[MAX_ROWS][MAX_COLS], int rows, int cols, int cursorX, int cursorY) {
    system("cls");
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (i == cursorY && j == cursorX) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("%3d ", matrix[i][j]);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {
                printf("%3d ", matrix[i][j]);
            }
        }
        printf("\n");
    }
    printf("\nUse arrow keys to move, type number to edit, BACKSPACE to erase, ESC to finish.\n");
}

int retry = 1;

int main() {
do{
    int rows, cols;
    int matrix[MAX_ROWS][MAX_COLS] = {0};

    printf("\nEnter number of rows (max %d): ", MAX_ROWS);
    scanf("%d", &rows);
    if (rows <= 0 || rows > MAX_ROWS) return 1;

    printf("\nEnter number of columns (max %d): ", MAX_COLS);
    scanf("%d", &cols);
    if (cols <= 0 || cols > MAX_COLS) return 1;

    int cursorX = 0, cursorY = 0;
    char buffer[10] = {0};
    int index = 0;

    printMatrix(matrix, rows, cols, cursorX, cursorY);

    while (1) {
        int ch = _getch();
        if (ch == 27) break; // ESC to finish

        if (ch == 224) { // arrow keys
            ch = _getch();
            if (ch == 72 && cursorY > 0) cursorY--;        // Up
            else if (ch == 80 && cursorY < rows - 1) cursorY++; // Down
            else if (ch == 75 && cursorX > 0) cursorX--;   // Left
            else if (ch == 77 && cursorX < cols - 1) cursorX++; // Right
            // reset buffer for new cell
            index = sprintf(buffer, "%d", matrix[cursorY][cursorX]);
        } 
        else if (ch >= '0' && ch <= '9') { // number input
            if (index < 9) {
                buffer[index++] = ch;
                buffer[index] = '\0';
                matrix[cursorY][cursorX] = atoi(buffer);
            }
        } 
        else if (ch == 8) { // backspace
            if (index > 0) {
                index--;
                buffer[index] = '\0';
                matrix[cursorY][cursorX] = (index > 0) ? atoi(buffer) : 0;
            }
        }

        printMatrix(matrix, rows, cols, cursorX, cursorY);
        // Move cursor inside highlighted cell
        gotoxy(cursorX * CELL_WIDTH, MATRIX_START_Y + cursorY);
        gotoxy(cursorX * CELL_WIDTH + index, MATRIX_START_Y + cursorY); // move cursor to end of input
    }

    // Count zeros and non-zeros
    int nonZeroCount = 0, zeroCount = 0;
    int i,j;
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
            if (matrix[i][j] != 0) nonZeroCount++;
            else zeroCount++;

    if (nonZeroCount >= zeroCount) {
        printf("\n\nError: Non-zero elements are not less than zero elements.\n");
    }
	else{
	    // Generate 3-tuple array
	    int ArrNew[nonZeroCount + 1][3];
	    ArrNew[0][0] = rows;
	    ArrNew[0][1] = cols;
	    ArrNew[0][2] = nonZeroCount;
	
	    int k = 1;
	    for (i = 0; i < rows; i++)
	        for (j = 0; j < cols; j++)
	            if (matrix[i][j] != 0) {
	                ArrNew[k][0] = i + 1;
	                ArrNew[k][1] = j + 1;
	                ArrNew[k][2] = matrix[i][j];
	                k++;
	            }
	
	    // Display 3-tuple array
	    printf("\nGenerated 3-Tuple Array (ArrNew):\nRow\tCol\tValue\n");
	    for (i = 0; i <= nonZeroCount; i++)
	        printf("%d\t%d\t%d\n", ArrNew[i][0], ArrNew[i][1], ArrNew[i][2]);
	}
        // --- Ask if user wants to retry ---
    printf("\nDo you want to retry? (1 = Yes, 0 = No): ");
    scanf("%d", &retry);
    while(getchar() != '\n'); // clear input buffer
} while (retry == 1);
    return 0;
}

