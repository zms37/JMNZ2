#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

int gameBoard[6][7] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
int playerToken = 0;
int botToken = 0;
int MAX = 100000;
int MIN = -100000;


#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
         __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })
      


// function to evaluate the board
int evaluate(int gameBoard[6][7])

    /*
    Requires: 2-D array gameBoard 
    Effects: The method evaluates gameBoard and returns the score evaluated upon each move. 
    Testing Strategy:
    covers the size and the content of the gameboard 2D array
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unpecified sized board: Failed 
    */
{
    int score = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 1; j < 8; j++)
        {
            if (gameBoard[i][j] == playerToken)
            {
                if (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken && gameBoard[i][j+3] == playerToken)
                {
                    score += 50;
                }
                if (gameBoard[i+1][j] == playerToken && gameBoard[i+2][j] == playerToken && gameBoard[i+3][j] == playerToken)
                {
                    score += 50;
                }
                if (gameBoard[i+1][j+1] == playerToken && gameBoard[i+2][j+2] == playerToken && gameBoard[i+3][j+3] == playerToken)
                {
                    score += 50;
                }
                if (gameBoard[i+1][j-1] == playerToken && gameBoard[i+2][j-2] == playerToken && gameBoard[i+3][j-3] == playerToken)
                {
                    score += 50;
                }
                if (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken)
                {
                    score += 20;
                }
                if (gameBoard[i+1][j] == playerToken && gameBoard[i+2][j] == playerToken)
                {
                    score += 20;
                }
                if (gameBoard[i+1][j+1] == playerToken && gameBoard[i+2][j+2] == playerToken)
                {
                    score += 20;
                }
                if (gameBoard[i+1][j-1] == playerToken && gameBoard[i+2][j-2] == playerToken)
                {
                    score += 20;
                }
                if (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken)
                {
                    score += 20;
                }
                if (gameBoard[i+1][j] == playerToken)
                {
                    score += 10;
                }
                if (gameBoard[i+1][j+1] == playerToken)
                {
                    score += 10;
                }
                if (gameBoard[i+1][j-1] == playerToken)
                {
                    score += 10;
                }
            }
            if (gameBoard[i][j] == botToken)
            {
                if (gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken && gameBoard[i][j+3] == botToken)
                {
                    score += 30;
                }
                if (gameBoard[i+1][j] == botToken && gameBoard[i+2][j] == botToken && gameBoard[i+3][j] == botToken)
                {
                    score += 30;
                }
                if (gameBoard[i+1][j+1] == botToken && gameBoard[i+2][j+2] == botToken && gameBoard[i+3][j+3] == botToken)
                {
                    score += 30;
                }
                if (gameBoard[i+1][j-1] == botToken && gameBoard[i+2][j-2] == botToken && gameBoard[i+3][j-3] == botToken)
                {
                    score += 30;
                }
                   if (gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken && gameBoard[i][j+3] == botToken)
                {
                    score += 30;
                }
                if (gameBoard[i+1][j] == botToken && gameBoard[i+2][j] == botToken && gameBoard[i+3][j] == botToken)
                {
                    score += 30;
                }
                if (gameBoard[i+1][j+1] == botToken && gameBoard[i+2][j+2] == botToken && gameBoard[i+3][j+3] == botToken)
                {
                    score += 30;
                }
                if (gameBoard[i+1][j-1] == botToken && gameBoard[i+2][j-2] == botToken && gameBoard[i+3][j-3] == botToken)
                {
                    score += 30;
                }
            }
        }
    }
    return score;
}
// miniMax is a recursive function that implements the miniMax algorithm to help find the best move.
int miniMax(int gameBoard[6][7], int depth, int isMaximizing, int alpha, int beta)
{
    /* 
    Requires: 2-D gameBoard array, depth of the miniMax algorithm, isMaximizing, alpha, and beta variables as parameters.
    Effects: The miniMax function is a recursive function that implements the miniMax algorithm to help find the best move of either the player or the bot from 
    the returned best variable. Recursively calling the function and comparing alpha max (that is the result of the maximum best move) with beta (that is less than alpha) 
    in the case of the availability of a maximum score, the function returns the best score. Otherwise, the function returns the minimum score by comparing the beta with the minimum if the best score from the recursive call of the function.
    Testing Strategy: covers updated gameboard, depth or miniMax algorithm, isMAximizing, alpha, and beta to find the best move
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unpecified sized board: Failed
    Partition on depth, isMaximizing, alpha, and beta, all denoted as variables:
        a- Variables = any real number: int and char: Success
        b- Variables =  double and float: Fail 
    */
   
    int score = evaluate(gameBoard);
     if (score == 30)
    {
        return score;
    }
    if (score == -30)
    {
        return score;
    }
    if (isMovesLeft(gameBoard) == 0)
    {
        return 0;
    }
    if (isMaximizing)
    {
        int best = -1000;
        for (int i = 1; i < 8; i++)
        {
            if (gameBoard[0][i] == 0)
            {
                int j = 0;
                while (gameBoard[j+1][i] == 0 && j < 5)
                {
                    j++;
                }
                gameBoard[j][i] = 2;
                best = max(best, miniMax(gameBoard, depth+1, !isMaximizing, alpha, beta));
                gameBoard[j][i] = 0;
                alpha = max(alpha, best);
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return best;
    }
    else
    {
        int best = 1000;
        for (int i = 1; i < 8; i++)
        {
            if (gameBoard[0][i] == 0)
            {
                int j = 0;
                while (gameBoard[j+1][i] == 0 && j < 5)
                {
                    j++;
                }
                gameBoard[j][i] = 1;
                best = min(best, miniMax(gameBoard, depth+1, !isMaximizing, alpha, beta));
                gameBoard[j][i] = 0;
                beta = min(beta, best);
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return best;
    }
}
// hard function that find the best move for the bot to win.
int hardMove(int gameBoard[6][7])
{
    /* 
    Requires: the gameBoard 2-D array to place the bestMove of the bot in the according column number in the gameBoard. 
    Effects: This function returns the best location for the bot to drop its token in the gameBoard while comparing the moveVal, 
    in which the return value of the miniMax function is assigned to it, with the bestVal, and returns the best column number (bestMove) 
    to place the bot's token in, increasing the winning chance of the bot.
    Testing Strategy:
    covers the size and the content of the gameboard 2D array
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unspecified sized board: Failed
    */
    int bestVal = -100000;
    int bestMove = -1;
    for (int i = 1; i < 8; i++)
    {
        if (gameBoard[0][i] == 0)
        {
            int j = 0;
            while (gameBoard[j+1][i] == 0 && j < 5)
            {
                j++;
            }
            gameBoard[j][i] = 1;
            int moveVal = miniMax(gameBoard, 0, 0, MAX, MIN);
            gameBoard[j][i] = 0;
            if (moveVal > bestVal)
            {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}
// function normal that finds the second best move for the bot.
int normalMove(int gameBoard[6][7])
{
    /* 
    Requires: the gameBoard 2-D array to place the bestMove of the bot in the according column number in the gameBoard. 
    Effects: This function returns the best location for the bot to drop its token in the gameBoard while comparing the moveVal, 
    in which the return value of the miniMax function is assigned to it, with the bestVal, and returns the best column number (bestMove) 
    to place the bot's token in, making the winning chance of the bot and the player equally likely.
    Testing Strategy:
    covers the size and the content of the gameboard 2D array
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unpecified sized board: Failed
    */
    int bestVal = -100000;
    int bestMove = -1;
    for (int i = 1; i < 8; i++)
    {
        if (gameBoard[0][i] == 0)
        {
            int j = 0;
            while (gameBoard[j+1][i] == 0 && j < 5)
            {
                j++;
            }
            gameBoard[j][i] = 1;
            int moveVal = miniMax(gameBoard, 0, 0, 10000, -10000);
            gameBoard[j][i] = 0;
            if (moveVal > bestVal)
            {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}

// function that finds the best move for the bot.
int easyMove(int gameBoard[6][7]){
    /* 
    Requires: the gameBoard 2-D array to place the bestMove of the bot in the according column number in the gameBoard. 
    Effects: This function returns the best location for the bot to drop its token in the gameBoard while comparing the moveVal, 
    in which the return value of the miniMax function is assigned to it, with the bestVal, and returns the best column number (bestMove) 
    to place the bot's token in, decreasing the winning chance of the bot.
    Testing Strategy:
    covers the size and the content of the gameboard 2D array
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unspecified sized board: Failed
    */
    int bestVal = -100000;
    int bestMove = -1;
    for (int i = 1; i < 8; i++)
    {
        if (gameBoard[0][i] == 0)
        {
            int j = 0;
            while (gameBoard[j+1][i] == 0 && j < 5)
            {
                j++;
            }
            gameBoard[j][i] = 1;
            int moveVal = miniMax(gameBoard, 0, 0, MIN, MAX);
            gameBoard[j][i] = 0;
            if (moveVal > bestVal)
            {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }
    return bestMove;
}
