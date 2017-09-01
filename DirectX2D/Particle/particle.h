// Programming 2D Games
// Copyright (c) 2014 by: 
// Charles Kelly
// particle.h v1.0

#ifndef _PARTICLE_H             // Prevent multiple definitions if this 
#define _PARTICLE_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

// inherits from Entity class
class Particle : public Entity
{
private:
    float life;                 // Time to live
    float gravity;

public:
    // constructor
    Particle(float g=0);        // g=gravity

    // update position
    void update(float frameTime);

    // set life
    void setLife(float l) 
    {
        if(l>=0)
            life = l;
    }

    // set gravity
    void setGravity(float g) {gravity = g;}
};
#endif

