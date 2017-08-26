// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Draw bouncing ball
// Chapter 6 version 2.0
// Last modification: April-13-2013

#include "gravity.h"

//=============================================================================
// Constructor
//=============================================================================
Gravity::Gravity()
{}

//=============================================================================
// Destructor
//=============================================================================
Gravity::~Gravity()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Gravity::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    // backdrop texture
    if (!backdropTexture.initialize(graphics,BACKDROP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

    // ball texture
    if (!ballTexture.initialize(graphics,BALL_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball texture"));

    // paddle texture
    if (!paddleTexture.initialize(graphics,PADDLE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

    // backdrop
    if (!backdrop.initialize(graphics,0,0,0,&backdropTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

    // ball
    if (!ball.initialize(this,ballNS::WIDTH,ballNS::HEIGHT,0,&ballTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball"));
    ball.setX(GAME_WIDTH/4 - ballNS::WIDTH);
    ball.setY(GAME_HEIGHT/2 - ballNS::HEIGHT);
    ball.setVelocity(VECTOR2(ballNS::SPEED,-ballNS::SPEED)); // VECTOR2(X, Y)

    // paddle
    if (!paddle.initialize(this,paddleNS::WIDTH,paddleNS::HEIGHT,0,&paddleTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball"));

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Gravity::update()
{
    if (input->isKeyDown(LEFT_KEY))
        paddle.setX(paddle.getX()-paddleNS::SPEED*frameTime);
    else if(input->isKeyDown(RIGHT_KEY))
        paddle.setX(paddle.getX()+paddleNS::SPEED*frameTime);
    ball.update(frameTime);
    paddle.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Gravity::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Gravity::collisions()
{
    VECTOR2 cv;
    if(ball.collidesWith(paddle, cv))
        ball.bounce(cv, paddle);
}

//=============================================================================
// Render game items
//=============================================================================
void Gravity::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    backdrop.draw();                        // add the backdrop to the scene
    ball.draw();                            // add the ball to the scene
    paddle.draw();                          // add the paddle to the scene
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Gravity::releaseAll()
{
    paddleTexture.onLostDevice();           // paddle texture
    ballTexture.onLostDevice();             // ball texture
    backdropTexture.onLostDevice();         // backdrop texture

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Gravity::resetAll()
{
    backdropTexture.onResetDevice();
    ballTexture.onResetDevice();
    paddleTexture.onResetDevice();

    Game::resetAll();
    return;
}
