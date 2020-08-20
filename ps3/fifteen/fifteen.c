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
    int f;
    int g;
    struct node *next;
} node;

// global board
int board[DIM_MAX][DIM_MAX];
int d;

// optimal solution
intList *optimalSolution;
boardList *closedSet;
int solutionIndex;

// prototypes
void clear();
void greet();
void init();
void draw();

// God mode prototypes
intList *solve();
node *createNode(node *, node *, int, int, int, int);
int h(int[DIM_MAX][DIM_MAX]);
bool inClosedSet(int[DIM_MAX][DIM_MAX]);
void insertIntoClosedSet(int[DIM_MAX][DIM_MAX]);
node *insertIntoQueue(node *, node *);

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

    boardList *closedSetNode;
    while (closedSet != NULL) {
        closedSetNode = closedSet;
        closedSet = closedSet->next;
        free(closedSetNode);
    }
    free(closedSet);
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
    // Begin the priority queue with the root node
    node *queue = malloc(sizeof(node));
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            queue->board[i][j] = board[i][j];
        }
    }
    queue->f = h(board);
    queue->g = 0;
    queue->moves = malloc(sizeof(intList));
    queue->moves->value = 0;
    queue->moves->next = NULL;
    queue->next = NULL;
    
    // Begin the closed set with the original board
    closedSet = malloc(sizeof(boardList));
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            closedSet->board[i][j] = board[i][j];
        }
    }
    closedSet->next = NULL;
    
    // Run A*
    while (queue != NULL) {
        int i,j,tileI,tileJ;

        // Find tile 0 and store its coordinates in x and y
        for (i = 0; i < d; i++) {
            for (j = 0; j < d; j++) {
                if (queue->board[i][j] == 0) {
                    tileI = i;
                    tileJ = j;
                }
            }
        }
        
        // pop queue
        node *currentNode = queue;
        queue = queue->next;
        
        // Determine possible moves and add them to the queue
        // up
        if (tileI > 0) {
            queue = createNode(currentNode, queue, tileI, tileJ, tileI-1, tileJ);
        }
        
        // down
        if (tileI < d-1) {
            queue = createNode(currentNode, queue, tileI, tileJ, tileI+1, tileJ);
        }
        
        // left
        if (tileJ > 0) {
            queue = createNode(currentNode, queue, tileI, tileJ, tileI, tileJ-1);
        }
        
        // right
        if (tileJ < d-1) {
            queue = createNode(currentNode, queue, tileI, tileJ, tileI, tileJ+1);
        }
        
        // return if we found the solution
        if (h(queue->board) == 0) {
            // store result
            intList *result = queue->moves;
            
            // free the first queue node (except for the result)
            node *queueNode = queue;
            queue = queue->next;
            free(queueNode);
            
            // free the rest of the queue
            while (queue != NULL) {
                queueNode = queue;
                queue = queue->next;
                
                intList *movesNode;
                while (queueNode->moves != NULL) {
                    movesNode = queueNode->moves;
                    queueNode->moves = queueNode->moves->next;
                    free(movesNode);
                }
                free(queueNode);
            }
            
            return result;
        }
    }
    
    // should never get here
    return NULL;
}

/*
 * node *createNode
 * Creates a new node given the current node, the queue, and move (i,j) -> (newI,newJ)
 */
node *createNode(node *currentNode, node *queue, int tileI, int tileJ, int newTileI, int newTileJ) {
    // create new node
    node *newNode = malloc(sizeof(node));
    
    // set the new node's board
    int swapValue = currentNode->board[newTileI][newTileJ];
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            newNode->board[i][j] = currentNode->board[i][j];
        }
    }
    newNode->board[newTileI][newTileJ] = 0;
    newNode->board[tileI][tileJ] = swapValue;
    
    // check board against the closed set
    if (inClosedSet(newNode->board)) {
        return queue;
    } else {
        insertIntoClosedSet(newNode->board);
    }
    
    // add value to the list of moves
    intList *currentQueueMove = currentNode->moves;
    intList *currentNewNodeMove = malloc(sizeof(intList));
    intList *newNodeMoves = currentNewNodeMove;
    while (currentQueueMove != NULL) {
        currentNewNodeMove->value = currentQueueMove->value;
        currentNewNodeMove->next = malloc(sizeof(intList));
        
        currentQueueMove = currentQueueMove->next;
        currentNewNodeMove = currentNewNodeMove->next;
    }
    currentNewNodeMove->value = swapValue;
    currentNewNodeMove->next = NULL;
    newNode->moves = newNodeMoves;
    
    
    // update value
    newNode->f = currentNode->g + 1 + h(newNode->board);
    newNode->g = currentNode->g + 1;
    
    // insert into the priority queue
    queue = insertIntoQueue(queue, newNode);
    
    return queue;
}

/*
 * bool inClosedSet
 * Returns whether the given board is in the closed set
 */
bool inClosedSet(int board[DIM_MAX][DIM_MAX]) {
    boardList *boardListCopy = closedSet;
    bool areEqual = true;
    while (boardListCopy != NULL) {
        for (int i = 0; i < d; i++) {
            for (int j = 0; j < d; j++) {
                if (boardListCopy->board[i][j] != board[i][j]) {
                    areEqual = false;
                }
            }
        }
        if (areEqual) return true;
        boardListCopy = boardListCopy->next;
    }
    return false;
}

/*
 * void insertIntoClosedSet
 * Inserts the given board into the closed set.
 */
void insertIntoClosedSet(int board[DIM_MAX][DIM_MAX]) {
    boardList *newBoardList = malloc(sizeof(boardList));
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            newBoardList->board[i][j] = board[i][j];
        }
    }
    newBoardList->next = closedSet;
    closedSet = newBoardList;
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
 * node *insertIntoQueue()
 *
 * Inserts the given node (respecting order) into the given queue
 */
node *insertIntoQueue(node *queue, node *element) {
    if (queue == NULL) {
        return element;
    }
    
    // does elt go in front?
    if (element->f <= queue->f) {
        element->next = queue;
        return element;
    }
    
    // otherwise, loop through
    node *current = queue;
    node *next = queue->next;
    while (next != NULL) {
        if (element->f <= next->f) {
            current->next = element;
            element->next = next;
            return queue;
        }
        next = next->next;
        current = current->next;
    }
    
    current->next = element;
    return queue;
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
