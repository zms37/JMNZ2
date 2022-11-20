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
int scoreArray[64];
int player1Start = 0;
int player2Start = 0;
int botStart = 0;
int playerOrBot = 0;
int playerToken = 0;
int botToken = 0;
int MAX = 1000;
int MIN = -1000;
int levelVal = 0;
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
bool isWinner();
void startGame();
void playerVsPlayer();
void botMove (int gameBoard[6][7], int token);
int evaluate(int gameBoard[6][7]);
int miniMax(int gameBoard[6][7], int depth, int isMax, int alpha, int beta);
int vsPlayerOrBot();
int level();
void playerVsBot();
int isMovesLeft(int gameBoard[6][7]);
int easyMove(int gameBoard[6][7]);
int normalMove(int gameBoard[6][7]);
int hardMove(int gameBoard[6][7]);
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
/*
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
bool isWinner(int playerToken)
/*
    Requires: The token number that is assinged to the playerAndBot
    Effects: The gameboard is updated within every move made by each playerAndBot. Simultaneously, we continously check if 4 tokens of the same playerAndBot allign vertically, horizontally,
    or diagonally. If one of these conditions is satisfied, we return true, signafying that the playerAndBot of the inputed token is the winner. False otherwise.
    
    With PlayerToken = 1 or PlayerToken = 2: 

    Partition on vertical column v: Success
    Partition on horizontal row h: Success
    Partition on upperward diagonal from column 4 to 7: Failure
    Partition on downward diagonal from column 4 to 7: Failure
    Partition on upperward diagonal from column 1 to 4: Success
    Partition on downward diagonal from column 2 to 5: Success

    PlayerToken != 1 && != 2:
    Failure On All Partitions

    | v |   | h | h | h | h |   |
    | v |   |   | ud|   |   |   |
    | v | dd| ud| dd|   |   | ud|
    | v | ud| dd|   | dd| ud|   |
    | ud|   |   | dd| ud| dd|   |
    |   |   |   | ud| dd|   | dd|
   
    Requires: The token number that is assinged to the player
    Effects: The gameboard is updated within every move made by each player. Simultaneously, we continously check if 4 tokens of the same player allign vertically, horizontally,
    or diagonally. If one of these conditions is satisfied, we return true, signifying that the player of the inputed token is the winner. False otherwise.
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
                return true;
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
                return true;
            }
        }
        count = 0;
    }
    //positively sloped diagonal
    for(int i = 0 ; i < 5 ; i++){
        for(int n = 0 ; n < 4 ; n++){
            if(gameBoard[n][i] == playerToken && gameBoard[n+1][i+1] == playerToken && gameBoard[n+2][i+2] == playerToken && gameBoard[n+3][i+3] == playerToken){
                return true;
            }
        }
    }

    //negatively sloped diagonal
    for(int i = 0 ; i < 5 ; i++){
        for(int n = 3 ; n < 7 ; n++){
            if(gameBoard[n][i] == playerToken && gameBoard[n-1][i+1] == playerToken && gameBoard[n-2][i+2] == playerToken &&gameBoard[n-3][i+3] == playerToken){
				return true;
            }
        }
    }
    return false;

}

// function tie to determine who is the winner if the game is a tie, based on the players time.
void tie(double player1Time, double player2Time) 
{
    /*
    Requires: player1Timer and player2Timer that record the total time taken for players 1 and 2 to play their move.
    
    Effects: the method declares the winner by comparing the total time consumed by both players and prints the player's name that took the least time selecting their moves (If no more tokens remain). 
    In case both durations are equal, the game declares a tie.

    Testing Strategy: covers the comparison of playerAndBot times
    Partition on player1Time less than player2Time: playerAndBot 1 is the winner: Success
    Partition on player1Time greater than player2Time: playerAndBot 2 is the winner: Success
    Partition on player1Time equal to player2Time: unsolved tie: Success

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
// function to create the players and their names and the tokens to be 1 for player 1 and 2 for player 2.
void createPlayers(){
/*
    Requires: no paramters required by the method
    Effects: If the variable playerOrBot equals 1, then this method outputs the names of the first and second players, prints who is player 1 and player 2, and assigns token 1 for player 1 and token 2 for player 2. Otherwise, this method only takes the name of player 1 and prints the player's name as player 1.
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
void startGame(){
    /*
    Requires: No Direct Parameters to the function, but the global variables playerOrBot and player 1 and player 2, representing both players' names, are required.
    
    Effects: The function vsPlayerOrBot is called and its return value is assigned to the global variable playerOrBot to determine which playing mode is desired. 
    If player1 v player2 mode is selected, then the players are created according to their inputed names. A coin flip determines which player starts, and their 
    tokens are assigned accordingly. The players make their moves according to their respective turns, and the winner is announced if the conditions of the 
    isWinner function are satisfied. If such conditions are not satisfied and the board is full, the tie function is called, comparing the times of the 
    players, and selects a winner accordingly.
    If player1 v bot mode is selected, then player1 is created according to its inputed name as well as the bot. A coin flip determines whether the player or the bot starts, and their 
    tokens are assigned accordingly. The players make their moves according to their respective turns, calling the playerVsPlayer and the botMove functions, and the winner is announced if the conditions of the 
    isWinner function are satisfied. If such conditions are not satisfied and the board is full, the tie function is called, comparing the times of the 
    player and the bot, and selects a winner accordingly. 
    */
   
   playerOrBot = vsPlayerOrBot();
    //use vsPlayerOrBot to determine which playing mode 
    if(playerOrBot == 1){
        // create the players and their names and the tokens to be 1 for player 1 and 2 for player 2.
        createPlayers();
        //determine which turn is it.
        int turn = 0;
        int moves = 0;
        // set the game to be on.
        int gameOn = 1;
        //player 1 and player 2 time.
        int move = 0;
        // set the turns for the players and save the turns in variables.
        srand(time(NULL));
        flipCoin();
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
        display(gameBoard);
        // while the game is on.
        while (gameOn == 1)
        {
            // if the player 1 turn is true.
            if (player1Start == 1)
            {
                playerVsPlayer(player1, 1);
                moves++;
                player1Time +=playerTime;
                playerTime =0;
                // set the player 1 turn to be false.
                //player1Turn = 0;
                // set the player 2 turn to be true.
                //player2Turn = 1;
                if (isWinner(1))
                {
                    printf("%s won!!!!!!!\n", player1);
                    break;
                }

                //botMove(gameBoard,playerToken);
                playerVsPlayer(player2, 2);
                moves++;
                player2Time+=playerTime;
                playerTime =0;
                // set the player 2 turn to be false.
                //player2Turn = 0;
                // set the player 1 turn to be true.
                //player1Turn = 1;
                if (isWinner(2))
                {
                    printf("%s won!!!!!!!\n", player2);
                    break;
                }
            }
            
            // if the player 2 turn is true.
            else if (player2Start == 1)
            {
            // botMove(gameBoard);
                playerVsPlayer(player2, 1);
                moves++;
                player2Time+=playerTime;
                playerTime =0;
                // set the player 2 turn to be false.
                //player2Turn = 0;
                // set the player 1 turn to be true.
                //player1Turn = 1;
                if (isWinner(1))
                {
                    printf("%s won!!!!!!!\n", player2);
                    break;
                }
                playerVsPlayer(player1, 2);
                moves++;
                player1Time +=playerTime;
                playerTime =0;
                // set the player 1 turn to be false.
                //player1Turn = 0;
                // set the player 2 turn to be true.
                //player2Turn = 1;
                if (isWinner(2))
                {
                    printf("%s won!!!!!!!\n", player1);
                    break;
                }
            }
            // if the game is a tie.
            if (moves == 42)
            {
                // set the winner using the tie function and declearing the tie.
                tie(player1Time, player2Time);
                // set the game to be off.
                gameOn = 0;
            }
        }
    }
    else{ //the game between the player and the bot, the code implement playerVsBot.
        createPlayers();
        //determine which turn is it.
        int turn = 0;
        int moves = 0;
        // set the game to be on.
        int gameOn = 1;
        int move = 0;
        // set the truns for the players and save the truns in variables.
        srand(time(NULL));
        flipCoin();
        if (flipCoin() == HEADS)
        {
            player1Start = 1;
            printf("%s is red and %s is yellow.\n", player1, bot);
            playerToken = 1;
            botToken =2;
            printf("%s token is 1, the bot token is 1.\n",player1);
            printf("\n");
        }
        else
        {
            botStart = 1;
            printf("%s is red and %s is yellow.\n", bot, player1);
            playerToken = 2;
            botToken =1;
            printf("%s token is 2, the bot token is 1.\n",player1);
            printf("\n");
        }
        display(gameBoard);
        // while the game is on.
        while (gameOn == 1)
        {
            // if the player 1 turn is true.
            if (player1Start == 1)
            {
                playerVsPlayer(player1, playerToken);
                moves++;
                player1Time +=playerTime;
                playerTime =0;
                // set the player 1 turn to be false.
                //player1Turn = 0;
                // set the player 2 turn to be true.
                //player2Turn = 1;
                if (isWinner(1))
                {
                    printf("%s won!!!!!!!\n", player1);
                    break;
                }

                botMove(gameBoard,botToken);
                moves++;
                player2Time+=playerTime;
                playerTime =0;
                // set the player 2 turn to be false.
                //player2Turn = 0;
                // set the player 1 turn to be true.
                //player1Turn = 1;
                if (isWinner(2))
                {
                    printf("%s won!!!!!!!\n", bot);
                    break;
                }
            }
            // if the player 2 turn is true.
            else if (botStart == 1)
            {
            // botMove(gameBoard);
                botMove(gameBoard,botToken);
                moves++;
                player2Time+=playerTime;
                playerTime =0;
                // set the player 2 turn to be false.
                //player2Turn = 0;
                // set the player 1 turn to be true.
                //player1Turn = 1;
                if (isWinner(1))
                {
                    printf("%s won!!!!!!!\n", bot);
                    break;
                }
            }
            playerVsPlayer(player1, playerToken);
                moves++;
                player1Time +=playerTime;
                playerTime =0;
                // set the player 1 turn to be false.
                //player1Turn = 0;
                // set the player 2 turn to be true.
                //player2Turn = 1;
                if (isWinner(2))
                {
                    printf("%s won!!!!!!!\n", player1);
                    break;
                }
        }
    }   
}

//function for players turn:
void playerVsPlayer(char player[10], int playerToken)
    /* 
    Requires: An array of chars of size 10 (String) to represent the name of the player, and the player token assigned to player 1 or player 2 that indicates the
    token number 1 or 2
    Effects: 
    We call the player to select the column number he wants to insert his token in. As the player selects his move, the time taken by him to input his move is measured.
    If the column selected by the player is out of the bounds of the array, he is asked to select another move within the proper bounds. The gameboard is updated and
    displayed to the user.

    Testing Strategy: covers the playerAndBot's move and the updated gameboard
    Partition on name:
        a- of size greater than 10: Failed
        b- of size = 10: Success
        c- of size < 10: Success
    Partition on playerToken:
        a- playerToken = 1: 1 is displayed on the gameboard: Success (for playerAndBot 1)
        b- playerToken = 2: 2 is displayed on the gameboard: Success (for playerAndBot 2)
        c- playerToken = any real number: int and char: Success
        d- playerToken =  double and float: Fail
    */
{
    int column;
    int i = 0;
    do
    {
        printf("\n%s: Select a column(1-7): ", player);
        startPlayer =clock();
        scanf("%d", &column);
        endPlayer = clock();
        playerTime += (double)(endPlayer-startPlayer)/CLOCKS_PER_SEC;
        if (column < 1 || column > 7 || !column)
        {
            printf("You have to enter a number between 1 and 7 !\n");
            fgetc(stdin);
            continue;
        }
       
        else if (gameBoard[0][column] != 0)
        {
            printf("Column %d is full !\n", column);
        }
    } while (column < 1 || column > 7 || gameBoard[0][column] != 0 || !column);

    while (gameBoard[i+1][column] == 0 && i < 5)
    {
        i++;
    }

    gameBoard[i][column] = playerToken;
    i = 0;
    display(gameBoard);
    printf("\n");
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
    isMaximizing = isMax(scoreArray, score);
    if (isMaximizing == 1 )
    {
        int best = -100000;
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
                best = max(best, miniMax(gameBoard, depth+1, !isMaximizing , alpha, beta));
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
        int best = 100000;
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
                best = min(best, miniMax(gameBoard, depth+1, !isMaximizing , alpha, beta));
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
int isMax(int scoreArray[7], int score)
{/*
Requires: scoreArray of size 7 and a score integer
Effects: This function takes the scoreArray and the score and checks if the score is the max score.

Testing strategy: covers the size of the scoreArray and the integer score.
Partition on an empty board of size 7: Success
Partition on a filled in board of size 7: Success
Partition on negatively sized board: int gameBoard[-1][2]: Failed
Partition on an unspecified sized board: Failed
Partition on different sized board:
        a- Size is less than the required input array size (size = 5): Failed
        b- Size is greater than the required input array size (size = 9): Success (board is displayed but elements that are out of range are not displayed)
Partition on: Positive/ negative integer or char: Success
Partition on: Positive/ negative float or double: Fail
*/
    for (int i = 0; i < 7; i++)
    {
        if (scoreArray[i] == score)
        {
            return 1;
        }
    }
    return 0;
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
            gameBoard[j][i] = 1;
            int moveVal = miniMax(gameBoard, 0, 0, 500, -500);
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
// function easy that finds the third best move for the bot.
int easyMove(int gameBoard[6][7])
{
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
    Partition on an unpecified sized board: Failed
    */
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
            gameBoard[j][i] = 1;
            int moveVal = miniMax(gameBoard, 0, 0, 100, -100);
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
void botMove(int gameBoard[6][7], int botToken)
{
    /* 
    Requires: 2-D gameBoard array and the botToken to update the gameBoard with the bot move.
    Effects: If the selected mode is easy (1), the bot places its token in the column number that is taken from the easyMove function and prints the updated gameBoard.
    If the selected mode is normal (2), the bot places its token in the column number that is taken from the normalMove function and prints the updated gameBoard.
    If the selected mode is hard (3), the bot places its token in the column number that is taken from the hardMove function and prints the updated gameBoard.

    Testing Strategy: covers the Bot's move and the updated gameboard
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unpecified sized board: Failed
    Partition on botToken:
        a- botToken = 1: 1 is displayed on the gameboard: Success (for playerAndBot 1)
        b- botToken = 2: 2 is displayed on the gameboard: Success (for playerAndBot 2)
        c- botToken = any real number: int and char: Success
        d- botToken =  double and float: Fail
    */
    if (levelVal=1){
        int column = easyMove(gameBoard);
        int i = 0;
        while (gameBoard[i+1][column] == 0 && i < 5)
        {
            i++;
        }
        gameBoard[i][column] = botToken;
        i = 0;
        display(gameBoard);
        printf("\n");
    }
    else if (levelVal=2){
        int column = normalMove(gameBoard);
        int i = 0;
        while (gameBoard[i+1][column] == 0 && i < 5)
        {
            i++;
        }
        gameBoard[i][column] = botToken;
        i = 0;
        display(gameBoard);
        printf("\n");
    }
    else if (levelVal=3){
        int column = hardMove(gameBoard);
        int i = 0;
        while (gameBoard[i+1][column] == 0 && i < 5)
        {
            i++;
        }
        gameBoard[i][column] = botToken;
        i = 0;
        display(gameBoard);
        printf("\n");
    }
}
// function to check if there are any moves left
int isMovesLeft(int gameBoard[6][7])
{
    /* 
    Requires: 2-D array gameBoard 
    Effects: returns 1 if there are any moves left in the game or 0 otherwise.

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

    int score = 0;
     for (int i = 0; i < 6; i++)
     {
        for (int j = 1; j < 8; j++)
        {
            if (gameBoard[i][j] == playerToken)
            {
                if (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken && gameBoard[i][j+3] == playerToken)
                {
                    score += 5;
                }
                if (gameBoard[i+1][j] == playerToken && gameBoard[i+2][j] == playerToken && gameBoard[i+3][j] == playerToken)
                {
                    score += 5;
                }
                if (gameBoard[i+1][j+1] == playerToken && gameBoard[i+2][j+2] == playerToken && gameBoard[i+3][j+3] == playerToken)
                {
                    score += 5;
                }
                if (gameBoard[i+1][j-1] == playerToken && gameBoard[i+2][j-2] == playerToken && gameBoard[i+3][j-3] == playerToken)
                {
                    score += 5;
                }
                if (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken)
                {
                    score += 2;
                }
                if (gameBoard[i+1][j] == playerToken && gameBoard[i+2][j] == playerToken)
                {
                    score += 2;
                }
                if (gameBoard[i+1][j+1] == playerToken && gameBoard[i+2][j+2] == playerToken)
                {
                    score += 2;
                }
                if (gameBoard[i+1][j-1] == playerToken && gameBoard[i+2][j-2] == playerToken)
                {
                    score += 2;
                }
                if (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken)
                {
                    score += 2;
                }
                if (gameBoard[i+1][j] == playerToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j+1] == playerToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j-1] == playerToken)
                {
                    score += 1;
                }
            }
            if (gameBoard[i][j] == botToken)
            {
                if (gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken && gameBoard[i][j+3] == botToken)
                {
                    score += 3;
                }
                if (gameBoard[i+1][j] == botToken && gameBoard[i+2][j] == botToken && gameBoard[i+3][j] == botToken)
                {
                    score += 3;
                }
                if (gameBoard[i+1][j+1] == botToken && gameBoard[i+2][j+2] == botToken && gameBoard[i+3][j+3] == botToken)
                {
                    score += 3;
                }
                if (gameBoard[i+1][j-1] == botToken && gameBoard[i+2][j-2] == botToken && gameBoard[i+3][j-3] == botToken)
                {
                    score += 3;
                }
                if (gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j] == botToken && gameBoard[i+2][j] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j+1] == botToken && gameBoard[i+2][j+2] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j-1] == botToken && gameBoard[i+2][j-2] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j+1] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j-1] == botToken)
                {
                    score += 1;
                }
            }
            if (gameBoard[i][j] == playerToken || gameBoard[i][j])
            {
                if (gameBoard[i][j+1] == playerToken || gameBoard[i][j+1] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j] == playerToken || gameBoard[i+1][j] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j+1] == playerToken || gameBoard[i+1][j+1] == botToken)
                {
                    score += 1;
                }
                if (gameBoard[i+1][j-1] == playerToken || gameBoard[i+1][j-1] == botToken)
                {
                    score += 1;
                }
                if (!(gameBoard[i][j+1] == playerToken || gameBoard[i][j+1] == botToken))
                {
                    score += -5;
                }
                if (!(gameBoard[i+1][j] == playerToken || gameBoard[i+1][j] == botToken))
                {
                    score += -5;
                }
                if (!(gameBoard[i+1][j+1] == playerToken || gameBoard[i+1][j+1] == botToken))
                {
                    score += -5;
                }
                if (!(gameBoard[i+1][j-1] == playerToken || gameBoard[i+1][j-1] == botToken))
                {
                    score += -5;
                }
                if ((gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken) || (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken))
                {
                    score += 1;
                }
                if ((gameBoard[i+1][j] == botToken && gameBoard[i+2][j] == botToken) || (gameBoard[i+1][j] == playerToken && gameBoard[i+2][j] == playerToken))
                {
                    score += 1;
                }
                if ((gameBoard[i+1][j+1] == botToken && gameBoard[i+2][j+2] == botToken) || (gameBoard[i+1][j+1] == playerToken && gameBoard[i+2][j+2] == playerToken))
                {
                    score += 1;
                }
                if ((gameBoard[i+1][j-1] == botToken && gameBoard[i+2][j-2] == botToken) || (gameBoard[i+1][j-1] == playerToken && gameBoard[i+2][j-2] == playerToken))
                {
                    score += 1;
                }
                 if (!((gameBoard[i][j+1] == botToken && gameBoard[i][j+2] == botToken) || (gameBoard[i][j+1] == playerToken && gameBoard[i][j+2] == playerToken)))
                {
                    score += -10;
                }
                if (!((gameBoard[i+1][j] == botToken && gameBoard[i+2][j] == botToken) || (gameBoard[i+1][j] == playerToken && gameBoard[i+2][j] == playerToken)))
                {
                    score += -10;
                }
                if (!((gameBoard[i+1][j+1] == botToken && gameBoard[i+2][j+2] == botToken) || (gameBoard[i+1][j+1] == playerToken && gameBoard[i+2][j+2] == playerToken)))
                {
                    score += -10;
                }
                if (!((gameBoard[i+1][j-1] == botToken && gameBoard[i+2][j-2] == botToken) || (gameBoard[i+1][j-1] == playerToken && gameBoard[i+2][j-2] == playerToken)))
                {
                    score += -10;
                } 
            }
        }
       // scoreArray[i]=score;
    }
    return score;
}
//int levelVal a function that asks the user what levelVal they want to play, Esay is 1, Normal is 2, Hard is 3.
int level()
{
    /* 
    Requires: no parameters
    Effects: returns the player's choice of selecting the easy, normal, or hard level.
    */ 
    int choice;
    printf("Please select a level: \n");
    printf("1. Easy\n");
    printf("2. Normal\n");
    printf("3. Hard\n");
    scanf("%d", &choice);
    return choice;
}
//boolean function vsPlayerOrBot to take the input if the user wants Player vs Player or Player vs computer.
int vsPlayerOrBot(){
    /* 
    Requires: no parameters
    Effects: returns the player's choice of selecting to either play versus another player or the bot.
    */
    int choice;
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if(choice == 1){
        return 1;
    }
    else if(choice == 2){
        int n = level();
        levelVal = n;
        return 0;
    }
    else{
        printf("Invalid choice. Please enter 1 or 2\n");
        vsPlayerOrBot();
    }
}
// a function playerVsBot that functions similarly to PlayerVsPlayer.
void playerVsBot(int gameBoard[6][7], int playerToken){
    /* 
    Requires: The 2-D array gameBoard, and the player token assigned to player 1 that indicates the token number 1 or 2.
    Effects: 
    We call the player to select the column number he wants to insert his token in. If the column selected by the player is out of the bounds of the array, 
    he is asked to select another move within the proper bounds. Then, the botMove funtction is called to display the bot's move on the gameBoard. 
    The gameboard is updated and displayed to the user. At the end, the function declares the winner of the game.

    Testing Strategy: covers the playerAndBot's move and the updated gameboard
    Partition on an empty board of size row: 6, column: 7 (filled with zeros): Success
    Partition on a filled in board of size row: 6, column: 7 (filled with non zero elements): Success 
    Partition on different sized board:
        a- Size is less than the required input array size (row: < 6, column < 7): int gameBoard[2][3]: Failed
        b- Size is greater than the required input array size (row: > 7, column > 8): int gameBoard[10][10]: Success (board is displayed but elements that are in rows > 6 , columns > 7 are not displayed)
    Partition on negatively sized board: int gameBoard[-1][2]: Failed
    Partition on an unpecified sized board: Failed
    Partition on playerToken:
        a- playerToken = 1: 1 is displayed on the gameboard: Success (for playerAndBot 1)
        b- playerToken = 2: 2 is displayed on the gameboard: Success (for playerAndBot 2)
        c- playerToken = any real number: int and char: Success
        d- playerToken =  double and float: Fail
    */
    //printf("Player %d's turn\n", playerToken);
    int column;
    int i = 0;
    do
    {
        printf("Player %d, enter a column: ", playerToken);
        scanf("%d", &column);
        if (column < 1 || column > 7)
        {
            printf("You have to enter a number between 1 and 7 !\n");
            fgetc(stdin);
            continue;
        }
       
        else if (gameBoard[0][column] != 0)
        {
            printf("Column %d is full !\n", column);
        }
    } while (column < 1 || column > 7 || gameBoard[0][column] != 0 || !column);

    while (gameBoard[i+1][column] == 0 && i < 5)
    {
        i++;
    }

    gameBoard[i][column] = playerToken;
    i = 0;
    display(gameBoard);
    printf("\n");
    if(isWinner(playerToken) == 0){
        botMove(gameBoard, playerToken);
    }
    else{
        printf("Player %d wins !\n", playerToken);
        exit(0);
    }
}
