// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Elevations.cpp Isometric Elevation Demo v1.0
// Elevations is the class we create.

#include "elevations.h"
using namespace elevationsNS;

//=============================================================================
// Constructor
//=============================================================================
Elevations::Elevations()
{
    for(int row=0; row<MAP_SIZE; row++)     // clear maps
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            tileMap[row][col]=0;
            heightMap[row][col]=0;
            objectMap[row][col]=0;   // 0=empty, 1=Tree0, 2=Tree1
        }
    }
}

//=============================================================================
// Destructor
//=============================================================================
Elevations::~Elevations()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
// Throws GameError on error
//=============================================================================
void Elevations::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    std::string str;

    // map textures
    if (!textures.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures"));

    // object textures
    if (!textures2.initialize(graphics,TEXTURES2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing textures2"));

    // load map data
    try{
        std::ifstream mapData("map1.txt");
        if(!mapData)                            // if file error
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error loading map data"));
        std::getline(mapData,str);              // skip 1st comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> tileMap[row][col];   // read Tile map
        std::getline(mapData,str);
        std::getline(mapData,str);              // skip 2nd comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> heightMap[row][col]; // read Height map
        std::getline(mapData,str);
        std::getline(mapData,str);              // skip 3rd comment line
        for(int row=0; row<MAP_SIZE; row++)
            for(int col=0; col<MAP_SIZE; col++)
                mapData >> objectMap[row][col]; // read Object map
    }
    catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error loading map data"));
    }

    // map tile image
    mapTile.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&textures);
    mapTile.setFrames(0, 0);
    mapTile.setCurrentFrame(0);

    // Tree image
    tree.initialize(graphics,TEXTURE2_SIZE,TEXTURE2_SIZE,TEXTURE2_COLS,&textures2);
    tree.setFrames(TREE0_FRAME, TREE0_FRAME);
    tree.setCurrentFrame(TREE0_FRAME);
}

//=============================================================================
// Update all game items
//=============================================================================
void Elevations::update()
{
    mapTile.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Elevations::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Elevations::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Elevations::render()
{
    graphics->spriteBegin();

    // Draw map in Isometric Diamond
    for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            mapTile.setCurrentFrame(tileMap[row][col]);
            mapTile.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ));
            mapTile.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                  heightMap[row][col] * HEIGHT_CHANGE));
            mapTile.draw();
        }
    }

    // Draw Objects,  0=empty, 1=Tree0, 2=Tree1
    float treeX = 0, treeY = 0;
    for(int row=0; row<MAP_SIZE; row++)
    {
        for(int col=0; col<MAP_SIZE; col++)
        {
            switch(objectMap[row][col])
            {
            case 1:     // Tree0
                tree.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) +
                                    TREE_OFFSET_X);
                tree.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                   heightMap[row][col] * HEIGHT_CHANGE) + TREE_OFFSET_Y);
                if(col%2)
                    tree.flipHorizontal(true);
                // draw shadow
                tree.setCurrentFrame(TREE0_SHADOW);
                tree.setDegrees(TREE_SHADOW_DEGREES);
                treeX = tree.getX();
                treeY = tree.getY();
                tree.setX(treeX + TREE_SHADOW_X);
                tree.setY(treeY + TREE_SHADOW_Y);
                tree.draw(graphicsNS::ALPHA25 & graphicsNS::BLACK);
                tree.setX(treeX);   // restore X
                tree.setY(treeY);   // restore Y
                // draw tree
                tree.setDegrees(0);
                tree.setCurrentFrame(TREE0_FRAME);
                tree.draw();
                tree.flipHorizontal(false);
                break;
            case 2:     // Tree1
                tree.setX((float)( SCREEN_X - (row*TEXTURE_SIZE/2) + (col*TEXTURE_SIZE/2) ) +
                                    TREE_OFFSET_X);
                tree.setY((float)( SCREEN_Y + (row*TEXTURE_SIZE/4) + (col*TEXTURE_SIZE/4) -
                                   heightMap[row][col] * HEIGHT_CHANGE) + TREE_OFFSET_Y);
                if(col%2)
                    tree.flipHorizontal(true);
                // draw shadow
                tree.setCurrentFrame(TREE1_SHADOW);
                tree.setDegrees(TREE_SHADOW_DEGREES);   // rotate shadow
                treeX = tree.getX();                    // save tree x,y
                treeY = tree.getY();
                tree.setX(treeX + TREE_SHADOW_X);       // position shadow
                tree.setY(treeY + TREE_SHADOW_Y);
                tree.draw(graphicsNS::ALPHA25 & graphicsNS::BLACK); // draw shadow
                tree.setX(treeX);                       // restore tree x,y
                tree.setY(treeY);
                // draw tree
                tree.setDegrees(0);
                tree.setCurrentFrame(TREE1_FRAME);
                tree.draw();
                tree.flipHorizontal(false);
                break;
            }
        }
    }

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Elevations::releaseAll()
{
    textures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Elevations::resetAll()
{
    textures.onResetDevice();

    Game::resetAll();
    return;
}
