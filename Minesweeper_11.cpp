#include <iostream>  
#include <vector>   
#include<time.h>    
#include <stdlib.h> 

using namespace std;
 
const int BEGINNER = 0;
const int INTERMEDIATE = 1;
const int ADVANCED = 2;
const int MAXSIDE = 30;
 
int NROWS;
int NCOLUMNS;
int MINES;
 
void chooseDifficultyLevel() {
    int level;
 
    cout << "Enter the Difficulty Level" << endl;
    cout << "Press 0 for BEGINNER     (9  * 9  cells and 10 mines)" << endl;
    cout << "Press 1 for INTERMEDIATE (16 * 16 cells and 40 mines)" << endl;
    cout << "Press 2 for ADVANCED     (16 * 30 cells and 99 mines)" << endl;
 
    cin >> level;
    if (level == BEGINNER) {
        NROWS = 9;
        NCOLUMNS = 9;
        MINES = 10;
    }
    else if (level == INTERMEDIATE) {
        NROWS = 16;
        NCOLUMNS = 16;
        MINES = 40;
    }
 
    else if (level == ADVANCED) {
        NROWS = 16;
        NCOLUMNS = 30;
        MINES = 99;
    }
 	
	else{
		cout<<"Invalid Input"<<endl;
		exit(0);
	}
    return ;
}
 
void clearBoards(char mineBoard[][MAXSIDE], char gameBoard[][MAXSIDE]) {
    for (int i = 0; i < NROWS; i++)
        for (int j = 0; j < NCOLUMNS; j++)
            gameBoard[i][j] = mineBoard[i][j] = '.';
    return;
}
 
void placeMines(char mineBoard[][MAXSIDE], int mines) {
    int placed = 0;
    int rand(void);
    while (placed < mines) {
        int random = rand() % (NROWS * NCOLUMNS);
        int row = random / NCOLUMNS;
        int col = random % NROWS;
        if (mineBoard[row][col] == '#') continue; 
        mineBoard[row][col] = '#';
        placed++;
    }
    return;
}
void replaceMine(int row, int col, char mineBoard[][MAXSIDE]) {
    placeMines(mineBoard, 1); 
    mineBoard[row][col] = '.';
    return;
}
 
char indexToChar(int index) {
    if (index < 10)
        return index + '0';
    else
        return 'a' + (index - 10);
}
 
int charToIndex(char ch) {
    if (ch <= '9')
        return ch - '0';
    else
        return (ch - 'a') + 10;
}
 
void displayBoard(char gameBoard[][MAXSIDE]) {
    cout << "    ";
    for (int i = 0; i < NCOLUMNS; i++)
        cout << indexToChar(i) << ' ';
    cout << endl << endl;
 
    for (int i = 0; i < NROWS; i++) {
        cout << indexToChar(i) << "   ";
        for (int j = 0; j < NCOLUMNS; j++)
            cout << gameBoard[i][j] << " ";
        cout << "  " << indexToChar(i);
        cout << endl;
    }
 
    cout << endl << "    ";
    for (int i = 0; i < NCOLUMNS; i++)
        cout << indexToChar(i) << ' ';
    cout << endl;
 
    return;
}
 
bool isValid(int row, int col) {
    return (row >= 0) && (row < NROWS) && (col >= 0) && (col < NCOLUMNS);
}
 
bool isMine(int row, int col, char board[][MAXSIDE]) {
    return (board[row][col] == '#');
}
 
vector< pair<int, int> > getNeighbours(int row, int col) {
   
 
    vector < pair <int, int> > neighbours;
 
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
            if (dx != 0 || dy != 0)
                if (isValid(row+dx, col+dy))
                    neighbours.push_back(make_pair(row+dx, col+dy));
 
    return neighbours;
}
 
int countAdjacentMines(int row, int col, char mineBoard[][MAXSIDE]) {
    vector < pair <int, int> > neighbours = getNeighbours(row, col);
 
    int count = 0;
    for (int i = 0; i < neighbours.size(); i++)
        if (isMine(neighbours[i].first, neighbours[i].second, mineBoard))
            count++;
 
    return count;
}
 
void uncoverBoard(char gameBoard[][MAXSIDE], char mineBoard[][MAXSIDE], int row, int col, int *nMoves) {
    (*nMoves)++;
    int count = countAdjacentMines(row, col, mineBoard);
    gameBoard[row][col] = count + '0';
 
    if (count == 0) {
        vector < pair <int, int> > neighbours = getNeighbours(row, col);
 
        for (int i = 0; i < neighbours.size(); i++)
            if (gameBoard[neighbours[i].first][neighbours[i].second] == '.')
                uncoverBoard(gameBoard, mineBoard, neighbours[i].first, neighbours[i].second, nMoves);
    }
 
    return;
}
 
void markMines(char gameBoard[][MAXSIDE], char mineBoard[][MAXSIDE], bool won) {
    for (int i = 0; i < NROWS; i++) {
        for (int j = 0; j < NCOLUMNS; j++) {
            if (gameBoard[i][j] == '.' && mineBoard[i][j] == '#') {
                if (won) {
                    gameBoard[i][j] = 'F';
                }
                else {
                    gameBoard[i][j] = '#';
                }
            }
        }
    }
}
 
void playMinesweeper() {
    char mineBoard[MAXSIDE][MAXSIDE], gameBoard[MAXSIDE][MAXSIDE];
    int nMovesTotal = NROWS * NCOLUMNS - MINES;
    int flags = MINES;
    clearBoards(mineBoard, gameBoard);
    placeMines(mineBoard, MINES);
 
    int nMoves = 0;
    bool gameOver = false;
 
    while (!gameOver) {
        displayBoard(gameBoard);
        cout << flags << " flags left" << endl << endl;
 
        char x, y, z;
        cout << "Enter your move, (row, column, safe(s)/flag(f)) -> ";
        cin >> x >> y >> z;
        cout << endl;
 
        int row = charToIndex(x);
        int col = charToIndex(y);
 
        if (nMoves == 0)
            if (isMine(row, col, mineBoard))
                replaceMine(row, col, mineBoard);
 
        if (z == 's') {
            if (gameBoard[row][col] == '.' && mineBoard[row][col] == '.') {
                uncoverBoard(gameBoard, mineBoard, row, col, &nMoves);
                if (nMoves == nMovesTotal) {
                    markMines(gameBoard, mineBoard, true);
                    displayBoard(gameBoard);
                    cout << endl << "You won!Well played" << endl;
                    gameOver = true;
                }
            }
            else if (gameBoard[row][col] == '.' && mineBoard[row][col] == '#') {
                
                gameBoard[row][col] = '#';
                markMines(gameBoard, mineBoard, false);
                displayBoard(gameBoard);
                cout << endl << "You lost!Better luck next time" << endl;
                gameOver = true;
            }
            else {
                
                cout << "Illegal move. ";
                if (gameBoard[row][col] == 'F')
                    cout << "Cell is a flag. Use f to toggle flag off. ";
                else
                    cout << "Cell is already a number. ";
                cout << endl;
            }
        }
 
        if (z == 'f') {
            if (gameBoard[row][col] == '.') {
                if (flags != 0) {
                    gameBoard[row][col] = 'F';
                    flags--;
                }
                else {
                    cout << "Illegal move. Too many flags!" << endl;
                }
            }
            else if (gameBoard[row][col] == 'F') {
    
                gameBoard[row][col] = '.';
                flags++;
            }
            else {
            
                cout << "Illegal move. Cell is a number. " << endl;
            }
        }
    }
 
    return;
}
 
int main() {
    srand(time(0));
    chooseDifficultyLevel();
    playMinesweeper();
    return 0;
}
