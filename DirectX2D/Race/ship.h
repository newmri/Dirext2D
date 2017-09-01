// Programming 2D Games
// Copyright (c) 2012 by: 
// Charles Kelly
// ship.h v1.0

#ifndef _SHIP_H                 // Prevent multiple definitions if this 
#define _SHIP_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class Ship;

#include "entity.h"
#include "constants.h"

namespace shipNS
{
    const int   WIDTH = 256;                // image width
    const int   HEIGHT = 256;               // image height
    const int   COLLISION_RADIUS = 100;     // for circular collision
    const int   X = GAME_WIDTH/2 - WIDTH/2; // location on screen
    const int   Y = GAME_HEIGHT/2 - HEIGHT/2;
    const int   TEXTURE_COLS = 4;           // texture has 8 columns
    const int   START_FRAME = 0;
    const int   END_FRAME = 3;
    const int   EXPLODE_START = 8;
    const int   EXPLODE_END = 11;
    const float ANIM_DELAY = 0.1f;
    const UCHAR LEFT_KEY    = VK_LEFT;      // left arrow
    const UCHAR RIGHT_KEY   = VK_RIGHT;     // right arrow
    const UCHAR UP_KEY      = VK_UP;        // up arrow
    const UCHAR DOWN_KEY    = VK_DOWN;      // down arrow
    const float MOVE_SPEED  = 400;          // ship movement speed
    const float ROLL_RATE   = 100;          // ship roll rate
    const float MAX_ROLL    = 30;
}

class Ship : public Entity
{
private:
    bool    up, down, left, right;
    float   roll;
public:
    // constructor
    Ship();
    void  disable() {visible = false; active = false;}
    void  enable()  {visible = true; active = true;}

    // frameTime is used to regulate the speed of movement and animation
    void update(float frameTime);

    // Ship move
    void moveLeft(float frameTime)
    {
        roll -= frameTime * shipNS::ROLL_RATE;
        if(roll < -shipNS::MAX_ROLL)
            roll = -shipNS::MAX_ROLL;
        setDegrees(roll);
        spriteData.x -= frameTime*shipNS::MOVE_SPEED;
        if(spriteData.x < 0)
            spriteData.x = 0;
        else
            left = true;
    }
    void moveRight(float frameTime)
    {
        roll += frameTime * shipNS::ROLL_RATE;
        if(roll > shipNS::MAX_ROLL)
            roll = shipNS::MAX_ROLL;
        setDegrees(roll);
        spriteData.x += frameTime*shipNS::MOVE_SPEED;
        if(spriteData.x > GAME_WIDTH - shipNS::WIDTH)
            spriteData.x = GAME_WIDTH - shipNS::WIDTH;
        else
            right = true;
    }
    void moveUp(float frameTime)
    {
        spriteData.y -= frameTime*shipNS::MOVE_SPEED;
        if(spriteData.y < 0)
            spriteData.y = 0;
        else
            up = true;
    }
    void moveDown(float frameTime)
    {
        spriteData.y += frameTime*shipNS::MOVE_SPEED;
        if(spriteData.y > GAME_HEIGHT - shipNS::HEIGHT)
            spriteData.y = GAME_HEIGHT - shipNS::HEIGHT;
        else
            down = true;
    }

    void explode()
    {
        setFrames(shipNS::EXPLODE_START, shipNS::EXPLODE_END+1);
        setCurrentFrame(shipNS::EXPLODE_START);
        active = false;     // prevent collisions while exploding
    }

    void setUp(bool u)      {up = u;}
    void setDown(bool d)    {down = d;}
    void setLeft(bool l)    {left = l;}
    void setRight(bool r)   {right = r;}

    bool getUp()    {return up;}
    bool getDown()  {return down;}
    bool getLeft()  {return left;}
    bool getRight() {return right;}

};

#endif

