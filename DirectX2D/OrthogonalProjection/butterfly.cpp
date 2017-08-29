// Programming 2D Games
// Copyright (c) 2011,2012 by: 
// Charles Kelly
// butterfly.cpp v2.0 X and Y scrolling


#include "butterfly.h"
using namespace butterflyNS;

//=============================================================================
// default constructor
//=============================================================================
Butterfly::Butterfly() : Entity()
{
    spriteData.width = WIDTH;           // size of butterfly
    spriteData.height = HEIGHT;
    spriteData.x = X;                   // location on screen
    spriteData.y = Y;
    spriteData.rect.bottom = HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = WIDTH;
    edge.top    = EDGE_TOP;             // ROTATED_BOX collision edges
    edge.bottom = EDGE_BOTTOM;
    edge.left   = EDGE_LEFT;
    edge.right  = EDGE_RIGHT;
    velocity.x = 0;
    velocity.y = 0;
    frameDelay = FLYING_ANIMATION_DELAY;
    startFrame = FLYING_START_FRAME;     // first frame of animation
    endFrame     = FLYING_END_FRAME;     // last frame of animation
    currentFrame = startFrame;
    radius = WIDTH/2.0;
    collisionType = entityNS::ROTATED_BOX;
    direction = RIGHT;                   // direction of flight
    mass = MASS;
}

//=============================================================================
// Initialize the Butterfly.
// Post: returns true if successful, false if failed
//=============================================================================
bool Butterfly::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Butterfly::update(float frameTime)
{
    oldX = spriteData.x;                // save current position
    oldY = spriteData.y;
    Entity::update(frameTime);

    if (input->isKeyDown(UP_KEY))       // if up arrow
    {
        velocity.y -= frameTime * CLIMB_RATE;
        if(velocity.y < -CLIMB_RATE)
            velocity.y = -CLIMB_RATE;
    }
    if (input->isKeyDown(DOWN_KEY))     // if decrease altitude
    {
        velocity.y += frameTime * DROP_RATE;
        if(velocity.y > DROP_RATE)
            velocity.y = DROP_RATE;
    }
    spriteData.y += velocity.y * frameTime;

    if (input->isKeyDown(LEFT_KEY))     // if left
    {
        this->flipHorizontal(true);
        velocity.x -= frameTime * SPEED;
        if(velocity.x < -MAX_SPEED)
            velocity.x = -MAX_SPEED;
    }
    if (input->isKeyDown(RIGHT_KEY))    // if right
    {
        this->flipHorizontal(false);
        velocity.x += frameTime * SPEED;
        if(velocity.x > MAX_SPEED)
            velocity.x = MAX_SPEED;
    }
    spriteData.x += velocity.x * frameTime;
}

//=============================================================================
// Draw Image using color as filter. Default color is WHITE.
//=============================================================================
//void Butterfly::draw(COLOR_ARGB color)
//{
    //int flyingFrame = getCurrentFrame();
    //setCurrentFrame(butterflyNS::FLYING_BODY);
    //Image::draw(color);
    //setCurrentFrame(flyingFrame);
//    Image::draw(color);
//}