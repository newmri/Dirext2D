// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 version 1.0

#ifndef ball_h               // Prevent multiple definitions if this 
#define ball_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace ballNS
{
    const int WIDTH = 60;                   // image width
    const int HEIGHT = 60;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float SPEED = 10;                // pixels per second
    const float ENERGY_LOSS = 100;          // energy loss on bounce (pixels/sec)
    const float MIN_VY = 1;                 // minumum Y velocity
    const float MASS = 1.0f;
}

// inherits from Entity class
class Ball : public Entity
{
public:
    // constructor
    Ball();

    // inherited member functions
    void update(float frameTime);
};
#endif

