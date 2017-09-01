// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// lineDemo.h

#ifndef _LINEDEMO_H           // Prevent multiple definitions if this 
#define _LINEDEMO_H           // file is included in more than one place
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
class LineDemo : public Game
{
private:
    // game items
    LP_DXLINE line;     // line pointer

public:
    // Constructor
    LineDemo();
    // Destructor
    virtual ~LineDemo();
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
