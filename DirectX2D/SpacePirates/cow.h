// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// cow.h v1.0

#ifndef _COW_H                  // Prevent multiple definitions if this 
#define _COW_H                  // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace cowNS
{
    const int   WIDTH = 32;                 // image width (each frame)
    const int   HEIGHT = 32;                // image height
    const float MASS = 100.0f;              // mass
    const int   TEXTURE_COLS = 8;           // texture has 8 columns
    const int   START_FRAME = 53;           // cow starts at frame 0
    const int   END_FRAME = 53;             // cow is not animated
    const int   SHIELD_START_FRAME = 24;    // shield start frame
    const int   SHIELD_END_FRAME = 27;      // shield end frame
    const float SHIELD_ANIMATION_DELAY = 0.1f; // time between frames
    const float TORPEDO_DAMAGE = 0;         // amount of damage caused by torpedo
    const float SHIP_DAMAGE = 0;            // damage caused by collision with a ship
    const float DRAG = 0.05f;               // cow slow down rate (drag)
}

// inherits from Entity class
class Cow : public Entity
{
private:
    float   oldX, oldY, oldAngle;
    Image   shield;

public:
    // constructor
    Cow();

    // inherited member functions
    virtual void draw();
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);

    // update position
    void update(float frameTime);

    // damage with WEAPON
    void damage(WEAPON);

    // new member functions
    
    // move out of collision
    void toOldPosition()            
    {
        spriteData.x = oldX; 
        spriteData.y = oldY, 
        spriteData.angle = oldAngle;
    }
};
#endif

