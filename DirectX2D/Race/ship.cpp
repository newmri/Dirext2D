// Programming 2D Games
// Copyright (c) 2012 by: 
// Charles Kelly
// Ship.cpp v0.8.1

#include "ship.h"
using namespace shipNS;

//=============================================================================
// Default constructor
//=============================================================================
Ship::Ship() : Entity()
{
    spriteData.width    = WIDTH;      // size of 1 image
    spriteData.height   = HEIGHT;
    spriteData.x    = X;              // location on screen
    spriteData.y    = Y;
    radius          = COLLISION_RADIUS;
    startFrame      = START_FRAME;    // first frame of ship animation
    endFrame        = END_FRAME;      // last frame of ship animation
    frameDelay      = ANIM_DELAY;
    cols            = TEXTURE_COLS;
    setCurrentFrame(startFrame);
    up = false;
    down = false;
    left = false;
    right = false;
    roll = 0;                           // roll angle
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Ship::update(float frameTime)
{
    Entity::update(frameTime);
    if(!left && !right)                 // if ship not turning
    {
        // 1.0 dead zone to prevent oscillations
        if(roll > 1.0f)                    // if ship rolled right
            roll -= frameTime * shipNS::ROLL_RATE;  // roll left
        else if(roll < -1.0f)               // if ship rolled left
            roll += frameTime * shipNS::ROLL_RATE;  // roll right
        setDegrees(roll);
    }

    if(currentFrame > EXPLODE_END)
    {
        setFrames(shipNS::START_FRAME, shipNS::END_FRAME);
        setCurrentFrame(shipNS::START_FRAME);
        active = true;              // enable collisions
    }
}

