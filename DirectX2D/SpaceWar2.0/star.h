// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// star.h v1.0

#ifndef _STAR_H                 // Prevent multiple definitions if this 
#define _STAR_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace starNS
{
    const int   WIDTH = 128;                // image width
    const int   HEIGHT = 128;               // image height
    const int   COLLISION_RADIUS = 50;      // for circular collision
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float MASS = 1.0e14f;             // mass
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   START_FRAME = 16;
    const int   END_FRAME = 19;
    const float ANIM_DELAY = 5.0f/255.0f;
}

class Star : public Entity            // inherits from Entity class
{
private:
    DWORD alphaOff;
    DWORD alphaOn;
    int oldFrame, newFrame;

public:
    // constructor
    Star();
    void  disable() {visible = false; active = false;}
    void  enable()  {visible = true; active = true;}

    // Draw Star
    void draw();

    // Update the animation. frameTime is used to regulate the speed.
    virtual void update(float frameTime);
};
#endif

