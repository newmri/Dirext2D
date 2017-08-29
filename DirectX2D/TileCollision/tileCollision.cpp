// Programming 2D Games
// Copyright (c) 2011,2013 by: 
// Charles Kelly
// tileCollision.cpp v1.4
// Last modification: Mar-1-2015

#include "tileCollision.h"
using namespace tileCollisionNS;

//=============================================================================
// Constructor
//=============================================================================
TileCollision::TileCollision() : Game()
{
    mapX = 0;
    mapY = 0;
    zoom = 1.0f;
    menuOn = true;
}

//=============================================================================
// Destructor
//=============================================================================
TileCollision::~TileCollision()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// initializes the game
//=============================================================================
void TileCollision::initialize(HWND hwnd)
{
    Game::initialize(hwnd);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_TEXTURE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // tile textures
    if (!tileTextures.initialize(graphics,TILES))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

    // boat texture
    if (!boatTexture.initialize(graphics,BOAT_TEXTURE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing boat texture"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // tile image
    if (!tile.initialize(graphics,0,0,0,&tileTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));

    // load map
    mapIn.open(MAP);
    if(!mapIn)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error loading map data"));
    for(int row=0; row<MAP_HEIGHT; row++)
    {
        for(int col=0; col<MAP_WIDTH; col++)
            mapIn >> tileMap[row][col];
    }
    mapY = -TILE_SIZE*(MAP_HEIGHT-3);

    // boat
    if (!boat.initialize(this,boatNS::WIDTH,boatNS::HEIGHT,boatNS::TEXTURE_COLS,&boatTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing boat"));
    boat.setFrames(boatNS::START_FRAME, boatNS::END_FRAME);
    boat.setCurrentFrame(boatNS::START_FRAME);
    boat.setMass(boatNS::MASS);
    boat.setDegrees(-90.0f);
    // Start boat at bottom
    boat.setX(static_cast<float>(GAME_WIDTH/2 + boat.getWidth()*1));
    boat.setY(static_cast<float>(GAME_HEIGHT - boat.getHeight()*3));
    boat.setCollisionType(entityNS::ROTATED_BOX);

    // tileEntity, used for collision testing
    tileEntity.initialize(this,TILE_SIZE,TILE_SIZE,1,&tileTextures);
    tileEntity.setEdge(TILE_EDGE);  // for tile collision
    tileEntity.setCollisionType(entityNS::BOX);
    tileEntity.setMass(TILE_MASS);
    tileEntity.setNoBounce(true);   // tiles do not move because of a collision

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void TileCollision::update()
{
    float boatX, boatY;

    if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
        }
    } 

    if(input->wasKeyPressed(VK_ADD) || input->wasKeyPressed(VK_OEM_PLUS))
    {
        zoom += 0.1f;
        if(zoom > 1.0f)
            zoom = 1.0f;
    }
    else if(input->wasKeyPressed(VK_SUBTRACT) || input->wasKeyPressed(VK_OEM_MINUS))
    {
        zoom -= 0.1f;
        if(zoom < 0.4f)
            zoom = 0.4f;
    }

    boat.update(frameTime);
    tileEntity.update(frameTime);

    // Scroll map left and right
    boatX = boat.getX();
    if(boatX < TILE_SIZE*2)         // if boat near left edge
    {
        mapX += TILE_SIZE*2-boatX;  // scroll map right
        boat.setX(TILE_SIZE*2);     // reposition boat
    }
    // if boat near right edge
    else if(boatX*zoom > GAME_WIDTH-TILE_SIZE*3)
    {
        mapX -= (boatX*zoom) - (GAME_WIDTH-TILE_SIZE*3);   // scroll map left
        boat.setX((float)((GAME_WIDTH-TILE_SIZE*3)/zoom)); // limit X
    }

    // Scroll map up and down
    boatY = boat.getY();
    if(boatY < TILE_SIZE*2)         // if boat near top
    {
        mapY += TILE_SIZE*2-boatY;  // scroll map down
        boat.setY(TILE_SIZE*2);     // reposition boat
    }
    // if boat near bottom
    else if(boatY > GAME_HEIGHT-TILE_SIZE*3)
    {
        mapY -= boatY-(GAME_HEIGHT-TILE_SIZE*3);    // scroll map up
        boat.setY(boat.getY()-(boatY-(GAME_HEIGHT-TILE_SIZE*3)));   // reposition boat
    }

    if(mapX > TILE_SIZE*2)  // if map past left edge
    {
        mapX = TILE_SIZE*2; // stop at left edge of map
    }
    // if map past right edge
    else if(mapX < (-MAP_WIDTH * TILE_SIZE) + GAME_WIDTH-TILE_SIZE*2)
    {
        // stop at right edge of map
        mapX = (-MAP_WIDTH * TILE_SIZE) + GAME_WIDTH-TILE_SIZE*2;
        boat.setVelocityX(0);  // stop boat
    }
    if(mapY > GAME_HEIGHT/2)  // if map top below screen middle
    {
        mapY = GAME_HEIGHT/2;  // limit Y
    }
    // if map bottom above screen middle
    else if(mapY < (-MAP_HEIGHT * TILE_SIZE) + GAME_HEIGHT/2)
    {
        // limit Y
        mapY = (-MAP_HEIGHT * TILE_SIZE) + GAME_HEIGHT/2;
    }
    tile.setScale(zoom);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void TileCollision::ai()
{}

//=============================================================================
// Handle collisions
// An Entity object (tileEntity) is used to represent a game tile. That allows
// using the standard collision tests in the game engine. The tile map is named
// tileMap. It holds a number representing the type of the tile in each grid.
// The tile numbers that are less than or equal to MAX_COLLISION_TILE may be
// collided with. All other tile numbers do not collide.
//
// The fist thing this collision code does is calculate the current row and
// column of the boat. The map can scroll. The amount the map has scrolled is
// contained in mapX and mapY. A collision test is performed between the boat
// and the nearby tiles. The boat's position is defined by its top left corner
// so we don't need to check for collisions with any tiles that are above the
// current row. The possible collision tiles is the tile to the left, the
// current tile, the tile to the right and the three closest tiles in the row
// below the boat. The screen position of each of those tiles is calculated and
// used to set the parameters of tileEntity. A standard collision test is used
// between the boat and tileEntity. The tileEntity object is reused for each
// tile.
//=============================================================================
void TileCollision::collisions()
{
    VECTOR2 collisionVector;
    // Calculate map grid location of boat
    UINT boatRow = static_cast<UINT>((boat.getY() - mapY)/TILE_SIZE);
    UINT boatCol = static_cast<UINT>((boat.getX() - mapX)/TILE_SIZE);
    // check for collision with left, current, right, bottom left, bottom and bottom right tiles.
    // if tile left of boat may be collided with
    if(boatCol > 0 && boatRow < MAP_HEIGHT && tileMap[boatRow][boatCol-1] <= MAX_COLLISION_TILE)
    {
        tileEntity.setX(static_cast<float>((boatCol-1)*TILE_SIZE+mapX));    // position tileEntity
        tileEntity.setY(static_cast<float>(boatRow*TILE_SIZE+mapY));
        tileEntity.setRotatedBoxReady(false);        
        // if collision between boat and tile
        if(boat.collidesWith(tileEntity, collisionVector))
            boat.bounce(collisionVector,tileEntity);
    }
    // if current tile may be collided with
    if(boatCol >= 0 && boatRow < MAP_HEIGHT && tileMap[boatRow][boatCol] <= MAX_COLLISION_TILE)
    {
        tileEntity.setX(static_cast<float>(boatCol*TILE_SIZE+mapX));     // position tileEntity
        tileEntity.setY(static_cast<float>(boatRow*TILE_SIZE+mapY));
        tileEntity.setRotatedBoxReady(false);        
        // if collision between boat and tile
        if(boat.collidesWith(tileEntity, collisionVector))
            boat.bounce(collisionVector,tileEntity);
    }
    // if tile right of boat may be collided with
    if(boatCol < MAP_WIDTH-1 && boatRow < MAP_HEIGHT && tileMap[boatRow][boatCol+1] <= MAX_COLLISION_TILE)
    {
        tileEntity.setX(static_cast<float>((boatCol+1)*TILE_SIZE+mapX));   // position tileEntity
        tileEntity.setY(static_cast<float>(boatRow*TILE_SIZE+mapY));
        tileEntity.setRotatedBoxReady(false);        
        // if collision between boat and tile
        if(boat.collidesWith(tileEntity, collisionVector))
            boat.bounce(collisionVector,tileEntity);
    }
    // if tile bottom left of boat may be collided with
    if(boatCol > 0 && boatRow < MAP_HEIGHT-1 && tileMap[boatRow+1][boatCol-1] <= MAX_COLLISION_TILE)
    {
        tileEntity.setX(static_cast<float>((boatCol-1)*TILE_SIZE+mapX));   // position tileEntity
        tileEntity.setY(static_cast<float>((boatRow+1)*TILE_SIZE+mapY));
        tileEntity.setRotatedBoxReady(false);        
        // if collision between boat and tile
        if(boat.collidesWith(tileEntity, collisionVector))
            boat.bounce(collisionVector,tileEntity);
    }
    // if tile below boat may be collided with
    if(boatRow < MAP_HEIGHT-1 && tileMap[boatRow+1][boatCol] <= MAX_COLLISION_TILE)
    {
        tileEntity.setX(static_cast<float>(boatCol*TILE_SIZE+mapX));     // position tileEntity
        tileEntity.setY(static_cast<float>((boatRow+1)*TILE_SIZE+mapY));
        tileEntity.setRotatedBoxReady(false);        
        // if collision between boat and tile
        if(boat.collidesWith(tileEntity, collisionVector))
            boat.bounce(collisionVector,tileEntity);
    }
    // if tile bottom right of boat may be collided with
    if(boatCol < MAP_WIDTH-1 && boatRow < MAP_HEIGHT-1 && tileMap[boatRow+1][boatCol+1] <= MAX_COLLISION_TILE)
    {
        tileEntity.setX(static_cast<float>((boatCol+1)*TILE_SIZE+mapX));   // position tileEntity
        tileEntity.setY(static_cast<float>((boatRow+1)*TILE_SIZE+mapY));
        tileEntity.setRotatedBoxReady(false);        
        // if collision between boat and tile
        if(boat.collidesWith(tileEntity, collisionVector))
            boat.bounce(collisionVector,tileEntity);
    }
}

//=============================================================================
// Render game items
//=============================================================================
void TileCollision::render()
{
    graphics->spriteBegin();

    for(int row=0; row<MAP_HEIGHT; row++)       // for each row of map
    {
        //tile.setY( (float)(row*TILE_SIZE*zoom) ); // set tile Y
        for(int col=0; col<MAP_WIDTH; col++)    // for each column of map
        {
            if(tileMap[row][col] >= 0)          // if tile present
            {
                tile.setX( (float)(col*(TILE_SIZE*zoom)) + mapX*zoom );  // set tile X
                tile.setY( (float)(row*(TILE_SIZE*zoom)) + mapY*zoom );  // set tile Y
                // if tile on screen
                if( (tile.getX() > -TILE_SIZE && tile.getX() < GAME_WIDTH) &&
                    (tile.getY() > -TILE_SIZE && tile.getY() < GAME_HEIGHT) )
                    tile.draw(UINT(tileMap[row][col]));             // draw tile
            }
        }
    }

    // draw boat
    boat.draw(zoom);

    if(menuOn)
        menu.draw();

    graphics->spriteEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void TileCollision::releaseAll()
{
    menuTexture.onLostDevice();
    tileTextures.onLostDevice();
    boatTexture.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void TileCollision::resetAll()
{
    boatTexture.onResetDevice();
    tileTextures.onResetDevice();
    menuTexture.onResetDevice();
    
    Game::resetAll();
    return;
}
