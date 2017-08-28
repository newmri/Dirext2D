// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 plane.cpp v1.0

#include "plane.h"

//=============================================================================
// default constructor
//=============================================================================
Plane::Plane() : Entity()
{
    spriteData.width = planeNS::PLANE_SIZE;     // size
    spriteData.height = planeNS::PLANE_SIZE;
    spriteData.x = planeNS::X;                  // location on screen
    spriteData.y = planeNS::Y;
    spriteData.rect.bottom = planeNS::PLANE_SIZE; // rectangle to select parts of an image
    spriteData.rect.right = planeNS::PLANE_SIZE;
    spriteData.angle = 0;
    velocity.x = 0;                             // velocity
    velocity.y = planeNS::SPEED;
    frameDelay = planeNS::ANIMATION_DELAY;
    startFrame = planeNS::PLANE_START_FRAME;    // first frame of ship animation
    endFrame     = planeNS::PLANE_END_FRAME;    // last frame of ship animation
    currentFrame = startFrame;
}

//=============================================================================
// Initialize the Plane.
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//      *textureM = pointer to TextureManager object
// Post: returns true if successful, false if failed
//=============================================================================
bool Plane::initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// Draw the plane and reflection
//=============================================================================
void Plane::draw()
{
    // Draw reflection
    planeY = spriteData.y;          // save plane Y
    spriteData.y = planeNS::RELECTION_MIN_Y + planeNS::PLANE_MAX_Y - spriteData.y;
    frame = getCurrentFrame();      // current frame of plane sprite
    setCurrentFrame(frame + planeNS::REFLECTION_FRAME_OFFSET);  // set frame for reflection
    // Draw reflection using 25% alpha
    Entity::draw(spriteData, graphicsNS::ALPHA25);
    setCurrentFrame(frame);         // restore plane's frame
    spriteData.y = planeY;          // restore plane Y

    // Draw plane
    Entity::draw();
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Plane::update(float frameTime)
{
    Entity::update(frameTime);
    if (input->isKeyDown(UP_KEY))       // if increase altitude
    {
        spriteData.y = spriteData.y - frameTime * planeNS::PLANE_VERTICAL_SPEED;
        if (spriteData.y < 0)
            spriteData.y = 0;
    }
    if (input->isKeyDown(DOWN_KEY))     // if decrease altitude
    {
        spriteData.y = spriteData.y + frameTime * planeNS::PLANE_VERTICAL_SPEED;
        if (spriteData.y > planeNS::PLANE_MAX_Y)
            spriteData.y = planeNS::PLANE_MAX_Y;
    }
    if (input->isKeyDown(LEFT_KEY))     // if turn left
    {
        spriteData.angle -= frameTime * planeNS::ROTATION_RATE;  // apply rotation
        if(spriteData.angle < 0)
            spriteData.angle += (float)PI*2;
    }
    if (input->isKeyDown(RIGHT_KEY))    // if turn right
    {
        spriteData.angle += frameTime * planeNS::ROTATION_RATE;  // apply rotation
        if(spriteData.angle > PI)
            spriteData.angle -= (float)PI*2;
    }
    // update plane's velocity
    velocity.x = (float)cos(spriteData.angle) * planeNS::SPEED;
    velocity.y = (float)sin(spriteData.angle) * planeNS::SPEED;
}
