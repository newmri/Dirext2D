// Programming 2D Games
// Copyright (c) 2013 by: 
// Charles Kelly
// demo.h v1.2

#ifndef _DEMO_H                 // Prevent multiple definitions if this 
#define _DEMO_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class Demo : public Game
{
private:
    // game items
    TextureManager image1Texture;   // texture1
    TextureManager image2Texture;   // texture2
    TextureManager image3Texture;   // texture3
    Image image1, image2, image3;   // images
    float demoScale;
    float demoX;
    _D3DTEXTUREFILTERTYPE filter;

public:
    // Constructor
    Demo();

    // Destructor
    virtual ~Demo();

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
