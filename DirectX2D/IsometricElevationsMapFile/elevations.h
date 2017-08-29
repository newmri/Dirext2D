// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Elevations.h Isometric Diamond Demo v1.1
//   The map data is read from the file map1.txt

#ifndef _ELEVATIONS_H           // Prevent multiple definitions if this 
#define _ELEVATIONS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"

namespace elevationsNS
{
    // Map textures
    const int TEXTURE_COLS = 4;
    const int TEXTURE_SIZE = 64;
    const int MAP_SIZE = 16;
    const int SCREEN_X = GAME_WIDTH/2-TEXTURE_SIZE/2;
    const int SCREEN_Y = GAME_HEIGHT/8;

    // Object textures
    const int TEXTURE2_COLS = 2;
    const int TEXTURE2_SIZE = 128;
    const int TREE0_FRAME = 0;
    const int TREE1_FRAME = 1;
    const int TREE_OFFSET_X = -32;  // add to map X to get tree X
    const int TREE_OFFSET_Y = -96;  // add to map Y to get tree Y
    const int TREE0_SHADOW = 2;
    const int TREE1_SHADOW = 3;
    const float TREE_SHADOW_DEGREES = 335;
    const float TREE_SHADOW_X = -21;  // offset after rotation
    const float TREE_SHADOW_Y = 5;
}

// Elevations is the class we create, it inherits from the Game class
class Elevations : public Game
{
private:
    // game items
    TextureManager textures;    // textures
    TextureManager textures2;   // object texture
    Image   mapTile;
    Image   tree;
    // Maps
    int tileMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];
    int heightMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];
    int objectMap[elevationsNS::MAP_SIZE][elevationsNS::MAP_SIZE];    // 0=empty, 1=Tree0, 2=Tree1

public:
	// Constructor
    Elevations();
    // Destructor
    virtual ~Elevations();
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
