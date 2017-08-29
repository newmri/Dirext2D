// Programming 2D Games
// Copyright (c) 2011,2013 by: 
// Charles Kelly
// boat.cpp v1.0

#include "boat.h"

//=============================================================================
// default constructor
//=============================================================================
Boat::Boat() : Entity()
{
    spriteData.width = boatNS::WIDTH;   // boat size
    spriteData.height = boatNS::HEIGHT;
    spriteData.x = boatNS::X;           // location on screen
    spriteData.y = boatNS::Y;
    spriteData.rect.bottom = boatNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = boatNS::WIDTH;
    edge.top    = boatNS::EDGE_TOP;             // BOX collision edges
    edge.bottom = boatNS::EDGE_BOTTOM;
    edge.left   = boatNS::EDGE_LEFT;
    edge.right  = boatNS::EDGE_RIGHT;
    velocity.x = 0;
    velocity.y = 0;
    frameDelay = boatNS::ANIMATION_DELAY;
    startFrame = boatNS::START_FRAME;           // first frame of animation
    endFrame     = boatNS::END_FRAME;           // last frame of animation
    currentFrame = startFrame;
    radius = boatNS::WIDTH / 3.0;
    collisionType = entityNS::ROTATED_BOX;
    mass = boatNS::MASS;
	bounciness = boatNS::BOUNCINESS;
}

//=============================================================================
// Initialize the Boat.
// Post: returns true if successful, false if failed
//=============================================================================
bool Boat::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Boat::update(float frameTime)
{
    Entity::update(frameTime);

    velocity.x -= velocity.x * 0.5f*frameTime;  // water drag (50% velocity/second)
    velocity.y -= velocity.y * 0.5f*frameTime;

    if (input->isKeyDown(UP_KEY))       // if up arrow
    {
        velocity.x += (float)cos(spriteData.angle) * boatNS::SPEED * frameTime;
        velocity.y += (float)sin(spriteData.angle) * boatNS::SPEED * frameTime;
        if(velocity.y < -boatNS::MAX_SPEED)
            velocity.y = -boatNS::MAX_SPEED;
    }
    if (input->isKeyDown(DOWN_KEY))     // if down arrow
    {
        velocity.x -= (float)cos(spriteData.angle) * boatNS::SPEED * frameTime;
        velocity.y -= (float)sin(spriteData.angle) * boatNS::SPEED * frameTime;
        if(velocity.y > boatNS::MAX_SPEED)
            velocity.y = boatNS::MAX_SPEED;
    }
    if (input->isKeyDown(LEFT_KEY))     // if left
    {
        spriteData.angle -= frameTime * boatNS::ROTATION_RATE;
    }
    if (input->isKeyDown(RIGHT_KEY))    // if right
    {
        spriteData.angle += frameTime * boatNS::ROTATION_RATE;
    }
    spriteData.x += velocity.x * frameTime;
    spriteData.y += velocity.y * frameTime;
}
