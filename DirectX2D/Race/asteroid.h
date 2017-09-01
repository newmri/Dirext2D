// Programming 2D Games
// Copyright (c) 2012 by: 
// Charles Kelly
// asteroid.h v1.0

#ifndef _ASTEROID_H             // Prevent multiple definitions if this 
#define _ASTEROID_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Asteroid;

#include "entity.h"
#include "constants.h"

namespace asteroidNS
{
    const int   WIDTH = 256;                // image width
    const int   HEIGHT = 256;               // image height
    const int   COLLISION_RADIUS = 100;     // for circular collision
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/2 - HEIGHT/2;
    const int   TEXTURE_COLS = 4;           // texture has 8 columns
    const int   START_FRAME = 0;
    const int   END_FRAME = 15;
    const float ANIM_DELAY = 0.1f;
    const float ROTATION_RATE = .4f;        // degrees/sec
    const float SPEED = 100.0f;             // approach speed
    const float SCALE1_DISTANCE = 100.0f;   // the asteroid distance where scale factor is 1.0
}

class Asteroid : public Entity
{
private:
    float   rotation;                       // rotation amount
    float   speed;                          // approach speed
    float   x,y,z;                          // asteroid x,y,z axis location
public:
    // constructor
    Asteroid();
    void  disable() {visible = false; active = false;}
    void  enable()  {visible = true; active = true;}

    // Draw Ship
    void draw();

    // Update the animation. frameTime is used to regulate the speed.
    virtual void update(float frameTime);
    // Set rotation amount
    void setRotation(float r)   {rotation = r;}
    // Set x
    void setX(float newX)   {x = newX;}
    // Set y
    void setY(float newY)   {y = newY;}
    // Set z
    void setZ(float newZ)   {z = newZ;}
    // Set speed
    void setSpeed(float s)  {speed = s;}
    // Move in y direction
    void moveY(float dist)  {y += dist;}
    // Move in x direction
    void moveX(float dist)  {x += dist;}

    // Get x
    float getX()    {return x;}
    // Get y
    float getY()    {return y;}
    // Get z
    float getZ()    {return z;}
    // Get speed
    float getSpeed()    {return speed;}
};
#endif

