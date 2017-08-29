// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Checkers.cpp Isometric Diamond Demo v1.0
// Modified Sep-14-2013
// Checker moves to selected square.
// Demonstrates isometric tile selection.

#include "checkers.h"
using namespace checkersNS;

//=============================================================================
// Constructor
//=============================================================================
Checkers::Checkers()
{
    for(int row=0; row<BOARD_SIZE; row++)
        for(int col=0; col<BOARD_SIZE; col++)
            board[row][col] = 0;

    // Put some checkers on board for demo
    Brow = 7;
    Bcol = 7;
    board[Brow][Bcol]='B';

    mouseX = 0;
    mouseY = 0;
}

//=============================================================================
// Destructor
//=============================================================================
Checkers::~Checkers()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void Checkers::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // game textures
    if (!textures.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures"));

    // Board square image
    boardSquare.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&textures);
    boardSquare.setFrames(BOARD_FRAME, BOARD_FRAME);
    boardSquare.setCurrentFrame(BOARD_FRAME);

    // Checker image
    checker.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&textures);
    checker.setFrames(CHECKER_FRAME, CHECKER_FRAME);
    checker.setCurrentFrame(CHECKER_FRAME);

    // King image
    king.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&textures);
    king.setFrames(KING_FRAME, KING_FRAME);
    king.setCurrentFrame(KING_FRAME);
}

//=============================================================================
// Update all game items
//=============================================================================
void Checkers::update()
{
    if (graphics->getFullscreen() == false) // if windowed
    {
        // calculate screen ratios incase window was resized
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        screenRatioX = (float)GAME_WIDTH / clientRect.right;
        screenRatioY = (float)GAME_HEIGHT / clientRect.bottom;
    }




    //*************************************************************************
    // Isometric Tile Selection
    // The white square at the top of the checkerboard is Row 0, Col 0
    // The black square at the right corner is Row 0, Col 7
    // The black square at the left corner is Row 7, Col 0
    // The white square at the bottom is Row 7, Col 7
    //*************************************************************************
    selectedCol = -1;
    selectedRow = -1;
    // Mouse position relative to board. 
    // This makes the top corner of the checkerboard location 0,0
    mouseX = input->getMouseX()*screenRatioX - BOARD_X - TEXTURE_SIZE/2;
    mouseY = input->getMouseY()*screenRatioY - BOARD_Y - TEXTURE_SIZE/2;

    if (input->getMouseLButton())   // If left mouse button pressed
    {
        // Calculate selected column
        selectedCol = (int)(mouseX / TEXTURE_SIZE + mouseY / (TEXTURE_SIZE/2));
        if(selectedCol >= BOARD_SIZE)
            selectedCol = -1;       // Invalid selection
        // Calculate selected row
        selectedRow = (int)(mouseY / (TEXTURE_SIZE/2) - mouseX / TEXTURE_SIZE);
        if(selectedRow >= BOARD_SIZE)
            selectedRow = -1;       // Invalid selection

        if(selectedCol >= 0 && selectedRow >= 0)    // If valid square selected
        {
            // Move the checker
            board[Brow][Bcol] = ' ';    // Remove from current location
            Brow = selectedRow;
            Bcol = selectedCol;
            board[Brow][Bcol] = 'B';    // Place in new location
        }
    }
    //*************************************************************************




    boardSquare.update(frameTime);
    checker.update(frameTime);
    king.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Checkers::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Checkers::collisions()
{
}

//=============================================================================
// Render game items
//=============================================================================
void Checkers::render()
{
    graphics->spriteBegin();

    // Draw checkerboard
    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            // Isometric Diamond draw
            if((row + col) % 2)
            {
                boardSquare.setX((float)( BOARD_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
                boardSquare.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) ));
                boardSquare.draw(graphicsNS::BLACK);
            }
            else
            {
                boardSquare.setX((float)( BOARD_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
                boardSquare.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) ));
                boardSquare.draw(graphicsNS::WHITE);
            }
        }
    }

    // Draw checkers
    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            switch(board[row][col])
            {
            case 'r':
                checker.setX((float)( BOARD_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
                // draw reflection
                checker.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) + TEXTURE_SIZE/7 ));
                checker.draw(graphicsNS::RED & graphicsNS::ALPHA25);
                // draw checker
                checker.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) ));
                checker.draw(checkersNS::RED_COLOR);
                break;
            case 'R':
                king.setX((float)( BOARD_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
                // draw reflection
                king.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) + TEXTURE_SIZE/4 ));
                king.draw(graphicsNS::RED & graphicsNS::ALPHA25);
                // draw checker
                king.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) ));
                king.draw(checkersNS::RED_COLOR);
                break;
            case 'b':
                checker.setX((float)( BOARD_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
                // draw reflection
                checker.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) + TEXTURE_SIZE/7 ));
                checker.draw(graphicsNS::BLUE & graphicsNS::ALPHA25);
                // draw checker
                checker.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) ));
                checker.draw(checkersNS::BLUE_COLOR);
                break;
            case 'B':
                king.setX((float)( BOARD_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
                // draw reflection
                king.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) + TEXTURE_SIZE/4 ));
                king.draw(graphicsNS::BLUE & graphicsNS::ALPHA25);
                // draw checker
                king.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) ));
                king.draw(checkersNS::BLUE_COLOR);
                break;
            }
        }
    }

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Checkers::releaseAll()
{
    textures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Checkers::resetAll()
{
    textures.onResetDevice();

    Game::resetAll();
    return;
}
