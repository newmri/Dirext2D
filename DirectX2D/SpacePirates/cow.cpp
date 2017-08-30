// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Cow.cpp v1.0

#include "cow.h"

//=============================================================================
// default constructor
//=============================================================================
Cow::Cow() : Entity()
{
    spriteData.width = cowNS::WIDTH;            // size of Cow
    spriteData.height = cowNS::HEIGHT;
    spriteData.rect.bottom = cowNS::HEIGHT;     // select part of an image
    spriteData.rect.right = cowNS::WIDTH;
    oldAngle = 0.0f;
    velocity.x = 0;                             // velocity X
    startFrame = cowNS::START_FRAME;            // first frame of Cow animation
    endFrame     = cowNS::END_FRAME;            // last frame of Cow animation
    currentFrame = startFrame;
    radius = cowNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
    active = true;
    visible = true;
    mass = cowNS::MASS;
}

//=============================================================================
// Initialize the Cow.
// Post: returns true if successful, false if failed
//=============================================================================
bool Cow::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    shield.initialize(gamePtr->getGraphics(), width, height, ncols, textureM);
    shield.setFrames(cowNS::SHIELD_START_FRAME, cowNS::SHIELD_END_FRAME);
    shield.setCurrentFrame(cowNS::SHIELD_START_FRAME);
    shield.setFrameDelay(cowNS::SHIELD_ANIMATION_DELAY);
    shield.setLoop(true);                       // loop animation
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw the Cow
//=============================================================================
void Cow::draw()
{
    Image::draw();              // draw Cow
    // draw shield using colorFilter 25% alpha
    shield.draw(spriteData, graphicsNS::ALPHA50 & colorFilter);
}


//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Cow::update(float frameTime)
{
    shield.update(frameTime);

    Entity::update(frameTime);
    oldX = spriteData.x;                        // save current position
    oldY = spriteData.y;
    oldAngle = spriteData.angle;

    spriteData.x += frameTime * velocity.x;     // move Cow along X 
    spriteData.y += frameTime * velocity.y;     // move Cow along Y

    velocity.x *= 1.0f - (cowNS::DRAG * frameTime);      // cow velocity slows
    velocity.y *= 1.0f - (cowNS::DRAG * frameTime);
}

//=============================================================================
// damage
//=============================================================================
void Cow::damage(WEAPON weapon)
{
    switch(weapon)
    {
    case TORPEDO:
        audio->playCue(TORPEDO_HIT);
        break;
    case SHIP:
        audio->playCue(COLLIDE);    // play sound
        break;
    case PLANET:
        audio->playCue(COLLIDE);    // play sound
        health = 0;
        shield.setVisible(false);
        active = false;
        visible = false;
        break;
    }
}

