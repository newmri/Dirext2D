// Programming 2D Games
// Copyright (c) 2012 by: 
// Charles Kelly
// Asteroid.cpp v1.0

#include "asteroid.h"
using namespace asteroidNS;

//=============================================================================
// default constructor
//=============================================================================
Asteroid::Asteroid() : Entity()
{
    spriteData.width    = WIDTH;      // size of 1 image
    spriteData.height   = HEIGHT;
    spriteData.x    = X;              // location on screen
    spriteData.y    = Y;
    radius          = COLLISION_RADIUS;
    startFrame      = START_FRAME;    // first frame of ship animation
    endFrame        = END_FRAME;      // last frame of ship animation
    cols            = TEXTURE_COLS;
    setCurrentFrame(startFrame);
    rotation        = 0;
    speed           = SPEED;
    x               = 0;
    y               = 0;
    z               = 10000;            // meters
}

//=============================================================================
// Draw the Asteroid
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//=============================================================================
void Asteroid::draw()
{
    if (!visible || graphics == NULL)
        return;
    // get fresh texture incase onReset() was called
    spriteData.texture = textureManager->getTexture();
    graphics->drawSprite(spriteData);
}

//=============================================================================
// update
// frameTime is used to regulate the speed of animation
//=============================================================================
void Asteroid::update(float frameTime)
{
    float deltaScale = SCALE1_DISTANCE/SHIP_Z;
    z -= frameTime * speed;
    if(z > 0)
    {
        deltaScale = SCALE1_DISTANCE/z;
        spriteData.scale = deltaScale;
    if(z < SHIP_Z)   // asteroid moves out of frame when close
    {
        x -= (GAME_WIDTH/2 - getCenterX())*deltaScale*frameTime;
        y -= (GAME_HEIGHT/2 - getCenterY())*deltaScale*frameTime;
    }
    } 
    spriteData.angle += frameTime * rotation;
    spriteData.x = x-spriteData.width/2*deltaScale;     // screen position
    spriteData.y = y-spriteData.height/2*deltaScale;
}
