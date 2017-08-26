// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly

#include "paddle.h"

//=============================================================================
// default constructor
//=============================================================================
Paddle::Paddle() : Entity()
{
    spriteData.width = paddleNS::WIDTH;         // size of Ship1
    spriteData.height = paddleNS::HEIGHT;
    spriteData.x = paddleNS::X;                 // location on screen
    spriteData.y = paddleNS::Y;
    spriteData.rect.bottom = paddleNS::HEIGHT;  // rectangle to select parts of an image
    spriteData.rect.right = paddleNS::WIDTH;
    frameDelay = 1;
    startFrame = 0;                             // first frame of animation
    endFrame     = 0;                           // last frame of animation
    currentFrame = startFrame;
    edge.top = -paddleNS::HEIGHT/2;             // set collision edges
    edge.bottom = paddleNS::HEIGHT/2;
    edge.left = -paddleNS::WIDTH/2;
    edge.right = paddleNS::WIDTH/2;
    collisionType = entityNS::ROTATED_BOX;
    mass = paddleNS::MASS;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Paddle::update(float frameTime)
{
    Entity::update(frameTime);
    
    // Bounce off walls
    if (spriteData.x > GAME_WIDTH - paddleNS::WIDTH)  // if hit right screen edge
        spriteData.x = GAME_WIDTH - paddleNS::WIDTH;  // position at right screen edge
    else if (spriteData.x < 0)                    // else if hit left screen edge
        spriteData.x = 0;                           // position at left screen edge
}
