// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 planet.h v1.0

#ifndef _PLANET_H               // Prevent multiple definitions if this 
#define _PLANET_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planetNS
{
    const int   WIDTH = 128;                // image width
    const int   HEIGHT = 128;               // image height
    const int   COLLISION_RADIUS = 120/2;   // for circular collision
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float MASS = 1.0e14f;             // mass
    const int   TEXTURE_COLS = 1;           // texture has 1 column
    const int   START_FRAME = 0;            // starts at frame 0
    const int   END_FRAME = 0;              // no animation
}

class Planet : public Entity // inherits from Entity class
{
public:
    // constructor
    Planet();
};
#endif

