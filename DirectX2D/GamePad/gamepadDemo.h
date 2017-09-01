// Programming 2D Games
// Copyright (c) 2013 by: 
// Charles Kelly
// gamepadDemo.h

#ifndef _CONTROLLERDEMO_H       // Prevent multiple definitions if this 
#define _CONTROLLERDEMO_H       // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <sstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class GamepadDemo : public Game
{
private:
    // game items
    TextureManager menuTexture, controllerTexture; // textures
    Image   menu;               // menu image
    Image   controller;         // controller image
    TextDX  *dxFont;            // DirectX font
    std::stringstream  message;
    int leftTrigger, leftTriggerUndead, rightTrigger, rightTriggerUndead;
    int leftThumbX, leftThumbXUndead, leftThumbY, leftThumbYUndead;
    int rightThumbX, rightThumbXUndead, rightThumbY, rightThumbYUndead;

public:
    // Constructor
    GamepadDemo();
    // Destructor
    virtual ~GamepadDemo();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void drawController(int n, int x, int y);
    void releaseAll();
    void resetAll();
};

#endif
