// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 version 2.0
// Last modification: April-13-2013

#ifndef _GRAVITY_H              // Prevent multiple definitions if this
#define _GRAVITY_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "ball.h"
#include "paddle.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class Gravity : public Game
{
private:
    // game items
    TextureManager backdropTexture; // backdrop texture
    TextureManager ballTexture;     // ball texture
    TextureManager paddleTexture;   // paddle texture
    Ball    ball;                   // ball
    Paddle  paddle;                 // paddle
    Image   backdrop;               // backdrop image

public:
    // Constructor
    Gravity();
    // Destructor
    virtual ~Gravity();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
