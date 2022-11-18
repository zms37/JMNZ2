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
int player1Start = 0;
int player2Start = 0;

//int botStart = 0;


clock_t startPlayer;
clock_t endPlayer;
double playerTime =0;
double player1Time = 0;
double player2Time = 0;

coin flipCoin();
void display();
bool isWinner();
void startGame();
void playerVsPlayer();
void botMove (int gameBoard[6][7], int token);
int evaluate(int gameBoard[6][7], int token);
int minimax(int gameBoard[6][7], int depth, bool isMaximizing, int playerToken);
bool vsPlayerOrBot();
void playerVsBot();
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
bool isWinner(int playerToken)
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
 if(vsPlayerOrBot){  

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
void startGame(){
    /*
    Requires: No Direct Parameters to the function, but the global variables player 1 and player 2, representing both players' names, are required.
    
    Effects: The players are created according to their inputed names. A coin flip determines which player starts, and their tokens are assigned accordingly. 
    The players make their moves according to their respective turns, and the winner is announced if the conditions of the isWinner function are satisfied. If 
    such conditions are not satisfied and the board is full, the tie function is called, comparing the times of the players, and selects a winner accordingly.
    */

    //use vsPlayerOrBot to determine which playing mood 
    if(vsPlayerOrBot){
        // create the players and their names and the tokins to be 1 for player 1 and 2 for player 2.
        createPlayers();
        //determine which turn is it.
        int turn = 0;
        int moves = 0;
        // set the game to be on.
        int gameOn = 1;
        //player 1 and player 2 time.

        int move = 0;
        // set the truns for the players and save the truns in variables.
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
    else{
        // create the players and their names and the tokins to be 1 for player 1 and 2 for player 2.
        createPlayers();
        //determine which turn is it.
        int turn = 0;
        int moves = 0;
        int botStart=0;
        // set the game to be on.
        int gameOn = 1;
        //player 1 and player 2 time.

        int move = 0;
        // set the truns for the players and save the truns in variables.
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
        
            botStart = 1;
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

                botMove(gameBoard,player2Token);
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
                botMove(gameBoard);
                moves++;
                player2Time+=playerTime;
                playerTime =0
                // set the player 2 turn to be false.
                //player2Turn = 0;
                // set the player 1 turn to be true.
                //player1Turn = 1;
                if (isWinner(1))
                {
                    printf("%s won!!!!!!!\n", player2);
                    break;
                }
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
int minimax(int gameBoard[6][7], int depth, bool isMaximizing, int playerToken) {
    bool winner = isWinner(playerToken);
    if (winner != 0) 
        return evaluate(gameBoard,playerToken); //score of player token
    if(isMaximizing){ 
        int score = -100000;
        for(int i = 0; i < 6; ++i) {
            for(int j = 0; j < 7; j++){
                if (gameBoard[i][j] == 0) {
                    gameBoard[i][j] = playerToken;
                    int thisScore = minimax(gameBoard, depth +1, 0, playerToken);
                    gameBoard[i][j] = 0;
                    if (thisScore > score) {
                        score = thisScore;
                    }
                }
            }
        }
    return score;
    }
    else{
        int score = 100000;
        for(int i = 0; i < 6; ++i) {
            for(int j = 0; j < 7; j++){
                if (gameBoard[i][j] == 0) {
                    gameBoard[i][j] = playerToken;
                    int thisScore = minimax(gameBoard, depth +1, 1, playerToken);
                    gameBoard[i][j] = 0;
                    if (thisScore > score) {
                        score = thisScore;
                    }
                }
            }
        }
    return score;
    }
}

void botMove (int gameBoard[6][7], int token){
    int bestmoveCol;
    int bestmoveRow;
    int score = -1000000;
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 7; j++){
            if (gameBoard[i][j] == 0) {
            gameBoard[i][j] = token;
            int tempScore = minimax(gameBoard, 0, 0, token);
            gameBoard[i][j] = 0;   
            if (tempScore > score) {
                score = tempScore;
                bestmoveCol = i;
                bestmoveRow = j;
            }
            }
        }
    }
    gameBoard[bestmoveCol][bestmoveRow] = token;

    display(gameBoard);
}

//function to evalute the best move to be choosen by the bot using the minimax 
//algorithm
int evaluate(int gameBoard[6][7], int token){
    int score = 0;
    //score center column
    int centerArray[7] = {3, 4, 5, 5, 4, 3};
    int centerCount = 0;
    for(int i = 0; i < 6; ++i) {
        if (gameBoard[i][3] == token) {
            score += centerArray[i];
            centerCount++;
        }
        else if (gameBoard[i][3] == token) {
            score -= centerArray[i];
            centerCount++;
        }
    }
    //score horizontal
    for(int i = 0; i < 6; ++i) {
        int tokenCount = 0;
        for(int j = 0; j < 7; j++){
            if (gameBoard[i][j] == token) {
                tokenCount++;
            }
            else if (gameBoard[i][j] == token) {
                tokenCount--;
            }
        }
        score += tokenCount;
    }
    //score vertical
    for(int i = 0; i < 7; ++i) {
        int tokenCount = 0;
        for(int j = 0; j < 6; j++){
            if (gameBoard[j][i] == token) {
                tokenCount++;
            }
            else if (gameBoard[j][i] == token) {
                tokenCount--;
            }
        }
        score += tokenCount;
    }
    //score positive sloped diagonal
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 4; j++){
            int tokenCount = 0;
            for(int k = 0; k < 4; k++){
                if (gameBoard[i+k][j+k] == token) {
                    tokenCount++;
                }
                else if (gameBoard[i+k][j+k] == token) {
                    tokenCount--;
                }
            }
            score += tokenCount;
        }
    }
    //score negative sloped diagonal
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 4; j++){
            int tokenCount = 0;
            for(int k = 0; k < 4; k++){
                if (gameBoard[i+k][j-k] == token) {
                    tokenCount++;
                }
                else if (gameBoard[i+k][j-k] == token) {
                    tokenCount--;
                }
            }
            score += tokenCount;
        }
    }
    return score;
}
//boolean function vsPlayerOrBot to take the input if the user wants Player vs Player or Player vs computer.
bool vsPlayerOrBot(){
    int choice;
    printf("1. Player vs Player\n");
    printf("2. Player vs Computer\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if(choice == 1){
        return true;
    }
    else if(choice == 2){
        return false;
    }
    else{
        printf("Invalid choice. Please enter 1 or 2\n");
        vsPlayerOrBot();
    }
}
// a function playerVsBot that functions similarly to PlayerVsPlayer.
void playerVsBot(int gameBoard[6][7], int playerToken){
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


