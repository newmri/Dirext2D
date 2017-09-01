// Programming 2D Games
// Copyright (c) 2011, 2014 by: 
// Charles Kelly
// mazeDemo.h v1.0

#ifndef _MAZEDEMO_H           // Prevent multiple definitions if this 
#define _MAZEDEMO_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include <ctime>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"

enum compass{ N, E, S, W };       // direction

// grid type flags. A single binary digit represents each flag: 0 = false, 1 = true
//      Bit 0 This grid is part of maze
//      Bit 1 This grid is maze starting point
//      Bit 2 This grid is maze exit
//      Bits 3-31 unused, BIT3 through BIT7 defined as an example and may be changed.
const int USED = 1, START = 2, EXIT = 4, BIT3 = 8, BIT4 = 16, BIT5 = 32, BIT6 = 64, BIT7 = 128;

// Structure that defines one grid of the maze
struct mazeGrid_stc {
    bool wallW;     // wall on West edge
    bool wallN;     // wall on North edge
    bool wallE;     // wall on East edge
    bool wallS;     // wall on South edge
    int  grid;  // what is this grid used for?
};

const int MAZE_DIM = 20;    // maze size in grids, width and height
const int GRID_SIZE = 20;   // width and height of one grid in pixels
const float MAZE_X = 20;    // screen X location of top left corner
const float MAZE_Y = 20;    // screen Y location of top left corner
const int START_ROW = 0;    // starting location
const int EXIT_ROW = MAZE_DIM - 1; // maze exit

//=============================================================================
// This class is the core of the game
//=============================================================================
class MazeDemo : public Game
{
private:
    // game items
    char wallr, wallb;
    mazeGrid_stc mazeArray[MAZE_DIM][MAZE_DIM]; // the maze 

public:
    MazeDemo();             // Constructor
    virtual ~MazeDemo();    // Destructor

    void initialize(HWND hwnd); // set up everything
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "

    // maze function prototypes
    void generateMaze();
    void generatePath(int row, int col);
    void connectGrid(int row, int col);
    bool moveRandom(int& row, int& col);

    // memory cleanup and restore
    void releaseAll();
    void resetAll();
};

#endif
