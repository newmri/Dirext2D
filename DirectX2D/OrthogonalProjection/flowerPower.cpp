// Programming 2D Games
// Copyright (c) 2011,2012,2013 by: 
// Charles Kelly
// flowerPower.cpp v2.1
// flowerPower is the class we create.
// Supports scrolling along X and Y axis.
// Butterfly collides with post
// Last modified: Feb-10-2013

#include "flowerPower.h"
using namespace flowerPowerNS;

//=============================================================================
// Constructor
//=============================================================================
FlowerPower::FlowerPower()
{
    mapX = 0;
    mapY = 0;
    menuOn = true;
}

//=============================================================================
// Destructor
//=============================================================================
FlowerPower::~FlowerPower()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
//=============================================================================
void FlowerPower::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // map textures
    if (!tileTextures.initialize(graphics,TILE_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

    // object textures
    if (!objectTextures.initialize(graphics,OBJECT_TEXTURES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing object textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // tile image
    if (!tile.initialize(graphics,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&tileTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
    tile.setFrames(0, 0);
    tile.setCurrentFrame(0);

    // butterfly
    if (!butterfly.initialize(this,butterflyNS::WIDTH,butterflyNS::HEIGHT,butterflyNS::TEXTURE_COLS,&objectTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing butterfly"));
    butterfly.setFrames(butterflyNS::FLYING_START_FRAME, butterflyNS::FLYING_END_FRAME);
    butterfly.setCurrentFrame(butterflyNS::FLYING_START_FRAME);
    // Start butterfly in center
    butterfly.setX(GAME_WIDTH/2 - butterflyNS::WIDTH);
    butterfly.setY(GAME_HEIGHT/2 - butterflyNS::HEIGHT);

    // postTop
    if (!postTop.initialize(this,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&tileTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing postTop"));
    postTop.setFrames(POST_TOP, POST_TOP);
    postTop.setCurrentFrame(POST_TOP);
    postTop.setCollisionType(entityNS::ROTATED_BOX);
    postTop.setEdge(POST_EDGE);
    // Set post position
    postTop.setX(POST_X);
    postTop.setY(POST_TOP_Y);

    // postBottom
    if (!postBottom.initialize(this,TEXTURE_SIZE,TEXTURE_SIZE,TEXTURE_COLS,&tileTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing postBottom"));
    postBottom.setFrames(POST_BOTTOM, POST_BOTTOM);
    postBottom.setCurrentFrame(POST_BOTTOM);
    postBottom.setCollisionType(entityNS::ROTATED_BOX);
    postBottom.setEdge(POST_EDGE);
    // Set post position
    postBottom.setX(POST_X);
    postBottom.setY(POST_BOTTOM_Y);
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void FlowerPower::update()
{
    float butterflyX, butterflyY;

    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
        }
    } 

    butterfly.update(frameTime);        // fly the butterfly
    postTop.update(frameTime);
    postBottom.update(frameTime);

    // Scroll map left and right
    butterflyX = butterfly.getX();
    if(butterflyX < 0)                  // if butterfly off screen left
    {
        mapX += (abs(butterfly.getVelocity().x)+0.1f) * frameTime;  // scroll map right
        butterfly.setX(0);              // put butterfly at left edge
    }
    // if butterfly off screen right
    else if(butterflyX > GAME_WIDTH - butterfly.getWidth())
    {
        mapX -= (abs(butterfly.getVelocity().x)+10.0f) * frameTime;  // scroll map left
        // put butterfly at right edge
        butterfly.setX((float)(GAME_WIDTH - butterfly.getWidth()));
    }

    // Scroll map up and down
    butterflyY = butterfly.getY();
    if(butterflyY < 0)                  // if butterfly off screen top
    {
        mapY -= butterfly.getVelocity().y * frameTime;  // scroll map down
        butterfly.setY(0);              // put butterfly at top edge
    }
    // if butterfly off screen bottom
    else if(butterflyY > GAME_HEIGHT - butterfly.getHeight())
    {
        mapY -= butterfly.getVelocity().y * frameTime;  // scroll map up
        // put butterfly at bottom edge
        butterfly.setY((float)(GAME_HEIGHT - butterfly.getHeight()));
    }

    if(mapX > 0)    // if map past left edge
    {
        mapX = 0;   // stop at left edge of map
        butterfly.setVelocityX(0);  // stop butterfly
    }
    // if map past right edge
    else if(mapX < (-MAP_WIDTH * TEXTURE_SIZE) + GAME_WIDTH)
    {
        // stop at right edge of map
        mapX = (-MAP_WIDTH * TEXTURE_SIZE) + GAME_WIDTH;
        butterfly.setVelocityX(0);  // stop butterfly
    }
    if(mapY > 0)    // if map past top edge
    {
        mapY = 0;   // stop at top edge of map
        butterfly.setVelocityY(0);  // stop butterfly
    }
    // if map past bottom edge
    else if(mapY < (-MAP_HEIGHT * TEXTURE_SIZE) + GAME_HEIGHT)
    {
        // stop at bottom edge of map
        mapY = (-MAP_HEIGHT * TEXTURE_SIZE) + GAME_HEIGHT;
        butterfly.setVelocityY(0);  // stop butterfly
    }

    // move post with map
    postTop.setX(POST_X + mapX);
    postTop.setY(POST_TOP_Y + mapY);
    postBottom.setX(POST_X + mapX);
    postBottom.setY(POST_BOTTOM_Y + mapY);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FlowerPower::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void FlowerPower::collisions()
{
    VECTOR2 collisionVector;
    // if collision between butterfly and postTop
    if(butterfly.collidesWith(postTop, collisionVector))
        butterfly.bounce(collisionVector,postTop);
    // if collision between butterfly and postBottom
    if(butterfly.collidesWith(postBottom, collisionVector))
        butterfly.bounce(collisionVector,postBottom);
}

//=============================================================================
// Render game items
//=============================================================================
void FlowerPower::render()
{
    graphics->spriteBegin();

    for(int row=0; row<MAP_HEIGHT; row++)       // for each row of map
    {
        tile.setY( (float)(row*TEXTURE_SIZE) ); // set tile Y
        for(int col=0; col<MAP_WIDTH; col++)    // for each column of map
        {
            if(tileMap[row][col] >= 0)          // if tile present
            {
                tile.setCurrentFrame(tileMap[row][col]);    // set tile texture
                tile.setX( (float)(col*TEXTURE_SIZE) + mapX );  // set tile X
                tile.setY( (float)(row*TEXTURE_SIZE) + mapY );  // set tile Y
                // if tile on screen
                if( (tile.getX() > -TEXTURE_SIZE && tile.getX() < GAME_WIDTH) &&
                    (tile.getY() > -TEXTURE_SIZE && tile.getY() < GAME_HEIGHT) )
                    tile.draw();                // draw tile
            }
        }
    }

    // draw post
    postTop.draw();
    postBottom.draw();

    // draw butterfly
    butterfly.draw();

    if(menuOn)
        menu.draw();

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void FlowerPower::releaseAll()
{
    menuTexture.onLostDevice();
    tileTextures.onLostDevice();
    objectTextures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void FlowerPower::resetAll()
{
    tileTextures.onResetDevice();
    objectTextures.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
