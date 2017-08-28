// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.cpp Scrolling Bitmap Demo
// This class is the core of the game

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{
    menuOn = true;
    countDownOn = false;
    roundOver = false;
}

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

    // initialize DirectX fonts
    fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
    fontBig.setFontColor(spacewarNS::FONT_COLOR);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // space texture
    if (!spaceTexture.initialize(graphics,SPACE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));

    // game textures
    if (!textures1.initialize(graphics,TEXTURES1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // space image
    if (!space.initialize(graphics,0,0,0,&spaceTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
    space.setScale((float)SPACE_SCALE);


    // ship1
    if (!ship1.initialize(this,shipNS::WIDTH,shipNS::HEIGHT,shipNS::TEXTURE_COLS,&textures1))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));

    ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
    ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
    ship1.setColorFilter(SETCOLOR_ARGB(255,230,230,255));   // light blue, used for shield and torpedo
    ship1.setMass(shipNS::MASS);
    // Start ship in center
    ship1.setX(GAME_WIDTH/2 - shipNS::WIDTH);
    ship1.setY(GAME_HEIGHT/2 - shipNS::HEIGHT);
}

//=============================================================================
// Start a new round of play
//=============================================================================
void Spacewar::roundStart()
{
    space.setX(0);
    space.setY(0);

    // Start ship in center
    ship1.setX(GAME_WIDTH/2 - shipNS::WIDTH);
    ship1.setY(GAME_HEIGHT/2 - shipNS::HEIGHT);

    ship1.setDegrees(0);
    countDownTimer = spacewarNS::COUNT_DOWN;
    countDownOn = true;
    roundOver = false;
}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
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
    else 
    {
        if (ship1.getActive())
        {
            if (input->isKeyDown(SHIP1_FORWARD_KEY) || input->getGamepadDPadUp(0))   // if engine on
            {
                ship1.setEngineOn(true);
                audio->playCue(ENGINE1);
            }
            else
            {
                ship1.setEngineOn(false);
                audio->stopCue(ENGINE1);
            }
            ship1.rotate(shipNS::NONE);
            if (input->isKeyDown(SHIP1_LEFT_KEY) || input->getGamepadDPadLeft(0))   // if turn ship1 left
                ship1.rotate(shipNS::LEFT);
            if (input->isKeyDown(SHIP1_RIGHT_KEY) || input->getGamepadDPadRight(0)) // if turn ship1 right
                ship1.rotate(shipNS::RIGHT);
        }
        if(roundOver)
        {
            roundTimer -= frameTime;
            if(roundTimer <= 0)
                roundStart();
        }
    }

    // Update the entities
    ship1.update(frameTime);
    // move space in X direction opposite ship
    space.setX(space.getX() - frameTime * ship1.getVelocity().x);
    // move space in Y direction opposite ship
    space.setY(space.getY() - frameTime * ship1.getVelocity().y);

    // Wrap space image around at edge
    // if left edge of space > screen left edge
    if (space.getX() > 0)               
        // move space image left by SPACE_WIDTH
        space.setX(space.getX() - SPACE_WIDTH);
    // if space image off screen left
    if (space.getX() < -SPACE_WIDTH)
        // move space image right by SPACE_WIDTH
        space.setX(space.getX() + SPACE_WIDTH);
    // if top edge of space > screen top edge
    if (space.getY() > 0)
        // move space image up by SPACE_HEIGHT
        space.setY(space.getY() - SPACE_HEIGHT);
    // if space image off screen top
    if (space.getY() < -SPACE_HEIGHT)
        // move space image down by SPACE_IMAGE
        space.setY(space.getY() + SPACE_HEIGHT);
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
{
}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    float x = space.getX();
    float y = space.getY();

    graphics->spriteBegin();                // begin drawing sprites

    // Wrap space image around at edges
    space.draw();                           // draw at current location
    // if space image right edge visible
    if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
    {
        space.setX(space.getX() + SPACE_WIDTH); // wrap around to left edge
        space.draw();                           // draw again
    }
    // if space image bottom edge visible
    if (space.getY() < -SPACE_HEIGHT + (int)GAME_HEIGHT)
    {
        space.setY(space.getY() + SPACE_HEIGHT); // wrap around to top edge
        space.draw();                           // draw again
        space.setX(x);                          // restore x position
        // if space image right edge visible
        // wrap around to left edge
        if (x < -SPACE_WIDTH + (int)GAME_WIDTH)
            space.draw();                       // draw again
    }
    space.setX(x);      // restore x position
    space.setY(y);      // restore y position

    ship1.draw();       // draw the spaceship

    if(menuOn)
        menu.draw();
    if(countDownOn)
    {
        _snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
        fontBig.print(buffer,spacewarNS::COUNT_DOWN_X,spacewarNS::COUNT_DOWN_Y);
    }

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Spacewar::releaseAll()
{
    menuTexture.onLostDevice();
    spaceTexture.onLostDevice();
    textures1.onLostDevice();
    fontScore.onLostDevice();
    fontBig.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Spacewar::resetAll()
{
    fontBig.onResetDevice();
    fontScore.onResetDevice();
    textures1.onResetDevice();
    spaceTexture.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
