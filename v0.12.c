#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
// coin
typedef enum coin
{
    HEADS,
    TAILS
} coin;
int gameBoard[6][7] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
double totalPlayer1 = 0, totalPlayer2 = 0;
char player[10];
char player1[10];
char player2[10];
char bot[3]="bot";
int player1Start = 0;
int player2Start = 0;
int botStart = 0;
int playerOrBot = 0;
//int botStart = 0;
clock_t startPlayer;
clock_t endPlayer;
double playerTime =0;
double player1Time = 0;
double player2Time = 0;

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
         __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

coin flipCoin();
void display();
int isWinner();
void startGame();
int setMove(int gameBoard[6][7], int playerToken);
void playerVsPlayer();
void botMove (int gameBoard[6][7], int token);
int evaluate(int gameBoard[6][7]);
int minimax(int gameBoard[6][7], int depth, bool isMaximizing, int playerToken);
int vsPlayerOrBot();
void playerVsBot();
int isMovesLeft(int gameBoard[6][7]);
int main()
{
   printf("Welcome to Connect Four Game\n a game developed by JMNZ \n \n we hope you enjoy it \n \n");
   startGame();
  
    return 0;
}

// Function to flip a coin
    coin flipCoin()
        /*
*Requires: nothing
*Effects: returns Heads if the random number generated is even, and returns Tails if the random number generated is odd.
*/
{
    if (rand() % 2 == 0)
    {
        return HEADS;
    }
    else
    {
        return TAILS;
    }
}

// Function to display board
void display(int gameBoard[6][7])
    /*Requires: a 2D array consisting of 6 rows and 7 columns 
Effects: returns a 2D array, being sandwiched between 2 lines of dashes. Above the 2D array, a sequence of line from 1 to 7 is printed to indicate the column numbers, respectively.
*/
{
   
    printf("\n");
    printf(" ");
    for (int n = 1; n < 7; n++)
    {
        printf("%d-", n);
    }
    printf("7");
    printf("\n");

    for (int dashes = 1; dashes <= 15; dashes++)
    {
        printf("-");
    }
    printf("\n");
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            printf("|%d", gameBoard[i][j+1]);
        }
        printf("|\n");
    }
    for (int dashes = 1; dashes <= 15; dashes++)
    {
        printf("-");
    }
}

// Winners
//player token is 1 or 2
int isWinner(int playerToken)
    /*
    Requires: The token number that is assinged to the player
    Effects: The gameboard is updated within every move made by each player. Simultaneously, we continously check if 4 tokens of the same player allign vertically, horizontally,
    or diagonally. If one of these conditions is satisfied, we return true, signafying that the player of the inputed token is the winner. False otherwise.
    */
{
    int count = 0;
    // Horizontal
    for (int i = 0; i < 6; i++)
    {
        for (int n = 0; n < 8; n++)
        {
            if (gameBoard[i][n] == playerToken)
            {
                count++;
            }
            else
            {
                count = 0;
            }
            if (count == 4)
            {
                return 1;
            }
        }
        count = 0;
    }
    // Vertical
    for (int i = 0; i < 7; i++)
    {
        for (int n = 0; n < 7; n++)
        {
            if (gameBoard[n][i] == playerToken)
            {
                count++;
            }
            else
            {
                count = 0;
            }
            if (count == 4)
            {
                return 1;
            }
        }
        count = 0;
    }
    //positively sloped diagonal
    for(int i = 0 ; i < 5 ; i++){
        for(int n = 0 ; n < 4 ; n++){
            if(gameBoard[n][i] == playerToken && gameBoard[n+1][i+1] == playerToken && gameBoard[n+2][i+2] == playerToken && gameBoard[n+3][i+3] == playerToken){
                return 1;
            }
        }
    }

    //negatively sloped diagonal
    for(int i = 0 ; i < 5 ; i++){
        for(int n = 3 ; n < 7 ; n++){
            if(gameBoard[n][i] == playerToken && gameBoard[n-1][i+1] == playerToken && gameBoard[n-2][i+2] == playerToken &&gameBoard[n-3][i+3] == playerToken){
				return 1;
            }
        }
    }
    return 0;

}

// function tie to determine who is the winner if the game is a tie, based on the players time.
void tie(double player1Time, double player2Time) 
{
    /*
    Requires: player1Timer and player2Timer that record the total time taken for players 1 and 2 to play their move.
    
    Effects: the method declares the winner by comparing the total time consumed by both players and prints the player's name that took the least time selecting their moves (If no more tokens remain). 
    In case both durations are equal, the game declares a tie.
    */
    
    
    // if the player 1 time is less than the player 2 time.
    if (player1Time < player2Time)
    {
        // print the winner is player 1.
        printf("%s's time is: %f\n",player1,player1Time);
        printf("%s's time is: %f\n",player2,player2Time);
        printf("The winner is %s!!!!", player1);
        printf("\n");
        printf("\n");
    }
    // if the player 2 time is less than the player 1 time.
    else if (player2Time < player1Time)
    {
        // print the winner is player 2.
        printf("%s's time is: %f\n",player1,player1Time);
        printf("%s's time is: %f\n",player2,player2Time);
        printf("The winner is %s!!!!", player2);
        printf("\n");
        printf("\n");
    }
    // if the player 1 time is equal to the player 2 time.
    else
    {
        // print the game is a tie.
        printf("The game is unsolved tie");
        printf("\n");
        printf("\n");
    }
}
// function to create the players and their names and the tokins to be 1 for player 1 and 2 for player 2.
void createPlayers(){
   
   /*
   Requires: no paramters required by the method
Effects: This method outputs the names of the first and second players, prints the who is player 1 and player 2, and assigns token 1 for player 1 and token 2 for player 2.
*/
 if(playerOrBot == 1){  

    printf("Player 1 please enter your name: ");
    scanf("%s", player1);
    printf("\n");
    printf("\n");
    printf("Player 2 please enter your name: "); 
    scanf("%s", player2);
    printf("\n");
    printf("\n");
    printf("%s is player 1\n", player1);
    printf("%s is player 2\n", player2);
    printf("\n");
    printf("\n");
    printf("Player 1 is token 1\n");
    printf("Player 2 is token 2\n");
    printf("\n");
    printf("\n");
    }
    else{
        printf("Player 1 please enter your name: ");
        scanf("%s", player1);
        printf("\n");
        printf("\n");
        printf("%s is player 1\n", player1);
        printf("\n");
        printf("\n");
        printf("Player 1 is token 1\n");
        printf("\n");
        printf("\n");
    }
}

// a function to set the moves for each player and change the tokins in the gameboard accordingly.

// a fuction to start the game.
//the function starts with using the createPlayers function to create the players and their names and the tokins to be 1 for player 1 and 2 for player 2.
//then the function uses the flipcoin function to set the truns for the players and safe the truns in variables.
//then the function set the game to be on.
//then the function ask the players to enter their move, calculate the time for each move and add it to the total time for each player, edit the borad using setMove function , print the board using display function.
//the function sets the winner using the isWinner function and declearing the winner.
//if the result is tie the function uses the tie function to declear the tie.
//the function ends when the game is off.

//function for player vs player that takes the gameboard and the playertoken 
void playerVsPlayer(int gameBoard[6][7], int playerToken)
{
    /*
    Requires: gameBoard and playerToken as parameters.
    
    Effects: This method is used to play the game between two players. It takes the gameBoard and playerToken as parameters and uses the setMove function to set the moves for each player and change the tokins in the gameboard accordingly. It also uses the display function to display the gameboard after each move. It uses the isWinner function to check if the player has won the game and uses the tie function to check if the game is a tie. It also uses the flipCoin function to decide who plays first.
    */
    // create the players and their names and the tokins to be 1 for player 1 and 2 for player 2.
    createPlayers();
    // set the truns for the players and safe the truns in variables.
    int turn = flipCoin();
    if (flipCoin() == HEADS)
        {
            player1Start = 1;
            printf("%s is red and %s is yellow.", player1, player2);
            printf("\n");
        }
        else
        {
        
            
            player2Start = 1;
            printf("%s is red and %s is yellow.", player2, player1);
            printf("\n");
        }
    // set the game to be on.
    int gameOn = 1;
    // set the total time for each player to be 0.
    double player1Time = 0;
    double player2Time = 0;
    // while the game is on.
    while (gameOn == 1)
    {
        // if the turn is 1.
        if (turn == 1)
        {
            // ask the player 1 to enter their move.
            printf("%s please enter your move: ", player1);
            // calculate the time for the move.
            clock_t start = clock();
            // set the move using the setMove function.
            setMove(gameBoard, playerToken);
            // calculate the time for the move.
            clock_t end = clock();
            // add the time for the move to the total time for player 1.
            player1Time += (double)(end - start) / CLOCKS_PER_SEC;
            // display the gameboard after the move.
            display(gameBoard);
            // check if the player has won the game.
            if (isWinner(playerToken) == 1)
            {
                // print the winner is player 1.
                printf("The winner is %s!!!!", player1);
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // check if the game is a tie.
            else if (isWinner(playerToken) == 0)
            {
                // print the game is a tie.
                printf("The game is unsolved tie");
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // if the player has not won the game and the game is not a tie.
            else
            {
                // set the turn to be 2.
                turn = 2;
            }
        }
        // if the turn is 2.
        else
        {
            // ask the player 2 to enter their move.
            printf("%s please enter your move: ", player2);
            // calculate the time for the move.
            clock_t start = clock();
            // set the move using the setMove function.
            setMove(gameBoard, playerToken);
            // calculate the time for the move.
            clock_t end = clock();
            // add the time for the move to the total time for player 2.
            player2Time += (double)(end - start) / CLOCKS_PER_SEC;
            // display the gameboard after the move.
            display(gameBoard);
            // check if the player has won the game.
            if (isWinner(playerToken) == 1)
            {
                // print the winner is player 2.
                printf("The winner is %s!!!!", player2);
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // check if the game is a tie.
            else if (isWinner(playerToken) == 0)
            {
                // print the game is a tie.
                printf("The game is unsolved tie");
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // if the player has not won the game and the game is not a tie.
            else
            {
                // set the turn to be 1.
                turn = 1;
            }
        }
    }
    // print the total time for each player.
    printf("The total time for %s is %f seconds", player1, player1Time);
    printf("\n");
    printf("\n");
    printf("The total time for %s is %f seconds", player2, player2Time);
    printf("\n");
    printf("\n");
}

//function playerMove to setmove for one player.
void playerMove(int gameBoard[6][7], int playerToken)
{
    /*
    Requires: gameBoard and playerToken as parameters.
    
    Effects: This method is used to play the game between one player and the computer. It takes the gameBoard and playerToken as parameters and uses the setMove function to set the moves for each player and change the tokins in the gameboard accordingly. It also uses the display function to display the gameboard after each move. It uses the isWinner function to check if the player has won the game and uses the tie function to check if the game is a tie. It also uses the flipCoin function to decide who plays first.
    */
    // create the players and their names and the tokins to be 1 for player 1 and 2 for player 2.
    createPlayers();
    // set the truns for the players and safe the truns in variables.
    int turn = flipCoin();
    if (flipCoin() == HEADS)
        {
            player1Start = 1;
            printf("%s is red and the computer is yellow.", player1);
            printf("\n");
        }
        else
        {
        
            
            player2Start = 1;
            printf("The computer is red and %s is yellow.", player1);
            printf("\n");
        }
    // set the game to be on.
    int gameOn = 1;
    // set the total time for each player to be 0.
    double player1Time = 0;
    double player2Time = 0;
    // while the game is on.
    while (gameOn == 1)
    {
        // if the turn is 1.
        if (turn == 1)
        {
            // ask the player 1 to enter their move.
            printf("%s please enter your move: ", player1);
            // calculate the time for the move.
            clock_t start = clock();
            // set the move using the setMove function.
            setMove(gameBoard, playerToken);
            // calculate the time for the move.
            clock_t end = clock();
            // add the time for the move to the total time for player 1.
            player1Time += (double)(end - start) / CLOCKS_PER_SEC;
            // display the gameboard after the move.
            display(gameBoard);
            // check if the player has won
            if (isWinner(playerToken) == 1)
            {
                // print the winner is player 1.
                printf("The winner is %s!!!!", player1);
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // check if the game is a tie.
            else if (isWinner(playerToken) == 0)
            {
                // print the game is a tie.
                printf("The game is unsolved tie");
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // if the player has not won the game and the game is not a tie.
            else
            {
                // set the turn to be 2.
                turn = 2;
            }
        }
        // if the turn is 2.
        else
        {
            // ask the player 2 to enter their move.
            printf("The computer is thinking...");
            printf("\n");
            // calculate the time for the move.
            clock_t start = clock();
            // set the move using the setMove function.
            setMove(gameBoard, playerToken);
            // calculate the time for the move.
            clock_t end = clock();
            // add the time for the move to the total time for player 2.
            player2Time += (double)(end - start) / CLOCKS_PER_SEC;
            // display the gameboard after the move.
            display(gameBoard);
            // check if the player has won the game.
            if (isWinner(playerToken) == 1)
            {
                // print the winner is player 2.
                printf("The winner is the computer!!!!");
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // check if the game is a tie.
            else if (isWinner(playerToken) == 0)
            {
                // print the game is a tie.
                printf("The game is unsolved tie");
                printf("\n");
                printf("\n");
                // set the game to be off.
                gameOn = 0;
            }
            // if the player has not won the game and the game is not a tie.
            else
            {
                // set the turn to be 1.
                turn = 1;
            }
        }
    }
    // print the total time for each player.
    printf("The total time for %s is %f seconds", player1, player1Time);
    printf("\n");
    printf("\n");
    printf("The total time for the computer is %f seconds", player2Time);
    printf("\n");
    printf("\n");
}


//function setMove that takes the gameboard and the playertoken
int setMove(int gameBoard[6][7], int playerToken)
{
    /*
    Requires: gameBoard and playerToken as parameters.
    
    Effects: This method is used to set the moves for each player and change the tokins in the gameboard accordingly. It takes the gameBoard and playerToken as parameters and uses the display function to display the gameboard after each move. It uses the isWinner function to check if the player has won the game and uses the tie function to check if the game is a tie. It also uses the flipCoin function to decide who plays first.
    */
    // set the move to be 0.
    int move = 0;
    // while the move is not valid.
    while (move == 0)
    {
        // set the move to be 0.
        move = 0;
        // set the column to be 0.
        int column = 0;
        // ask the player to enter the column.
        scanf("%d", &column);
        // if the column is 1.
        if (column == 1)
        {
            // if the first column is not full.
            if (gameBoard[0][0] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the first column to be the player token.
                gameBoard[0][0] = playerToken;
            }
            // if the first column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is 2.
        else if (column == 2)
        {
            // if the second column is not full.
            if (gameBoard[0][1] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the second column to be the player token.
                gameBoard[0][1] = playerToken;
            }
            // if the second column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is 3.
        else if (column == 3)
        {
            // if the third column is not full.
            if (gameBoard[0][2] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the third column to be the player token.
                gameBoard[0][2] = playerToken;
            }
            // if the third column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is 4.
        else if (column == 4)
        {
            // if the fourth column is not full.
            if (gameBoard[0][3] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the fourth column to be the player token.
                gameBoard[0][3] = playerToken;
            }
            // if the fourth column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is 5.
        else if (column == 5)
        {
            // if the fifth column is not full.
            if (gameBoard[0][4] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the fifth column to be the player token.
                gameBoard[0][4] = playerToken;
            }
            // if the fifth column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is 6.
        else if (column == 6)
        {
            // if the sixth column is not full.
            if (gameBoard[0][5] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the sixth column to be the player token.
                gameBoard[0][5] = playerToken;
            }
            // if the sixth column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is 7.
        else if (column == 7)
        {
            // if the seventh column is not full.
            if (gameBoard[0][6] == 0)
            {
                // set the move to be 1.
                move = 1;
                // set the seventh column to be the player token.
                gameBoard[0][6] = playerToken;
            }
            // if the seventh column is full.
            else
            {
                // set the move to be 0.
                move = 0;
                // print the column is full.
                printf("Column is full");
                printf("\n");
                printf("\n");
            }
        }
        // if the column is not between 1 and 7.
        else
        {
            // set the move to be 0.
            move = 0;
            // print the column is not between 1 and 7.
            printf("Column is not between 1 and 7");
            printf("\n");
            printf("\n");
        }

        // if the move is 1.
        if (move == 1)
        {
            // if the column is 1.
            if (column == 1)
            {
                // if the first column is not full.
                if (gameBoard[0][0] == 0)
                {
                    // set the first column to be the player token.
                    gameBoard[0][0] = playerToken;
                }
                // if the first column is full.
                else
                {
                    // set the second column to be the player token.
                    gameBoard[1][0] = playerToken;
                }
            }
            // if the column is 2.
            else if (column == 2)
            {
                // if the second column is not full.
                if (gameBoard[0][1] == 0)
                {
                    // set the second column to be the player token.
                    gameBoard[0][1] = playerToken;
                }
                // if the second column is full.
                else
                {
                    // set the third column to be the player token.
                    gameBoard[1][1] = playerToken;
                }
            }
            // if the column is 3.
            else if (column == 3)
            {
                // if the third column is not full.
                if (gameBoard[0][2] == 0)
                {
                    // set the third column to be the player token.
                    gameBoard[0][2] = playerToken;
                }
                // if the third column is full.
                else
                {
                    // set the fourth column to be the player token.
                    gameBoard[1][2] = playerToken;
                }
            }
            // if the column is 4.
            else if (column == 4)
            {
                // if the fourth column is not full.
                if (gameBoard[0][3] == 0)
                {
                    // set the fourth column to be the player token.
                    gameBoard[0][3] = playerToken;
                }
                // if the fourth column is full.
                else
                {
                    // set the fifth column to be the player token.
                    gameBoard[1][3] = playerToken;
                }
            }
            // if the column is 5.
            else if (column == 5)
            {
                // if the fifth column is not full.
                if (gameBoard[0][4] == 0)
                {
                    // set the fifth column to be the player token.
                    gameBoard[0][4] = playerToken;
                }
                // if the fifth column is full.
                else
                {
                    // set the sixth column to be the player token.
                    gameBoard[1][4] = playerToken;
                }
            }
            // if the column is 6.
            else if (column == 6)
            {
                // if the sixth column is not full.
                if (gameBoard[0][5] == 0)
                {
                    // set the sixth column to be the player token.
                    gameBoard[0][5] = playerToken;
                }
                // if the sixth column is full.
                else
                {
                    // set the seventh column to be the player token.
                    gameBoard[1][5] = playerToken;
                }
            }
            // if the column is 7.
            else if (column == 7)
            {
                // if the seventh column is not full.
                if (gameBoard[0][6] == 0)
                {
                    // set the seventh column to be the player token.
                    gameBoard[0][6] = playerToken;
                }
                // if the seventh column is full.
                else
                {
                    // set the eighth column to be the player token.
                    gameBoard[1][6] = playerToken;
                }
            }
        }
    }
    // if the move is 0.
    if (move == 0)
    {
        // print the move is invalid.
        printf("Move is invalid");
        printf("\n");
        printf("\n");
    }
    // return the move.
    return move;
}

//playeMove



// int function using the miniMax algorithm and the evalute function
int miniMax(int gameBoard[6][7], int depth, int isMaximizing)
{
    int score = evaluate(gameBoard);
    if (score == 10)
    {
        return score;
    }
    if (score == -10)
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
                best = max(best, miniMax(gameBoard, depth+1, !isMaximizing));
                gameBoard[j][i] = 0;
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
                best = min(best, miniMax(gameBoard, depth+1, !isMaximizing));
                gameBoard[j][i] = 0;
            }
        }
        return best;
    }
}
// function to find the best move for the bot
int findBestMove(int gameBoard[6][7])
{
    int bestVal = -1000;
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
            gameBoard[j][i] = 2;
            int moveVal = miniMax(gameBoard, 0, 0);
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
// function to make the bot move
void botMove(int gameBoard[6][7], int playerToken)
{
    int column = findBestMove(gameBoard);
    int i = 0;
    while (gameBoard[i+1][column] == 0 && i < 5)
    {
        i++;
    }
    gameBoard[i][column] = playerToken;
    i = 0;
    display(gameBoard);
    printf("\n");
}
// function to check if there are any moves left
int isMovesLeft(int gameBoard[6][7])
{
    for (int i = 1; i < 8; i++)
    {
        if (gameBoard[0][i] == 0)
        {
            return 1;
        }
    }
    return 0;
}
// function to evaluate the board
int evaluate(int gameBoard[6][7])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 1; j < 8; j++)
        {
            if (gameBoard[i][j] == 1)
            {
                if (gameBoard[i][j+1] == 1 && gameBoard[i][j+2] == 1 && gameBoard[i][j+3] == 1)
                {
                    return -10;
                }
                if (gameBoard[i+1][j] == 1 && gameBoard[i+2][j] == 1 && gameBoard[i+3][j] == 1)
                {
                    return -10;
                }
                if (gameBoard[i+1][j+1] == 1 && gameBoard[i+2][j+2] == 1 && gameBoard[i+3][j+3] == 1)
                {
                    return -10;
                }
                if (gameBoard[i+1][j-1] == 1 && gameBoard[i+2][j-2] == 1 && gameBoard[i+3][j-3] == 1)
                {
                    return -10;
                }
            }
            if (gameBoard[i][j] == 2)
            {
                if (gameBoard[i][j+1] == 2 && gameBoard[i][j+2] == 2 && gameBoard[i][j+3] == 2)
                {
                    return 10;
                }
                if (gameBoard[i+1][j] == 2 && gameBoard[i+2][j] == 2 && gameBoard[i+3][j] == 2)
                {
                    return 10;
                }
                if (gameBoard[i+1][j+1] == 2 && gameBoard[i+2][j+2] == 2 && gameBoard[i+3][j+3] == 2)
                {
                    return 10;
                }
                if (gameBoard[i+1][j-1] == 2 && gameBoard[i+2][j-2] == 2 && gameBoard[i+3][j-3] == 2)
                {
                    return 10;
                }
            }
        }
    }
    return 0;
}

//boolean function vsPlayerOrBot to take the input if the user wants Player vs Player or Player vs computer.
int vsPlayerOrBot(){
    int choice;
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if(choice == 1){
        return 1;
    }
    else if(choice == 2){
        return 0;
    }
    else{
        printf("Invalid choice. Please enter 1 or 2\n");
        vsPlayerOrBot();
    }
}
// a function playerVsBot that functions similarly to PlayerVsPlayer but with the botMove function instead of playerMove
void playerVsBot(int gameBoard[6][7], int playerToken){
    createPlayers();
    // set the truns for the players and safe the truns in variables.
    int turn = flipCoin();
    if (flipCoin() == HEADS)
        {
            player1Start = 1;
            printf("%s is red and %s is yellow.", player1, bot);
            printf("\n");
        }
        else
        {
        
            
            botStart = 1;
            printf("%s is red and %s is yellow.", bot, player1);
            printf("\n");
        }
    // set the game to be on.
    int gameOn = 1;
    // while the game is on, the game will continue to run.
    while (gameOn == 1)
    {
        // if the turn is 1, it is player 1's turn.
        if (turn == 1)
        {
            // if player 1 starts, the player will be asked to make a move.
            if (player1Start == 1)
            {
                playerMove(gameBoard,playerToken);
                // if the player wins, the game will end.
                if (isWinner(playerToken) == 1)
                {
                    printf("%s wins!", player1);
                    printf("\n");
                    gameOn = 0;
                }
                // if the player does not win, the turn will be switched to player 2.
                else
                {
                    turn = 2;
                }
            }
            // if player 2 starts, the bot will make a move.
            else
            {
                botMove(gameBoard, playerToken);
                // if the bot wins, the game will end.
                if (isWinner(playerToken) == 1)
                {
                    printf("%s wins!", bot);
                    printf("\n");
                    gameOn = 0;
                }
                // if the bot does not win, the turn will be switched to player 2.
                else
                {
                    turn = 2;
                }
            }
        }
        // if the turn is 2, it is bot's trun.
        else
        {
            // if player 2 starts, the player will be asked to make a move.
            if (botStart == 1)
            {
                playerMove(gameBoard, playerToken);
                // if the player wins, the game will end.
                if (isWinner(playerToken) == 1)
                {
                    printf("%s wins!", player1);
                    printf("\n");
                    gameOn = 0;
                }
                // if the player does not win, the turn will be switched to player 1.
                else
                {
                    turn = 1;
                }
            }
            // if player 1 starts, the bot will make a move.
            else
            {
                botMove(gameBoard, playerToken);
                // if the bot wins, the game will end.
                if (isWinner(playerToken) == 1)
                {
                    printf("%s wins!", bot);
                    printf("\n");
                    gameOn = 0;
                }
                // if the bot does not win, the turn will be switched to player 1.
                else
                {
                    turn = 1;
                }
            }
        }

    }

}

    
//a function to start the game 
void startGame(){
    int gameBoard[6][7] = {0};
    int playerToken = 1;
    int choice = vsPlayerOrBot();
    if(choice == 1){
        while (1)
        {
            playerVsPlayer(gameBoard, playerToken);
            if (isWinner(playerToken) == 0)
            {
                playerToken = (playerToken == 1) ? 2 : 1;
            }
            else
            {
                printf("Player %d wins !\n", playerToken);
                exit(0);
            }
        }
    }
    else if(choice == 0){
        while (1)
        {
            playerVsBot(gameBoard,playerToken);
            if (isWinner(playerToken) == 0)
            {
                playerToken = (playerToken == 1) ? 2 : 1;
            }
            else
            {
                printf("Player %d wins !\n", playerToken);
                exit(0);
            }
        }
    }
}
