// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Star.cpp v1.0

#include "Star.h"
using namespace starNS;

//=============================================================================
// default constructor
//=============================================================================
Star::Star() : Entity()
{
    spriteData.width    = WIDTH;      // size of 1 image
    spriteData.height   = HEIGHT;
    spriteData.x    = X;              // location on screen
    spriteData.y    = Y;
    radius          = COLLISION_RADIUS;
    mass            = MASS;
    startFrame      = START_FRAME;    // first frame of ship animation
    endFrame        = END_FRAME;      // last frame of ship animation
    cols            = TEXTURE_COLS;
    setCurrentFrame(startFrame);
    alphaOff = D3DCOLOR_ARGB( 255,255,255,255);
    alphaOn  = D3DCOLOR_ARGB( 0,255,255,255);
    oldFrame = newFrame = startFrame;
}

//=============================================================================
// Draw the star
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//=============================================================================
void Star::draw()
{
    if (!visible || graphics == NULL)
        return;
    // get fresh texture incase onReset() was called
    spriteData.texture = textureManager->getTexture();
    setCurrentFrame(oldFrame);
    graphics->drawSprite(spriteData,alphaOff);
    setCurrentFrame(newFrame);
    graphics->drawSprite(spriteData,alphaOn);
}

//=============================================================================
// update
// frameTime is used to regulate the speed of animation
// Animate the star by fading out the old frame and fading in the new
//=============================================================================
void Star::update(float frameTime)
{
    if (endFrame - startFrame > 0)              // if animated sprite
    {
        animTimer += frameTime;                 // total elapsed time

        if(animTimer > ANIM_DELAY)
        {
            if(alphaOn == D3DCOLOR_ARGB(255,255,255,255))
            {
                oldFrame = currentFrame++;
                if (currentFrame > endFrame)
                    currentFrame = startFrame;
                newFrame = currentFrame;
                alphaOff = D3DCOLOR_ARGB( 255,255,255,255);
                alphaOn  = D3DCOLOR_ARGB( 0,255,255,255);
            }
            else
            {
                alphaOff -= D3DCOLOR_ARGB( 1,0,0,0);
                alphaOn  += D3DCOLOR_ARGB( 1,0,0,0);
            }
            animTimer -= ANIM_DELAY;
        }
    }
}
