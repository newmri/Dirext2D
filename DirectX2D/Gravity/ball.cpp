// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 version 1.0

#include "ball.h"

//=============================================================================
// default constructor
//=============================================================================
Ball::Ball() : Entity()
{
    spriteData.width = ballNS::WIDTH;           // size of Ship1
    spriteData.height = ballNS::HEIGHT;
    spriteData.x = ballNS::X;                   // location on screen
    spriteData.y = ballNS::Y;
    spriteData.rect.bottom = ballNS::HEIGHT;    // rectangle to select parts of an image
    spriteData.rect.right = ballNS::WIDTH;
    velocity.x = ballNS::SPEED;                 // velocity X
    velocity.y = ballNS::SPEED;                 // velocity Y
    frameDelay = 1;
    startFrame = 0;                             // first frame of animation
    endFrame     = 0;                           // last frame of animation
    currentFrame = startFrame;
    radius = ballNS::WIDTH/2.0;
    collisionType = entityNS::CIRCLE;
    mass = ballNS::MASS;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Ball::update(float frameTime)
{
    Entity::update(frameTime);
    spriteData.x += frameTime * velocity.x;         // move along X 
    spriteData.y += frameTime * velocity.y;         // move along Y

    // Bounce off walls
    if (spriteData.x > GAME_WIDTH - ballNS::WIDTH)  // if hit right screen edge
    {
        spriteData.x = GAME_WIDTH - ballNS::WIDTH;  // position at right screen edge
        velocity.x = -velocity.x;                   // reverse X direction
    } else if (spriteData.x < 0)                    // else if hit left screen edge
    {
        spriteData.x = 0;                           // position at left screen edge
        velocity.x = -velocity.x;                   // reverse X direction
    }
    if (spriteData.y > GAME_HEIGHT - ballNS::HEIGHT) // if hit bottom screen edge
    {
        spriteData.y = GAME_HEIGHT - ballNS::HEIGHT; // position at bottom screen edge
        velocity.y -= ballNS::ENERGY_LOSS;
        if (velocity.y < ballNS::MIN_VY)            // if ball has small bounce
        {
            spriteData.y = GAME_HEIGHT/4;
            velocity.x = ballNS::SPEED;
        }
        velocity.y = -velocity.y;                   // reverse Y direction
    } else if (spriteData.y < 0)                    // else if hit top screen edge
    {
        spriteData.y = 0;                           // position at top screen edge
        velocity.y = -velocity.y;                   // reverse Y direction
    }

    velocity.y += frameTime * GRAVITY;              // gravity
}
