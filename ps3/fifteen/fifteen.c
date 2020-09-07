/***************************************************************************
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements The Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 ***************************************************************************/
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


// constants
#define DIM_MIN 3
#define DIM_MAX 9

// structs
typedef struct intList {
    int value;
    struct intList *next;
} intList;

typedef struct boardList {
    int board[DIM_MAX][DIM_MAX];
    struct boardList *next;
} boardList;

typedef struct node {
    int board[DIM_MAX][DIM_MAX];
    intList *moves;
    struct node *next;
} node;

// global board
int board[DIM_MAX][DIM_MAX];
int d;

// god mode globals
node *path;

// optimal solution
intList *optimalSolution;
int solutionIndex;

// prototypes
void clear();
void greet();
void init();
void draw();

// God mode prototypes
intList *solve();
node *runIDAStar(int[DIM_MAX][DIM_MAX]);
int search(int, int);
bool boardInPath(int, int, int, int);
void prependSuccessorNode(int, int, int, int);
int h(int[DIM_MAX][DIM_MAX]);

// Normal mode prototypes
bool move();

bool won();

int
main(int argc, char *argv[])
{
    // greet user with instructions
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: %s d\n", argv[0]);
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %d x %d and %d x %d, inclusive.\n",
               DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    bool godMode = false;
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // Make a smartMove if in god mode, else make a move
        if (godMode) {
            // DEBUGGING
            if (optimalSolution == NULL) {
                printf("finished too early\n");
                break;
            }
            
            move(optimalSolution->value);
            optimalSolution = optimalSolution->next;
        } else {
            // prompt for move
            printf("Tile to move: ");
            char *tile = GetString();
            
            // move if possible, else report illegality
            if (strcmp(tile, "GOD") == 0) {
                godMode = true;
                optimalSolution = solve();
            } else if (atoi(tile) == 0 || !move(atoi(tile))) {
                printf("\nIllegal move.\n");
                usleep(500000);
            }
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // that's all folks
    intList *optimalSolutionNode;
    while (optimalSolution != NULL) {
        optimalSolutionNode = optimalSolution;
        optimalSolution = optimalSolution->next;
        free(optimalSolutionNode);
    }

    return 0;
}


/*
 * void
 * clear()
 *
 * Clears screen using ANSI escape sequences.
 */

void
clear()
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}


/*
 * void
 * greet()
 *
 * Greets player.
 */

void
greet()
{
    clear();
    printf("WELCOME TO THE GAME OF FIFTEEN\n");
    usleep(2000000);
}


/*
 * void
 * init()
 *
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */

void
init()
{
    // Initialize board to a correct configuration (reversed numbers)
    int counter = d * d - 1;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (counter == 1 && (d % 2 == 0)) {
                board[i][j] = 2;
            } else if (counter == 2 && (d % 2 == 0)) {
                board[i][j] = 1;
            } else {
                board[i][j] = counter;
            }
            counter--;
        }
    }
    
    // Randomize by making a large number of moves
    srand((unsigned int) time(NULL));
    int currentNumber = 0;
    int lastNumber = 0;
    for (int i = 0; i < 1000000; i++) {
        currentNumber = rand() % (d * d);
        if (currentNumber != lastNumber && currentNumber != 0) {
            move(currentNumber);
            lastNumber = currentNumber;
        }
    }
}


/* 
 * void
 * draw()
 *
 * Prints the board in its current state.
 */

void
draw()
{
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == 0) {
                printf("   ");
            } else if (board[i][j] < 10) {
                printf(" %d ", board[i][j]);
            } else {
                printf("%d ", board[i][j]);
            }
        }
        printf("\n");
    }
}

/*
 * intList *solve()
 *
 * Solves the board's current state and
 * returns the solution
 */
intList *solve() {
    // Solve using IDA*
    path = runIDAStar(board);
            
    // free the first path node (except for the result)
    intList *result = path->moves;
    node *firstPathNode = path;
    path = path->next;
    free(firstPathNode);
    
    // free the rest of the path
    while (path != NULL) {
        node *pathNode = path;
        path = path->next;
        
        intList *movesNode;
        while (pathNode->moves != NULL) {
            movesNode = pathNode->moves;
            pathNode->moves = pathNode->moves->next;
            free(movesNode);
        }
        free(pathNode);
    }
    
    // return the optimal path
    return result;
}

node* runIDAStar(int board[DIM_MAX][DIM_MAX]) {
    // Set initial bound
    int bound = h(board);
        
    // Begin the path (in reverse, like a stack)) with the root node
    path = malloc(sizeof(node));
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            path->board[i][j] = board[i][j];
        }
    }
    path->moves = malloc(sizeof(intList));
    path->moves->value = 0;
    path->moves->next = NULL;
    path->next = NULL;
    
    while (1) {
        int t = search(0, bound);
        if (t == 0) return path;
        bound = t;
    }
}

int search(int g, int bound) {
    int f = g + h(path->board);
    if (f > bound) return f;
    if (h(path->board) == 0) return 0;
    int min = 2000000000;
    
    // check successors
    // Find tile 0 and store its coordinates in x and y
    int tileI;
    int tileJ;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (path->board[i][j] == 0) {
                tileI = i;
                tileJ = j;
            }
        }
    }
    // up
    if ((tileI > 0) && !boardInPath(tileI, tileJ, tileI-1, tileJ)) {
        prependSuccessorNode(tileI, tileJ, tileI-1, tileJ);
        int t = search(g + 1, bound);
        if (t == 0) return 0;
        if (t < min) min = t;
        node *oldPath = path;
        path = path->next;
        intList *movesNode;
        while (oldPath->moves != NULL) {
            movesNode = oldPath->moves;
            oldPath->moves = oldPath->moves->next;
            free(movesNode);
        }
        free(oldPath);
    }
    // down
    if ((tileI < d-1) && !boardInPath(tileI, tileJ, tileI+1, tileJ)) {
        prependSuccessorNode(tileI, tileJ, tileI+1, tileJ);
        int t = search(g + 1, bound);
        if (t == 0) return 0;
        if (t < min) min = t;
        node *oldPath = path;
        path = path->next;
        intList *movesNode;
        while (oldPath->moves != NULL) {
            movesNode = oldPath->moves;
            oldPath->moves = oldPath->moves->next;
            free(movesNode);
        }
        free(oldPath);
    }
    // left
    if ((tileJ > 0) && !boardInPath(tileI, tileJ, tileI, tileJ-1)) {
        prependSuccessorNode(tileI, tileJ, tileI, tileJ-1);
        int t = search(g + 1, bound);
        if (t == 0) return 0;
        if (t < min) min = t;
        node *oldPath = path;
        path = path->next;
        intList *movesNode;
        while (oldPath->moves != NULL) {
            movesNode = oldPath->moves;
            oldPath->moves = oldPath->moves->next;
            free(movesNode);
        }
        free(oldPath);
    }
    // right
    if ((tileJ < d-1) && !boardInPath(tileI, tileJ, tileI, tileJ+1)) {
        prependSuccessorNode(tileI, tileJ, tileI, tileJ+1);
        int t = search(g + 1, bound);
        if (t == 0) return 0;
        if (t < min) min = t;
        node *oldPath = path;
        path = path->next;
        intList *movesNode;
        while (oldPath->moves != NULL) {
            movesNode = oldPath->moves;
            oldPath->moves = oldPath->moves->next;
            free(movesNode);
        }
        free(oldPath);
    }
    // return the minimum f
    return min;
}

/**
 * Checks that a successor board isn't already in path
 */
bool boardInPath(int tileI, int tileJ, int newTileI, int newTileJ) {
    // get the successor board
    int successorBoard[DIM_MAX][DIM_MAX];
    int swapValue = path->board[newTileI][newTileJ];
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            successorBoard[i][j] = path->board[i][j];
        }
    }
    successorBoard[newTileI][newTileJ] = 0;
    successorBoard[tileI][tileJ] = swapValue;
    
    node *pathPtr = path;
    while (pathPtr != NULL) {
        bool boardMatch = true;
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < d; j++) {
                if (pathPtr->board[i][j] != successorBoard[i][j]) {
                    boardMatch = false;
                }
            }
        }
        if (boardMatch) return true;
        pathPtr = pathPtr->next;
    }
    
    return false;
}

/**
 * prepend a successor node onto path
 */
void prependSuccessorNode(int tileI, int tileJ, int newTileI, int newTileJ) {
    // get the successor board
    int successorBoard[DIM_MAX][DIM_MAX];
    int swapValue = path->board[newTileI][newTileJ];
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            successorBoard[i][j] = path->board[i][j];
        }
    }
    successorBoard[newTileI][newTileJ] = 0;
    successorBoard[tileI][tileJ] = swapValue;
    
    // add value to the list of moves
    intList *currentMovesPtr = path->moves;
    intList *successorMoves = malloc(sizeof(intList));
    intList *successorMovesPtr = successorMoves;
    while (currentMovesPtr != NULL) {
        successorMovesPtr->value = currentMovesPtr->value;
        successorMovesPtr->next = malloc(sizeof(intList));
        
        successorMovesPtr = successorMovesPtr->next;
        currentMovesPtr = currentMovesPtr->next;
    }
    successorMovesPtr->value = swapValue;
    successorMovesPtr->next = NULL;
    
    // push successor node onto path and return it
    node *newPath = malloc(sizeof(node));
    for (int i = 0; i < d; i++)
        for (int j = 0; j < d; j++)
            newPath->board[i][j] = successorBoard[i][j];
    newPath->moves = successorMoves;
    newPath->next = path;
    path = newPath;
}

/*
 * int h(int board[][])
 *
 * Computes the Manhattan-distance heuristic for a given board
 */
int h(int board[DIM_MAX][DIM_MAX]) {
    int manhattanDistance = 0;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] != 0) {
                // horizontal distance
                manhattanDistance += abs(j - ((board[i][j] - 1) % d));
                // vertical distance
                manhattanDistance += abs(i - ((board[i][j] - 1) / d));
            }
        }
    }
    
    return manhattanDistance;
}

/*
 * bool
 * move(int tile)
 *
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */

bool
move(int tile)
{
    // find the tile
    int tileI = -1;
    int tileJ = -1;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == tile) {
                tileI = i;
                tileJ = j;
            }
        }
    }
    
    // check that tile was found
    if (tileI == -1 || tileJ == -1) {
        return false;
    }
    
    // up
    if (tileI > 0 && board[tileI-1][tileJ] == 0) {
        board[tileI-1][tileJ] = tile;
        board[tileI][tileJ] = 0;
        return true;
    }
    
    // down
    if (tileI < d-1 && board[tileI+1][tileJ] == 0) {
        board[tileI+1][tileJ] = tile;
        board[tileI][tileJ] = 0;
        return true;
    }
    
    // left
    if (tileJ > 0 && board[tileI][tileJ-1] == 0) {
        board[tileI][tileJ-1] = tile;
        board[tileI][tileJ] = 0;
        return true;
    }
    
    // right
    if (tileJ < d-1 && board[tileI][tileJ+1] == 0) {
        board[tileI][tileJ+1] = tile;
        board[tileI][tileJ] = 0;
        return true;
    }
    
    return false;
}

/*
 * bool
 * won()
 *
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */

bool
won()
{
    int counter = 0;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (i == d - 1 && j == d - 1) counter = 0;
            else counter++;
            if (board[i][j] != counter) return false;
        }
    }
    return true;
}
