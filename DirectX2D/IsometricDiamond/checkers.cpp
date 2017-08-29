// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Checkers.cpp Isometric Diamond Demo v1.0
// Checkers is the class we create.

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
    board[0][1]='r';
    board[0][2]='B';
    board[0][3]='r';
    board[1][1]='R';
    board[1][2]='R';
    board[1][3]='B';
    board[4][7]='R';
    board[5][7]='B';
    board[6][7]='R';
    board[7][7]='B';
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
