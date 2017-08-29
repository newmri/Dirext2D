// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Checkers.cpp Isometric Grid Demo v1.0
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

    // Put some checkers on the board for demo
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

    drawOrder = 0;
    drawCheckers = true;
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
    if(input->wasKeyPressed('5'))
        drawCheckers = !drawCheckers;
    keyin = input->getCharIn();         // get char pressed
    if(keyin != '5')
        drawOrder = keyin;
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
{}

//=============================================================================
// Render game items
//=============================================================================
void Checkers::render()
{
    int black=0;

    graphics->spriteBegin();

    // Menu
    dxFont.print("1 - rows and cols are alternate colors", 10, 10);
    dxFont.print("2 - rows are alternate colors", 10, 30);
    dxFont.print("3 - cols are alternate colors", 10, 50);
    dxFont.print("4 - rows and cols in wrong order", 10, 70);
    dxFont.print("5 - toggle checkers on/off", 10, 90);

    // Draw checkerboard in Isometric Stagger
    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            // set tile position
            boardSquare.setX((float)( BOARD_X + (col*TEXTURE_SIZE/2) ));

            if(col % 2)                 // if odd col
                if (drawOrder == '4')   // if even columns are higher (wrong order)
                    boardSquare.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/2 + TEXTURE_SIZE/4) ));
                else
                    boardSquare.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/2) ));
            else                        // even col
                if (drawOrder == '4')
                    boardSquare.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/2) ));
                else
                    boardSquare.setY((float)( BOARD_Y + (row*TEXTURE_SIZE/2 + TEXTURE_SIZE/4) ));

            // determine tile color
            if(drawOrder == '2')        // rows are alternating colors
                black = row %2;
            else if(drawOrder == '3')   // cols are alternating colors
                black = col % 2;
            else                        // rows and cols are alternating colors
                black = (row + col)%2;
            if(black)   
                boardSquare.draw(graphicsNS::BLACK);
            else
                boardSquare.draw(graphicsNS::WHITE);
        }
    }

    // Draw checkers back to front
    // The odd column squares are drawn behind the even column squares
    // Draw the checkers by columns, odd, even, odd, etc.
    if(drawCheckers)
    {
        for(int row=0; row<BOARD_SIZE; row++)
        {
            for(int col=1; col<BOARD_SIZE; col+=2)  // for odd columns
                drawOneChecker(row,col);
            for(int col=0; col<BOARD_SIZE; col+=2)  // for even columns
                drawOneChecker(row,col);
        }
    }

    graphics->spriteEnd();
}

// Draw one checker at row,col
void Checkers::drawOneChecker(int row, int col)
{
    float checkerY=0;

    checker.setX((float)( BOARD_X + (col*TEXTURE_SIZE/2) ));
    king.setX((float)( BOARD_X + (col*TEXTURE_SIZE/2) ));

    if(col % 2)                         // if odd col
        if (drawOrder == '4')           // if even columns are higher (wrong order)
            checkerY = (float)(BOARD_Y + (row*TEXTURE_SIZE/2 + TEXTURE_SIZE/4) );
        else
            checkerY = (float)(BOARD_Y + (row*TEXTURE_SIZE/2) );
    else                                // even col
        if (drawOrder == '4')
            checkerY = (float)(BOARD_Y + (row*TEXTURE_SIZE/2) );
        else
            checkerY = (float)(BOARD_Y + (row*TEXTURE_SIZE/2 + TEXTURE_SIZE/4) );

    switch(board[row][col])
    {
    case 'r':
        // draw reflection
        checker.setY((float)(checkerY + TEXTURE_SIZE/7));
        checker.draw(checkersNS::RED_COLOR & graphicsNS::ALPHA25);
        // draw checker
        checker.setY(checkerY);
        checker.draw(checkersNS::RED_COLOR);
        break;
    case 'R':
        // draw reflection
        king.setY((float)(checkerY + TEXTURE_SIZE/4));
        king.draw(checkersNS::RED_COLOR & graphicsNS::ALPHA25);
        // draw king
        king.setY(checkerY);
        king.draw(checkersNS::RED_COLOR);
        break;
    case 'b':
        // draw reflection
        checker.setY((float)(checkerY + TEXTURE_SIZE/7));
        checker.draw(checkersNS::BLUE_COLOR & graphicsNS::ALPHA25);
        // draw checker
        checker.setY(checkerY);
        checker.draw(checkersNS::BLUE_COLOR);
        break;
    case 'B':
        // draw reflection
        king.setY((float)(checkerY + TEXTURE_SIZE/4));
        king.draw(checkersNS::BLUE_COLOR & graphicsNS::ALPHA25);
        // draw king
        king.setY(checkerY);
        king.draw(checkersNS::BLUE_COLOR);
        break;
    }
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
