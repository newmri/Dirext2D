// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// asteroidRace.cpp v0.8.1
// This class is the core of the game

#include "asteroidRace.h"
using namespace asteroidRaceNS;

//=============================================================================
// Constructor
//=============================================================================
AsteroidRace::AsteroidRace()
{
    menuOn = true;
    countDownOn = false;
    roundOver = false;
    shipScore = 0;
    //shipScored = false;
    initialized = false;
    asteroids.resize(NUMA);
    for(int i=0; i<NUMA; i++)  // create asteroid objects
        asteroids[i] = new Asteroid;
}

//=============================================================================
// Destructor
//=============================================================================
AsteroidRace::~AsteroidRace()
{
    for(int i=0; i<NUMA; i++)  // delete asteroid objects
        SAFE_DELETE(asteroids[i]);
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void AsteroidRace::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // initialize DirectX fonts
    fontBig.initialize(graphics, FONT_BIG_SIZE, false, false, FONT);
    fontBig.setFontColor(FONT_COLOR);
    fontScore.initialize(graphics, FONT_SCORE_SIZE, false, false, FONT);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // nebula texture
    if (!nebulaTexture.initialize(graphics,NEBULA_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

    // main game textures
    if (!gameTextures.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

    // asteroid textures
    if (!asteroidTextures.initialize(graphics,ASTEROID_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing asteroid textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // nebula image
    if (!nebula.initialize(graphics,0,0,0,&nebulaTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

    // ship
    if (!ship.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
    ship.setFrames(shipNS::START_FRAME, shipNS::END_FRAME);
    ship.setCurrentFrame(shipNS::START_FRAME);
    ship.setColorFilter(SETCOLOR_ARGB(255,230,230,255));   // light blue, used for shield
    // Start ship in center of screen
    ship.setX(GAME_WIDTH/2 - shipNS::WIDTH/2);
    ship.setY(GAME_HEIGHT/2 - shipNS::HEIGHT/2);

    // asteroids
    for(int i=0; i<NUMA; i++)
    {
        if (!asteroids[i]->initialize(this, asteroidNS::WIDTH, asteroidNS::HEIGHT, asteroidNS::TEXTURE_COLS, &asteroidTextures))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing asteroid"));
        asteroids[i]->setFrames(asteroidNS::START_FRAME, asteroidNS::END_FRAME);
        asteroids[i]->setCurrentFrame(rand()%asteroidNS::END_FRAME+1);
        asteroids[i]->setX((rand()%(GAME_WIDTH+GAME_WIDTH/2))-GAME_WIDTH/4.0f);
        asteroids[i]->setY((rand()%(GAME_HEIGHT+GAME_HEIGHT/2))-GAME_HEIGHT/4.0f);
        asteroids[i]->setZ(1000.0f+(i*100.0f));
        asteroids[i]->setRotation(((rand()%101)-50)*0.01f);
    }

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void AsteroidRace::update()
{
    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
            roundStart();
        }
    } 
    else if(countDownOn)
    {
        countDownTimer -= frameTime;
        if(countDownTimer <= 0)
            countDownOn = false;
    } 
    ship.setLeft(false);
    ship.setRight(false);
    ship.setUp(false);
    ship.setDown(false);
    if(input->isKeyDown(shipNS::UP_KEY) || input->getGamepadDPadUp(0))      // if ship move up
        ship.moveUp(frameTime);
    if(input->isKeyDown(shipNS::DOWN_KEY) || input->getGamepadDPadDown(0))  // if ship move down
        ship.moveDown(frameTime);
    if(input->isKeyDown(shipNS::LEFT_KEY) || input->getGamepadDPadLeft(0))  // if ship move left
        ship.moveLeft(frameTime);
    if(input->isKeyDown(shipNS::RIGHT_KEY) || input->getGamepadDPadRight(0))    // ship move right
        ship.moveRight(frameTime);

    // Update the entities
    ship.update(frameTime);
    for(UINT i=0; i<asteroids.size(); i++)
    {
        if(ship.getUp())
            asteroids[i]->moveY(TURN_SPEED*frameTime);
        if(ship.getDown())
            asteroids[i]->moveY(-TURN_SPEED*frameTime);
        if(ship.getLeft())
            asteroids[i]->moveX(TURN_SPEED*frameTime);
        if(ship.getRight())
            asteroids[i]->moveX(-TURN_SPEED*frameTime);
        asteroids[i]->update(frameTime);
    }
    if(asteroids[0]->getZ() < 0.0)
    {
        asteroids[0]->setCurrentFrame(rand()%asteroidNS::END_FRAME+1);
        asteroids[0]->setX((rand()%(GAME_WIDTH+GAME_WIDTH/2))-GAME_WIDTH/4.0f);
        asteroids[0]->setY((rand()%(GAME_HEIGHT+GAME_HEIGHT/2))-GAME_HEIGHT/4.0f);
        asteroids[0]->setZ(10000.0f);
        asteroids[0]->setRotation(((rand()%101)-50)*0.01f);
        asteroids.push_back(asteroids[0]);
        asteroids.pop_front();
    }
}

//=============================================================================
// Start a new round of play
//=============================================================================
void AsteroidRace::roundStart()
{
    //countDownTimer = COUNT_DOWN;
    //countDownOn = true;
    //roundOver = false;
    //shipScored = false;
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void AsteroidRace::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void AsteroidRace::collisions()
{
    VECTOR2 vector;
    for(UINT i=0; i<asteroids.size(); i++)
    {
        if(asteroids[i]->getZ() < SHIP_Z && asteroids[i]->getZ() > SHIP_Z/2)
            if(ship.collidesWith(*asteroids[i],vector))
                ship.explode();
    }
}

//=============================================================================
// Render game items
//=============================================================================
void AsteroidRace::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    nebula.draw();                          // display Orion nebula
    if(menuOn)
        menu.draw();
    else
    {
        if(countDownOn)
        {
            _snprintf_s(buffer, BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
            fontBig.print(buffer,COUNT_DOWN_X,COUNT_DOWN_Y);
        }
        else
        {
            for(int i=asteroids.size()-1; i>=0; i--)
                asteroids[i]->draw();
            ship.draw();
        }
    }

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void AsteroidRace::releaseAll()
{
    asteroidTextures.onLostDevice();
    gameTextures.onLostDevice();
    nebulaTexture.onLostDevice();
    menuTexture.onLostDevice();
    fontScore.onLostDevice();
    fontBig.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void AsteroidRace::resetAll()
{
    fontBig.onResetDevice();
    fontScore.onResetDevice();
    menuTexture.onResetDevice();
    nebulaTexture.onResetDevice();
    gameTextures.onResetDevice();
    asteroidTextures.onResetDevice();
    Game::resetAll();
    return;
}
