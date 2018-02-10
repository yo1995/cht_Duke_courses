#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define CLICK_KNOWN_MINE -2
#define CLICK_INVALID -1
#define CLICK_CONTINUE 0
#define CLICK_LOSE 1

#define KNOWN_MINE -3
#define HAS_MINE -2
#define UNKNOWN -1

#define IS_MINE(x) ((x) == HAS_MINE || (x) == KNOWN_MINE)


struct _board_t {
  int ** board;
  int width;
  int height;
  int totalMines;
};typedef struct _board_t board_t;

void addRandomMine(board_t * b) {
  int x;
  int y;
  //we could have a board too small for the number
  //of mines that we request. try w*h*10 times before
  //we give up
  int limit = b->width * b->height * 10;
  do {
    x = random() % b->width;
    y = random() % b->height;
    assert(limit > 0);
    limit--;
  } while(b->board[y][x] == HAS_MINE);
  b->board[y][x] = HAS_MINE;
}

board_t * makeBoard(int w, int h, int numMines) {
  //WRITE ME!
  //the very best way of initializing
  board_t *mineBoard = malloc(sizeof(*mineBoard));
  //board_t *mineBoard = malloc(w * h * sizeof(*mineBoard));
  mineBoard -> width = w;
  mineBoard -> height = h;
  mineBoard -> totalMines = numMines;
  //initializing all the value in mineBoard.board into UNKNOWN
  //first line then column
  mineBoard -> board = malloc(h * sizeof(*mineBoard -> board));
  
  int i = 0;
  int j;
  while(i < h){
	  mineBoard->board[i] = malloc(w * sizeof(*mineBoard -> board[i]));
	  j = 0;
	  while(j < w){
		  mineBoard->board[i][j] = UNKNOWN;
		  j++;
	  }
	  i++;
  }
  //call the addRandomMine function 
  int call_add_random = 0;
  while(call_add_random < numMines){
	  addRandomMine(mineBoard);
	  call_add_random++;
  }
  return mineBoard;
}

void printBoard(board_t * b) {    
  int found = 0;
  printf("    ");
  for (int x = 0; x < b->width; x++) {
    printf("%d",x/10);
  }
  printf("\n    ");
  for (int x = 0; x < b->width; x++) {
    printf("%d",x%10);
  }
  printf("\n----");
  for (int x = 0; x < b->width; x++) {
    printf("-");
  }
  printf("\n");
  for (int y = 0; y < b->height; y++) {
    printf("%2d: ", y %100);
    for (int x = 0; x < b->width; x++) {
      if (b->board[y][x] == KNOWN_MINE) {
	printf("*");
	found++;
      }
      else if (b->board[y][x] < 0) {
	printf("?");
      }
      else if (b->board[y][x] == 0) {
	printf(" ");
      }
      else {
	printf("%d", b->board[y][x]);
      }
    }
    printf("\n");
  }
  printf("\n----");
  for (int x = 0; x < b->width; x++) {
    printf("-");
  }
  printf("\n");

  printf("    ");
  for (int x = 0; x < b->width; x++) {
    printf("%d",x/10);
  }
  printf("\n    ");
  for (int x = 0; x < b->width; x++) {
    printf("%d",x%10);
  }
  printf("\nFound %d of %d mines\n", found, b->totalMines);
}

int countMines(board_t * b, int x, int y) {
  //WRITE ME!
  int countsum = 0;
  int w;
  int h;
  w = b -> width;
  h = b -> height;
  int i = 0;
  int j;
  //pay attention to the order of decision! mistaken and failed in corner cases
  while(i < 3){
	  j = 0;
	  while(j < 3){
		  // we need to consider the bondary conditions here
		  //and this one is pretty long...maybe we can marcodef it.
		  // or we can write 8 conditions to judge each block. the
		  // complexity is not increased, just the code become long
		  if(((x-1+i) >= 0) && ((y-1+j) >= 0) && ((x-1+i) < w) && ((y-1+j) < h) && (IS_MINE(b -> board[y-1+j][x-1+i]))){
			  if(((x-1+i) == x) && ((y-1+j) == y)){
				  //the center block is not counted
				  continue;
			  }
			  countsum++;
		  }
		  j++;
	  }
	  i++;
  }
  return countsum;
}

int click (board_t * b, int x, int y) {
  if (x < 0 || x >= b->width ||
      y < 0 || y >= b->height) {
    return CLICK_INVALID;
  }
  if (b->board[y][x] == KNOWN_MINE) {
    return CLICK_KNOWN_MINE;
  }
  if (b->board[y][x] == HAS_MINE) {
    return CLICK_LOSE;
  }
  if (b->board[y][x] != UNKNOWN) {
    return CLICK_CONTINUE;
  }

  b->board[y][x] = countMines(b,x,y);
  return CLICK_CONTINUE;
}

//this part have problems that I did not catch!
//after winning should goto play another part
int checkWin(board_t * b) {
  //WRITE ME!
  int x;
  int y;
  int i;
  int j;
  x = b -> width;
  y = b -> height;
  i = 0;
  while(i < y){
	  j = 0;
	  while(j < x){
		  if(b -> board[i][j] == UNKNOWN){
			  return 0;
		  }
		  j++;
	  }
	  i++;
  }
  // all the blocks are checked, won the game!
  return 1;
}

void freeBoard(board_t * b) {
  //WRITE ME!
  int y;
  int i = 0;
  y = b -> height;
  //first free all the lines
  while(i < y){
	  free(b -> board[i]);
	  i++;
  }
  //then free the pointer
  free(b -> board);
  //all sum up! free b itself
  free(b);
  
}

int readInt(char ** linep, size_t * lineszp) {
  if (getline (linep, lineszp, stdin) == -1) {
    fprintf (stderr,"End of file from keyboard reading a number.  Quitting\n");
    exit(EXIT_FAILURE);
  }
  char * endptr = NULL;
  long int x = strtol (*linep, &endptr, 10);
  if (endptr == *linep) {
    fprintf (stderr,"You did not enter any valid number\n");
    printf ("Please try again\n");
    return readInt (linep, lineszp);
  }
  if (*endptr != '\n') {
    fprintf( stderr, 
	     "Input was not entirely a number (junk at end)\n");
    printf ("Please try again\n");
    return readInt (linep, lineszp);
  }
  if (x > INT_MAX) {
    fprintf(stderr,"%ld is too big for an int!\n", x);
    printf("Please try again\n");
    return readInt(linep, lineszp);
  }
  return x;
}

void doReveal(board_t * b, int x, int y, int revealMines){
  for (int dy = -1; dy <=1 ; dy++) {
    for (int dx = -1; dx <=1 ; dx++) {
      int nx = x + dx;
      int ny = y + dy;
      if (nx >= 0 && nx < b->width &&
	  ny >= 0 && ny < b->height) {
	if (revealMines) {
	  assert(b->board[ny][nx] != UNKNOWN);
	  if (b->board[ny][nx] == HAS_MINE){
	    b->board[ny][nx] = KNOWN_MINE; 
	  }
	}
	else  {
	  assert(b->board[ny][nx] != HAS_MINE);
	  if (b->board[ny][nx] == UNKNOWN) {
	    b->board[ny][nx] = countMines(b,nx,ny);
	  }
	}
      }
    }
  }
}

int maybeReveal(board_t * b, int x, int y) {
  int unknownSquares = 0;
  int knownMines = 0;
  for (int dy = -1; dy <=1 ; dy++) {
    for (int dx = -1; dx <=1 ; dx++) {
      int nx = x + dx;
      int ny = y + dy;
      if (nx >= 0 && nx < b->width &&
	  ny >= 0 && ny < b->height) {
	if (b->board[ny][nx] == UNKNOWN || 
	    b->board[ny][nx] == HAS_MINE) {
	  unknownSquares++;
	}
	else if(b->board[ny][nx] == KNOWN_MINE) {
	  knownMines++;
	}
      }
    }
  }
  assert(knownMines + unknownSquares >= b->board[y][x]);
  assert(knownMines <= b->board[y][x]);
  if (unknownSquares > 0) {
    int revealMines = (knownMines + unknownSquares == 
		       b->board[y][x]);
    int allKnown = knownMines == b->board[y][x];
    if(revealMines || allKnown) {
      assert(!revealMines || !allKnown);
      doReveal(b,x,y, revealMines);
      return 1;
    }
  }
  return 0;
}
void determineKnownMines(board_t * b) {
  int foundMore = 0;
  for (int y = 0; y < b->height; y++) {
    for (int x = 0; x < b->width; x++) {
      if (b->board[y][x] >= 0) {
	foundMore = maybeReveal(b,x,y) || foundMore;
      }
    }
  }
  if (foundMore) {
    determineKnownMines(b);
  }
}

void revealMines(board_t * b) {
  for (int y = 0; y < b->height; y++) {
    for (int x = 0; x < b->width; x++) {
      if (b->board[y][x] == HAS_MINE) {
	b->board[y][x] = KNOWN_MINE;
      }
    }
  }
}
int playTurn(board_t * b, char ** linep, size_t *lineszp) {
  printf("Current board:\n");
  printBoard(b);
  printf("x coordinate:\n");
  int x = readInt(linep, lineszp);
  printf("y coordinate:\n");
  int y = readInt(linep, lineszp);
  int result = click(b,x,y);
  determineKnownMines(b);
  if (result == CLICK_LOSE) {
    printf("Oh no! That square had a mine. You lose!\n");
    revealMines(b);
    printBoard(b);
    return 1;
  }
  else if (result == CLICK_INVALID) {
    printf("That is not a valid square, please try again\n");
  }
  else if (result == CLICK_KNOWN_MINE) {
    printf("You already know there is a mine there!\n");
  }
  else if(checkWin(b)) {
    printBoard(b);
    printf("You win!\n");
    return 1;
  }
  return 0;
}


int main(int argc, char ** argv) {
  if (argc != 4) {
    fprintf(stderr,"Usage: minesweeper width height numMines\n");
    return EXIT_FAILURE;
  }
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  int numMines = atoi(argv[3]);
  if (width <= 0 || height <= 0 || numMines <= 0) {
    fprintf(stderr,
	    "Width, height, and numMines must all be positive ints\n");
    return EXIT_FAILURE;
  }
  char * line = NULL;
  size_t linesz = 0;

  do {
    board_t * b = makeBoard (width, height, numMines);
    int gameOver = 0;
    while (!gameOver) {
      gameOver = playTurn(b, &line, &linesz);
    }
    freeBoard(b);
    do {
      printf("Do you want to play again?\n");
    } while(getline(&line, &linesz, stdin) == -1);
  } while(line[0] == 'Y' || line[0] == 'y');
  free(line);
  return EXIT_SUCCESS;
}
