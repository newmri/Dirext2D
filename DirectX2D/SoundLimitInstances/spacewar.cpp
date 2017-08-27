// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Draw animated spaceship
// Chapter 7 spacewar.cpp v1.0
// This class is the core of the game

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{}

//=============================================================================
// Destructor
//=============================================================================
Spacewar::~Spacewar()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    // backdrop texture
    if (!backTexture.initialize(graphics,BACK_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));
    // spaceship1 texture
    if (!ship1Texture.initialize(graphics,SHIP1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1 texture"));
    // spaceship2 texture
    if (!ship2Texture.initialize(graphics,SHIP2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2 texture"));

    // backdrop
    if (!backdrop.initialize(graphics,0,0,0,&backTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

    // ship1
    if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &ship1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1 texture"));
    ship1.setFrames(shipNS::SHIP_START_FRAME, shipNS::SHIP_END_FRAME);
    ship1.setCurrentFrame(shipNS::SHIP_START_FRAME);
    ship1.setX(GAME_WIDTH/4 - shipNS::WIDTH);
    ship1.setY(GAME_HEIGHT/2 - shipNS::HEIGHT*2);
    ship1.setDegrees(270);
    ship1.setScale(2.0f);

    // ship2
    if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &ship2Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2 texture"));
    ship2.setFrames(shipNS::SHIP_START_FRAME, shipNS::SHIP_END_FRAME);
    ship2.setCurrentFrame(shipNS::SHIP_START_FRAME);
    ship2.setX(GAME_WIDTH - GAME_WIDTH/4 - shipNS::WIDTH);
    ship2.setY(GAME_HEIGHT/2 - shipNS::HEIGHT*2);
    ship2.setDegrees(270);
    ship2.setScale(2.0f);

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
{
    if(input->isKeyDown('A'))
        audio->playCue(LIMIT1);                     // play sound
    if(input->isKeyDown('B'))
        audio->playCue(NOLIMIT);                    // play sound
    ship1.update(frameTime);
    ship2.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Spacewar::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();

    backdrop.draw();                        // add the backdrop to the scene
    ship1.draw();                           // add the spaceship to the scene
    ship2.draw();                           // add the spaceship to the scene

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Spacewar::releaseAll()
{
    ship1Texture.onLostDevice();
    ship2Texture.onLostDevice();
    backTexture.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Spacewar::resetAll()
{
    backTexture.onResetDevice();
    ship1Texture.onResetDevice();
    ship2Texture.onResetDevice();

    Game::resetAll();
    return;
}
