// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.cpp Parallax Scroll Demo
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
    space.setScale(SPACE_SCALE);

    // moon images
    for(int i=0; i<4; i++)
    {
        if (!moons[i].initialize(graphics,MOON_SIZE,MOON_SIZE,4,&textures1))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moons"));
        moons[i].setFrames(MOON1_FRAME+i, MOON1_FRAME+i);
        moons[i].setCurrentFrame(MOON1_FRAME+i);
        // Start moons in center
        moons[i].setX(GAME_WIDTH/2 - MOON_SIZE/2);
        moons[i].setY(GAME_HEIGHT/2 - MOON_SIZE/2);
    }

    // planet
    if (!planet.initialize(graphics,PLANET_SIZE,PLANET_SIZE,2,&textures1))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
    planet.setFrames(PLANET_FRAME, PLANET_FRAME);
    planet.setCurrentFrame(PLANET_FRAME);
    // Start planet in centet
    planet.setX(GAME_WIDTH/2 - PLANET_SIZE/2);
    planet.setY(GAME_HEIGHT/2 - PLANET_SIZE/2);

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

    // Update the entities position for parallax scroll
    ship1.update(frameTime);
    // move planet along X
    planet.setX(planet.getX() - frameTime * ship1.getVelocity().x);
    // move planet along Y
    planet.setY(planet.getY() - frameTime * ship1.getVelocity().y);
    planet.update(frameTime);
    for(int i=0; i<4; i++)      // move moons
    {
        // reduce scroll speed by 20% for each moon
        moons[i].setX(moons[i].getX() - frameTime * ship1.getVelocity().x * 0.2f * (4-i));
        moons[i].setY(moons[i].getY() - frameTime * ship1.getVelocity().y * 0.2f * (4-i));
    }

    // move space along X
    space.setX(space.getX() - frameTime * ship1.getVelocity().x*0.1f);
    // move space along Y
    space.setY(space.getY() - frameTime * ship1.getVelocity().y*0.1f);

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

    // Wrap space image around at edge
    // The Scrolling Bitmap example uses if statements so the space image is
    // only drawn when necessary. This code always draws the space image even
    // when it may be off screen.
    space.draw();
    space.setX(space.getX() + SPACE_WIDTH);
    space.draw();
    space.setY(space.getY() + SPACE_HEIGHT);
    space.draw();
    space.setX(x);
    space.draw();
    space.setY(y);

    for (int i=3; i>=0; i--)
        moons[i].draw();                    // draw moons, back to front
    planet.draw();                          // draw the planet

    ship1.draw();                           // draw the spaceships

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
    textures1.onResetDevice();
    spaceTexture.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
