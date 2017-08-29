// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// butterfly.h v1.0

#ifndef _BUTTERFLY_H            // Prevent multiple definitions if this 
#define _BUTTERFLY_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace butterflyNS
{
    const int   WIDTH = 128;                // image width (each frame)
    const int   HEIGHT = 128;               // image height
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/6 - HEIGHT;
    const int   EDGE_TOP = -50;             // For BOX and ROTATE_BOX collison.
    const int   EDGE_BOTTOM = 30;           // "   relative to center
    const int   EDGE_LEFT = -50;            // "
    const int   EDGE_RIGHT = 50;            // "
    const float SPEED = 50;                 // pixels per second
    const float MAX_SPEED = 100;
    const float MASS = 3.0f;                // mass
    enum DIRECTION {LEFT, RIGHT};           // direction
    const int   TEXTURE_COLS = 4;			// texture has 8 columns
    const int   FLYING_START_FRAME = 0;     // animation starting frame
    const int   FLYING_END_FRAME = 7;       // animation ending frame
    const float FLYING_ANIMATION_DELAY = 0.1f;    // time between frames
    const float FEEDING_ANIMATION_DELAY = 0.2f;
    const float CLIMB_RATE = 64;
    const float GLID_RATE = 16;
    const float DROP_RATE = 64;
}

// inherits from Entity class
class Butterfly : public Entity
{
private:
    float   oldX, oldY, oldAngle;
    float   rotation;                       // current rotation rate (radians/second)
    butterflyNS::DIRECTION direction;            // direction of rotation

public:
    // constructor
    Butterfly();

    // inherited member functions
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);
    void repair();

    // new functions
    void setVelocityX(float v)  {velocity.x = v;}
    void setVelocityY(float v)  {velocity.y = v;}
    // move butterfly out of collision
    //void toOldX() { spriteData.x = oldX; }
    //void toOldY() { spriteData.y = oldY; }
};
#endif

