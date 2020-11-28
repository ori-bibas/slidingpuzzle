/*
  Ori Bibas
  Number Slider Game
  Written in C
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "slidingFunctions.h"

int main(){

    srand(time(NULL));

    // SpacePosition pointer will keep track of where the space is on the game board for the entirity of the program.
    SpacePosition* space = malloc(sizeof(SpacePosition));
    space->i = -1;
    space->j = -1;

    printf("\n          ---------- Numbers Puzzle Game ----------\n\n");
    printf("Your goal is to continually move pieces until the game board \nis in the correct order. Once it is, you win and the game will end!\n\n");
    printf("Please choose your difficulty by typing easy, medium, or hard.\nEasy will be a 3x3 board, medium will be 4x4, and hard is a 5x5.\n\n");

    int flag = 0;
    int length;

    while(flag != 1){

      char str[20];
      printf("Enter your difficuly: ");
      scanf("%s", str);

      if(strcmp(str, "easy") == 0){
        length = 3;
        flag = 1;
      }
      else if(strcmp(str, "medium") == 0){
        length = 4;
        flag = 1;
      }
      else if(strcmp(str, "hard") == 0){
        length = 5;
        flag = 1;
      }
      else{
        printf("Your entry was not valid, please either type in easy, medium, or hard.\n\n");
      }
    }

    Element** finalBoard = createFinalBoard(length);

    Element** gameBoard = createGameBoard(length, space);

    // Will continually check if the game board is in an initial solvable state, if not, will re initialize a new one until it is solvable.
    while(checkIfSolvable(gameBoard, length) == 0){
      for(int i = 0; i < length; i++){
        free(gameBoard[i]);
      }
      free(gameBoard);
      gameBoard = createGameBoard(length, space);
    }

    printf("\n\n\n");

    printf("Your solvable game board is set, good luck!\n\n\n\n\n\n\n\n\n");

    printf("\n\n\n");

    printBoard(gameBoard, length);

    while(1)
    {
        int tempChoice;
        printf("\nWhat number would you like to move: ");
        scanf("%d", &tempChoice);

        // Check if move is valid, if so, "slide" the piece by swapping it with the space.
        if(validateMove(gameBoard, length,space, tempChoice)){
            swap(gameBoard, length, tempChoice, space);
        }
        else{
            printf("That is an invalid move, please try again.\n\n");
            continue;
        }
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printBoard(gameBoard, length);

        // After every iteration, board needs to be checked for completion.
        if(checkDone(finalBoard, gameBoard, length)){
          printf("\n\nGreat job, you won!!\n\n");
          break;
        }

    }

    // Free all data
    for(int i = 0; i < length; i++){
      free(gameBoard[i]);
    }
    free(gameBoard);

    for(int i = 0; i < length; i++){
      free(finalBoard[i]);
    }
    free(finalBoard);

    free(space);

    return 0;
}
