// Programming 2D Games
// Copyright (c) 2011, 2013 by: 
// Charles Kelly
// boat.h v1.2

#ifndef _BOAT_H                 // Prevent multiple definitions if this 
#define _BOAT_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Boat;

#include "entity.h"
#include "constants.h"

namespace boatNS
{
    const int   WIDTH = 64;                 // image width (each frame)
    const int   HEIGHT = 64;                // image height
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/6 - HEIGHT;
    const int   EDGE_TOP = -10;             // For BOX and ROTATE_BOX collison.
    const int   EDGE_BOTTOM = 10;           // "   relative to center
    const int   EDGE_LEFT = -28;            // "
    const int   EDGE_RIGHT = 28;            // "
    const float SPEED = 200;                // pixels per second
    const float MAX_SPEED = 1000;
    const float ROTATION_RATE = 3.14f;      // radians per second
    const float MASS = 50.0f;               // mass
    const int   TEXTURE_COLS = 0;			// texture has 0 columns
    const int   START_FRAME = 0;            // animation starting frame
    const int   END_FRAME = 0;              // animation ending frame
    const float ANIMATION_DELAY = 0.1f;     // time between frames
	const float BOUNCINESS = 0.5f;			// how bouncy is the boat
}

// inherits from Entity class
class Boat : public Entity
{
public:
    // constructor
    Boat();

    // inherited member functions
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);
    // Draw Image using color as filter. Default color is WHITE.
    // Use zoom to scale boat for drawing.
    virtual void draw(float zoom, COLOR_ARGB color = graphicsNS::WHITE) 
    { 
        float x, y, scale;
        x = getX();         // save current x, y and scale
        y = getY();
        scale = getScale();
        setX(x * zoom);     // set x, y and scale
        setY(y * zoom);
        setScale(zoom);
        Image::draw(color, 0);  // draw scaled boat
        setX(x);            // restore x, y and scale
        setY(y);
        setScale(scale);
    }

    void repair();

    // new functions
    void setVelocityX(float v)  {velocity.x = v;}
    void setVelocityY(float v)  {velocity.y = v;}
};
#endif

