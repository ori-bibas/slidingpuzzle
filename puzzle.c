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

/*
  Structure with 2 integers used because
  values cover the ASCII number range, so differentiating
  between a space and an integer can be a challenge with just
  one number.
*/
typedef struct element{
    int value;
    int isSpace; // 1 indiciates that this element is a space, 0 otherwise
}Element;

/*
  Will store the indexes of
  the space in the matrix.
*/
typedef struct spacePosition{
    int i;
    int j;
}SpacePosition;

Element** createGameBoard(int length, SpacePosition* space);
int checkIfSolvable(Element** gameBoard, int length);
Element** createFinalBoard(int length);
int checkDone(Element** finalBoard, Element** gameBoard, int length);
void shuffle(Element* array, int length);
void printBoard(Element** arr, int length);
void swap(Element** arr, int length, int num, SpacePosition* space);
int validateMove(Element** arr, int length, SpacePosition* space, int num);

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

// Create final board returns a 2D array of elements, and is identical to what the
// board at its completion should look like. Used to check against current player's board.
Element** createFinalBoard(int length){

  Element** finalBoard = malloc(sizeof(Element*) * length);
  for(int i = 0; i < length; i++){
    finalBoard[i] = malloc(sizeof(Element) * length);
  }

  // Fill the board in ascending order from 1 to length - 1
  int count = 1;
  for(int i = 0; i < length; i++){
    for(int j = 0; j < length; j++){
      finalBoard[i][j].value = count;
      count++;
    }
  }

  return finalBoard;
}

// checkIfSolvable function returns 1 if the gameBoard is in an initial solvable state, or 0 otherwise.
int checkIfSolvable(Element** gameBoard, int length){
  
  // Created an integer array of able to fit all elements in, and filled it up with the array's current elements.
  int temporary[length*length];
  int index = 0;
  for(int i = 0; i < length; i++){
    for(int j = 0; j < length; j++){
      Element temp = gameBoard[i][j];
      if(temp.isSpace == 0){
        temporary[index] = temp.value;
        index++;
      }
      else{
        temporary[index] = temp.isSpace;
        index++;
      }
    }
  }

  int len = index + 1;
  int inversions = 0;

  // Check for how many inversions there are
  for(int i = 0; i < index - 1; i++){
    for(int j = i+1; j < index; j++){
      if(temporary[i] && temporary[j] && temporary[i] > temporary[j]){
        inversions++;
      }
    }
  }

  // If the inversion count is even, the puzzle is in an initial solvable state.
  if(inversions % 2 == 1){
    return 0;
  }
  else{
    return 1;
  }
}

// createGameBoard function returns a 2D game board of size length * length, and fills it with the correct amount of numbers
// in a random order, while also putting a space inside the puzzle and keeping track of it's location.
Element** createGameBoard(int length, SpacePosition* space){

    /*
      Allocates memory for a square gameboard (length x length) , that holds
      Element structures indicating the value of the square, and whether or not
      that square is a space
    */

    Element** temp = malloc(sizeof(Element*) * length);
    for(int i = 0; i < length; i++){
        temp[i] = malloc(sizeof(Element) * length);
    } 

    /*
      Array for the elements that will be fed into the 2-D array game board,
      setting the values 1 - (range - 1) , and the last element will be a space.
      Needs to be shuffled before it is inserted into the game board.
    */
    int range = (length * length);
    Element* arr = malloc(sizeof(Element) * range);

    for(int i = 0; i < range - 1; i++){

        arr[i].value = i + 1;
        arr[i].isSpace = 0;

    }

    arr[range-1].value = 999;
    arr[range-1].isSpace = 1;

    // Shuffle the array with the range values, to be able to insert into 2D array in a random order.
    shuffle(arr, range);

    // Read in the shuffled array into the game board
    int count = 0;
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length; j++){
            Element tempNum = arr[count];
            temp[i][j] = tempNum;
            count++;

            if(tempNum.isSpace == 1){
                space->i = i;
                space->j = j;
            }

        }
    }
    
    return temp;
}

// shuffle() function takes an array of elements and randomly shuffles them
void shuffle(Element* array, int length){
    if (length > 1)
    {
        int i;
        for (i = 0; i < length - 1; i++)
        {
          int j = i + rand() / (RAND_MAX / (length - i) + 1);
          Element temp = array[j];
          array[j] = array[i];
          array[i] = temp;
        }
    }
}

// printBoard utility function prints out the game board.
void printBoard(Element** arr, int length){

    for(int i = 0; i < length; i++){
        for(int j = 0; j < length; j++){

            Element temp = arr[i][j];

            if(temp.isSpace == 1){
                printf("%7c ", ' ');
            }
            else{
                printf("%7d ", temp.value);
            }
        }
        printf("\n\n\n");
    }
}

// swap function takes in the game board, length, the number to be swapped, and the location of the space.
void swap(Element** arr, int length, int num, SpacePosition* space){

    int numLocation[2];

    // This loop looks for the current location of the number on the game board, and saves its indexes.
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length; j++){

            if(arr[i][j].value == num){
                numLocation[0] = i;
                numLocation[1] = j;
            }
        }
    }

    // Swap the location of the number and the space, and update the space's new location to the number that was just swapped.
    Element temp = arr[space->i][space->j];
    arr[space->i][space->j] = arr[numLocation[0]][numLocation[1]];
    arr[numLocation[0]][numLocation[1]] = temp;

    space->i = numLocation[0];
    space->j = numLocation[1];
}

// validateMove function returns 1 if the move asked by the player is valid, and 0 if not.
int validateMove(Element** arr, int length, SpacePosition* space, int num){

    // In main, the user is asked which number they would like to slide.
    // Firstly, we need the location of that number to perform the correct check.

    int x;
    int y;

    for(int i = 0; i < length; i++){
        for(int j = 0; j < length; j++){
            if(arr[i][j].value == num){
                x = i;
                y = j;
            }
        }
    }

    /*
      Possibilities:
      Number is in the inner square (check all adjacent sides for space)
      Number is in one of four corners (check appropriate 2 adjacent sides for space)
      Number is in outermost square, and not a corner (check appropriate 3 adjacent squares for space)
      Number has no valid adjacent spaces
    */

    // ENTIRE INNER SQUARE
    if((x-1) >= 0 && (x+1) < length && (y-1) >= 0 && (y+1) < length){
      if(arr[x][y-1].isSpace == 1 || arr[x][y+1].isSpace == 1 || arr[x-1][y].isSpace == 1 || arr[x+1][y].isSpace == 1){
          return 1;
      }
    }

    // TOP LEFT CORNER
    else if(x == 0 && y == 0){
      if(arr[x][y+1].isSpace == 1 || arr[x+1][y].isSpace == 1){
        return 1;
      }
    }

    // TOP RIGHT CORNER
    else if(x == 0 && y == (length - 1)){
      if(arr[x][y-1].isSpace == 1 || arr[x+1][y].isSpace == 1){
        return 1;
      }
    }

    // BOTTOM LEFT CORNER
    else if((x == (length - 1)) && y == 0){
      if(arr[x-1][y].isSpace == 1 || arr[x][y+1].isSpace == 1){
        return 1;
      }
    }

    // BOTTOM RIGHT CORNER
    else if(y == (length - 1) && x == (length - 1)){
      if(arr[x-1][y].isSpace == 1 || arr[x][y-1].isSpace == 1){
        return 1;
      }
    }

    // LEFT SIDE
    else if(y == 0){
      if(arr[x-1][y].isSpace == 1 || arr[x][y+1].isSpace == 1  || arr[x+1][y].isSpace == 1){
        return 1;
      }
    }

    // TOP SIDE
    else if(x == 0){
      if(arr[x][y+1].isSpace == 1 || arr[x+1][y].isSpace == 1 || arr[x][y-1].isSpace == 1){
        return 1;
      }
    }

    // RIGHT SIDE
    else if(y == (length - 1)){
      if(arr[x-1][y].isSpace == 1 || arr[x][y-1].isSpace == 1 || arr[x+1][y].isSpace == 1){
        return 1;
      }
    }

    // BOTTOM SIDE
    else if(x == (length - 1)){
      if(arr[x][y-1].isSpace == 1 || arr[x-1][y].isSpace == 1 || arr[x][y+1].isSpace == 1){
        return 1;
      }
    }

    else {
      return 0;
    }
}

// checkDone returns 1 if the board is complete, and 0 if not.
int checkDone(Element** finalBoard, Element** gameBoard, int length){

  // If any equivilant positions in the finalBoard compared against the gameBoard are unequal, we can safely return false.
  for(int i = 0; i < length; i++){
    for(int j = 0; j < length; j++){
      if(i == (length - 1) && j == (length - 1)){
        return 1;
      }
      if(finalBoard[i][j].value != gameBoard[i][j].value){
        return 0;
      }
    }
  }

  // If end is reached with no false returns, the two boards are identical and the player must have won.
  return 1;
}