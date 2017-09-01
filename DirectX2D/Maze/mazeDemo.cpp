// Programming 2D Games
// Copyright (c) 2011, 2014 by: 
// Charles Kelly
// mazeDemo.cpp v1.0
// Generate a random maze. The maze is created in the array mazeArray.
// MAZE_DIM is the size of the maze array.
// This algorithm is of my own design. I just came up with it on the fly so it may not
// be the greatest example in the world but it works. It uses loops instead of recursion
// because some programmers have difficulty understanding recursive code.

#include "mazeDemo.h"

//=============================================================================
// Constructor
//=============================================================================
MazeDemo::MazeDemo()
{}

//=============================================================================
// Destructor
//=============================================================================
MazeDemo::~MazeDemo()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void MazeDemo::initialize(HWND hwnd)
{
    srand(static_cast<UINT>(time(NULL)));   // initialize random numbers
    Game::initialize(hwnd); // throws GameError
    generateMaze();         // create maze

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void MazeDemo::update()
{}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void MazeDemo::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void MazeDemo::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void MazeDemo::render()
{
    // Display the maze.
    for (int r = 0; r < MAZE_DIM; r++) {        // for all rows
        for (int c = 0; c < MAZE_DIM; c++) {    // for all columns
            if (mazeArray[r][c].wallN)          // if North wall
                graphics->drawLine( c * GRID_SIZE + MAZE_X,             r * GRID_SIZE + MAZE_Y,     // X1, Y1
                                    c * GRID_SIZE + MAZE_X + GRID_SIZE, r * GRID_SIZE + MAZE_Y,     // X2, Y2
                                    2.0f, graphicsNS::GREEN);                                       // width, color
            if (mazeArray[r][c].wallE)          // if East wall
                graphics->drawLine( c * GRID_SIZE + MAZE_X + GRID_SIZE, r * GRID_SIZE + MAZE_Y,
                                    c * GRID_SIZE + MAZE_X + GRID_SIZE, r * GRID_SIZE + MAZE_Y + GRID_SIZE,
                                    2.0f, graphicsNS::GREEN);
            if (mazeArray[r][c].wallS)          // if South wall
                graphics->drawLine( c * GRID_SIZE + MAZE_X,             r * GRID_SIZE + MAZE_Y + GRID_SIZE,
                                    c * GRID_SIZE + MAZE_X + GRID_SIZE, r * GRID_SIZE + MAZE_Y + GRID_SIZE,
                                    2.0f, graphicsNS::GREEN);
            if (mazeArray[r][c].wallW)          // if West wall
                graphics->drawLine( c * GRID_SIZE + MAZE_X,             r * GRID_SIZE + MAZE_Y,
                                    c * GRID_SIZE + MAZE_X, r * GRID_SIZE + MAZE_Y + GRID_SIZE,
                                    2.0f, graphicsNS::GREEN);
        }
    }
}

//=============================================================================
// Generate a new random maze in mazeArray
// The entry is always on the left edge and the exit is always on the right edge
//=============================================================================
void MazeDemo::generateMaze() {
    int r, c;              // row & col

    // clear mazeArray
    for (r = 0; r < MAZE_DIM; r++) {        // for all rows in maze
        for (c = 0; c < MAZE_DIM; c++) {    // for all columns in maze
            mazeArray[r][c].wallW = true;   // add walls
            mazeArray[r][c].wallN = true;   // add walls
            mazeArray[r][c].wallE = true;   // add walls
            mazeArray[r][c].wallS = true;
            mazeArray[r][c].grid = 0;
        }
    }

    // Initialize the maze start and exit
    // The start grid is always on the left and the exit is always on the right
    mazeArray[START_ROW][0].wallW = false;      // open entrance
    mazeArray[START_ROW][0].grid |= USED;       // make part of maze
    mazeArray[EXIT_ROW][MAZE_DIM - 1].wallE = false;   // open exit
    mazeArray[EXIT_ROW][MAZE_DIM - 1].grid |= EXIT;    // exit

    // Generate the maze. Make sure every grid is part of maze
    for (r = 0; r < MAZE_DIM; r++) {            // for all rows
        for (c = 0; c < MAZE_DIM; c++) {        // for all columns
            if ((mazeArray[r][c].grid & USED) == 0)   // if grid not used
                generatePath(r, c);             // generate path from this grid
        }
    }

    return;
}

//=============================================================================
// Generate a random path from the specified grid
//=============================================================================
void MazeDemo::generatePath(int row, int col) {

    connectGrid(row, col);         // connect this grid to existing maze

    // move a random direction and random distance until we can't move
    while (moveRandom(row, col))
    {}

    return;
} // end GeneratePath


//=============================================================================
// Connect this grid to the existing maze
//=============================================================================
void MazeDemo::connectGrid(int row, int col) {
    bool done = false;
    compass dir;                    // direction
    int count = 4;                  // count directions tried
    dir = compass(rand() % 4);      // random direction to start
    do {
        switch (dir) {
        case N:
            if (row - 1 < 0)        // if outside maze
                break;
            if (mazeArray[row - 1][col].grid & USED) {
                mazeArray[row][col].wallN = false;
                mazeArray[row - 1][col].wallS = false;
                done = true;
            }
            break;
        case E:
            if (col + 1 >= MAZE_DIM)    // if outside maze
                break;
            if (mazeArray[row][col + 1].grid & USED) {
                mazeArray[row][col].wallE = false;
                mazeArray[row][col + 1].wallW = false;
                done = true;
            }
            break;
        case S:
            if (row + 1 >= MAZE_DIM)    // if outside maze
                break;
            if (mazeArray[row + 1][col].grid & USED) {
                mazeArray[row][col].wallS = false;
                mazeArray[row + 1][col].wallN = false;
                done = true;
            }
            break;
        case W:
            if (col - 1 < 0)            // if outside maze
                break;
            if (mazeArray[row][col - 1].grid & USED) {
                mazeArray[row][col].wallW = false;
                mazeArray[row][col - 1].wallE = false;
                done = true;
            }
            break;
        } // end switch

        dir = compass(dir + 1);         // try next direction if no connection made
        if (dir > W)
            dir = N;
        count--;                        // directions tried
    } while (!(done) && count > 0);     // loop until connection is made or all directions have been tried

    mazeArray[row][col].grid |= USED;   // mark this grid as used
    return;
}

//=============================================================================
// Move a random direction and random distance
// Returns: true if move successful
//          false if move failed
//=============================================================================
bool MazeDemo::moveRandom(int& row, int& col) {
    bool done = false;
    compass dir, dir2;          // direction
    int distance;
    int turn;                   // direction to turn if path blocked

    dir = compass(rand() % 4);  // random direction
    dir2 = dir;                 // save it
    turn = rand() % 2;          // 0(left) or 1(right)
    if (turn == 0)
        turn = -1;              // turn left

    do {
        switch (dir) {
        case N:
            if (row - 1 < 0)    // if outside maze
                break;
            if ((mazeArray[row - 1][col].grid & USED) == 0) {   // if grid not used
                distance = (rand() % (row + 2 / 2)) + 1;        // random distance N
                for (int i = 1; i <= distance; i++) {
                    mazeArray[row][col].wallN = false;          // open wall
                    mazeArray[row - 1][col].wallS = false;      // open wall
                    mazeArray[row - 1][col].grid |= USED;       // make part of maze
                    row--;                                      // move up 1 row
                    // if next grid is outside maze OR alreay in use
                    if (row - 1 < 0 || mazeArray[row - 1][col].grid & USED)
                        break;                                  // stop moving in this direction
                }
                done = true;            // we moved
            }
            break;
        case E:
            if (col + 1 >= MAZE_DIM)    // if outside maze
                break;
            if ((mazeArray[row][col + 1].grid & USED) == 0) {   // if grid not used
                distance = (rand() % (MAZE_DIM - col + 2) / 2) + 1; // random distance E
                for (int i = 1; i <= distance; i++) {
                    mazeArray[row][col].wallE = false;          // open wall
                    mazeArray[row][col + 1].wallW = false;      // open wall
                    mazeArray[row][col + 1].grid |= USED;       // make part of maze
                    col++;                                      // move right 1 col
                    // if next grid is outside maze OR alreay in use
                    if (col + 1 >= MAZE_DIM || mazeArray[row][col + 1].grid & USED)
                        break;                                  // stop moving in this direction
                }
                done = true;            // we moved
            }
            break;
        case S:
            if (row + 1 >= MAZE_DIM)    // if outside maze
                break;
            if ((mazeArray[row + 1][col].grid & USED) == 0) {   // if grid not used
                distance = (rand() % ((MAZE_DIM - row + 2) / 2)) + 1; // random distance S
                for (int i = 1; i <= distance; i++) {
                    mazeArray[row][col].wallS = false;          // open wall
                    mazeArray[row + 1][col].wallN = false;      // open wall
                    mazeArray[row + 1][col].grid |= USED;       // make part of maze
                    row++;                                      // move down 1 row
                    // if next grid is outside maze OR alreay in use
                    if (row + 1 >= MAZE_DIM || mazeArray[row + 1][col].grid & USED)
                        break;                                  // stop moving in this direction
                }
                done = true;            // we moved
            }
            break;
        case W:
            if (col - 1 < 0)            // if outside maze
                break;
            if ((mazeArray[row][col - 1].grid & USED) == 0) {   // if grid not used
                distance = (rand() % ((col + 2) / 2)) + 1;      // random distance W
                for (int i = 1; i <= distance; i++) {
                    mazeArray[row][col].wallW = false;          // open wall
                    mazeArray[row][col - 1].wallE = false;      // open wall
                    mazeArray[row][col - 1].grid |= USED;       // make part of maze
                    col--;                                      // move left 1 col
                    // if next grid is outside maze OR alreay in use
                    if (col - 1 < 0 || mazeArray[row][col - 1].grid & USED)
                        break;                                  // stop moving in this direction
                }
                done = true;            // we moved
            }
            break;
        } // end switch

        dir = compass(dir + turn);      // try next direction if we did not move
        if (dir > W)
            dir = N;
        else if (dir < N)
            dir = W;

        // loop until we move or try all directions
    } while (!(done) && (dir != dir2));

    return(done);
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void MazeDemo::releaseAll()
{
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void MazeDemo::resetAll()
{
    Game::resetAll();
    return;
}
