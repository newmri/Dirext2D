// Programming 2D Games
// Copyright (c) 2014 by: 
// Charles Kelly
// particle.cpp v1.0

#include "particle.h"

//=============================================================================
// default constructor
//=============================================================================
Particle::Particle(float g) : Entity(), gravity(g)
{
    life = 0;       // seconds to live
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Particle::update(float frameTime)
{
    if(getIntersecting() == false)              // if this entity is not intersecting another entity
        velocity.y += frameTime * gravity;      // gravity

    Entity::update(frameTime);
    spriteData.x += frameTime * velocity.x;     // move along X 
    spriteData.y += frameTime * velocity.y;     // move along Y

    life -= frameTime;
    if(life <= 0)       // if time to live <= 0
    {
        life=0;
        visible = false;
    }
}
