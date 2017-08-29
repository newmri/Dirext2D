// Programming 2D Games
// Copyright (c) 2011,2013 by: 
// Charles Kelly
// tileCollision.h v1.0

#ifndef _TILECOLLISION_H        // Prevent multiple definitions if this 
#define _TILECOLLISION_H        // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class TileCollision;

#include <string>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "entity.h"
#include "boat.h"

namespace tileCollisionNS
{
    const char FONT[] = "Arial Bold";  // font
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const int TILE_SIZE = 64;
    const int MAP_HEIGHT = 64;
    const int MAP_WIDTH = 16;
    const float SCROLL_RATE = 10;
    const float MAX_BOAT_SPEED = 20;
    const USHORT EMPTY = 0;
    const USHORT MAX_COLLISION_TILE = 25;
    const int TILE_EDGE_TOP = -TILE_SIZE/2;  // for tile collision
    const int TILE_EDGE_BOTTOM = TILE_SIZE/2;
    const int TILE_EDGE_LEFT = -TILE_SIZE/2;
    const int TILE_EDGE_RIGHT = TILE_SIZE/2;
    const RECT TILE_EDGE = {TILE_EDGE_LEFT, TILE_EDGE_TOP, TILE_EDGE_RIGHT, TILE_EDGE_BOTTOM};
    const float TILE_MASS = 10000.0f;
}

// tileCollision is the class we create, it inherits from the Game class
class TileCollision : public Game
{
private:
    // game items
    TextureManager menuTexture;
    TextureManager tileTextures;        // tile textures
    TextureManager boatTexture;
    Image   tile;
    Image   menu;
    Entity  tileEntity;                 // for tile collision
    Boat    boat;
    std::ifstream mapIn;                // used to read map
    float   mapX, mapY;
    float   zoom;
    bool    menuOn;
    // Tile numbers
    // 0 empty, 1-17 land, 50 water
    USHORT tileMap[tileCollisionNS::MAP_HEIGHT][tileCollisionNS::MAP_WIDTH];

public:
	// Constructor
    TileCollision();
    // Destructor
    virtual ~TileCollision();
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
