// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 plane.h v1.0

#ifndef _PLANE_H                // Prevent multiple definitions if this 
#define _PLANE_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace planeNS
{
    const int   PLANE_SIZE = 64;            // the texture size
    const int   PLANE_START_FRAME = 0;      // frame numbers
    const int   PLANE_END_FRAME = 1;
    // add to current frame to get reflection frame
    const int   REFLECTION_FRAME_OFFSET = 2;    
    const int   TEXTURE_COLS = 2;           // number of columns in texture image
    const int   PLANE_VERTICAL_SPEED = 64;  // climb/descend rate
    const int   PLANE_MIN_ALTITUDE = 10;
    const int   PLANE_MAX_Y = GAME_HEIGHT/2 - PLANE_MIN_ALTITUDE; // the plane's maximum Y coordinate
    const int   RELECTION_MIN_Y = PLANE_MAX_Y + PLANE_MIN_ALTITUDE;
    const int   X = GAME_WIDTH/2;           // the plane's starting location
    const int   Y = GAME_HEIGHT/4;
    const float ROTATION_RATE = (float)PI;  // radians per second
    const float SPEED = 100;                // 100 pixels per second
    const float ANIMATION_DELAY = 0.01f;    // time between frames
}

// inherits from Entity class
class Plane : public Entity
{
private:
    float   planeY;         // temp storage for plane Y
    int     frame;          // temp storage for current frame

public:
    // constructor
    Plane();

    // inherited member functions

    // draw the plane and reflection
    virtual void draw();

    // Initialize the plane
    // Pre: *gamePtr = pointer to Game object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
    //      *textureM = pointer to TextureManager object
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    // update the plane's animation and position
    void update(float frameTime);
};
#endif

