// Programming 2D Games
// Copyright (c) 2011,2012,2013 by: 
// Charles Kelly
// flowerPower.h v2.1
// Last modified: Feb-10-2013

#ifndef _FLOWERPOWER_H          // Prevent multiple definitions if this 
#define _FLOWERPOWER_H          // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "butterfly.h"

namespace flowerPowerNS
{
    const char FONT[] = "Arial Bold";  // font
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const int TEXTURE_SIZE = 128;
    const int TEXTURE_COLS = 4;
    const int MAP_HEIGHT = 8;
    const int MAP_WIDTH = 16;
    const float SCROLL_RATE = 10;
    const float MAX_BUTTERFLY_SPEED = 20;
    const int POST_TOP = 11;
    const int POST_BOTTOM = 15;
    const float POST_X = TEXTURE_SIZE*7;        // post position in map
    const float POST_TOP_Y = TEXTURE_SIZE*6;    //  "
    const float POST_BOTTOM_Y = TEXTURE_SIZE*7; //  "
    // collision box {left, top, right ,bottom} relative to center
    const RECT POST_EDGE = {-26,-64,26,64};

    // map tile legend 
    // C=cloud, G=grass, F=flower, W=web, S=seed, L=leaf, P=post
    const int C1=0,  C2=1,  F1T=2, F2T=3;
    const int G1=4,  G2=5,  F1B=6, F2B=7;
    const int W1=8,  W2=9,  S=10,  PT=11;
    const int CL=12, CR=13, L=14,  PB=15;
    const int __ = -1;                  // empty tile
    
    // The world map
    const int tileMap[MAP_HEIGHT][MAP_WIDTH] = {
        __, C1, __, __, C2, __, __, __, __, __, C2, __, __, C1, __, __,
        __, __, __, CL, CR, __, C1, __, __, __, __, __, __, __, __, __,
        __, __, C2, __, __, __, __, __, __, __, __, C2, __, __, __, __,
        __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
        __, C1, __, __, C2, __, __, __, CL, CR, __, __, __, __, C1, __,
        __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
        __, __, __,F1T, __, __, __, __, __,F2T, __, __, __, __,F2T,F1T,
        G2, G1, G2,F1B, G1, G1, G2, __, G1,F2B, G2, __,  L, G2,F2B,F1B
    };
}

// flowerPower is the class we create, it inherits from the Game class
class FlowerPower : public Game
{
private:
    // game items
    TextureManager menuTexture;
    TextureManager tileTextures;    // tiles
    TextureManager objectTextures;  // objects
    Image   tile;
    Image   menu;
    Butterfly butterfly;
    Entity  postTop;                // collision with post
    Entity  postBottom;
    float   mapX, mapY;
    bool    menuOn;

public:
	// Constructor
    FlowerPower();
    // Destructor
    virtual ~FlowerPower();
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
