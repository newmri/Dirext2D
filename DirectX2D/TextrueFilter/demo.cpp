// Programming 2D Games
// Copyright (c) 2013 by: 
// Charles Kelly
// Demo of texture filtering.
// demo.cpp v1.2

#include "demo.h"

//=============================================================================
// Constructor
//=============================================================================
Demo::Demo()
{}

//=============================================================================
// Destructor
//=============================================================================
Demo::~Demo()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Demo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    demoX = 1.0f;
    demoScale = 1.0f;
    filter = D3DTEXF_LINEAR;

    // Image1 texture
    if (!image1Texture.initialize(graphics,TEXTURE1_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image1 texture"));
    // Image2 texture
    if (!image2Texture.initialize(graphics,TEXTURE2_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image2 texture"));
    // Image3 texture
    if (!image3Texture.initialize(graphics, TEXTURE3_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image3 texture"));

    // image1
    if (!image1.initialize(graphics,0, 0, 1, &image1Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image1"));
    image1.setY(GAME_HEIGHT/4);
    //image1.setFrames(IMAGE_FRAME, IMAGE_FRAME); // animation frames (not animated)
    //image1.setCurrentFrame(IMAGE_FRAME);        // starting frame

    // image2
    if (!image2.initialize(graphics,0, 0, 1, &image2Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image2"));
    image2.setY(GAME_HEIGHT/2);
    //image2.setFrames(IMAGE_FRAME, IMAGE_FRAME); // animation frames (not animated)
    //image2.setCurrentFrame(IMAGE_FRAME);        // starting frame

    // image3, same as image2 but with transparent border
    if (!image3.initialize(graphics, 0, 0, 1, &image3Texture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image2"));
    image3.setY(GAME_HEIGHT/2);
    //image2.setFrames(IMAGE_FRAME, IMAGE_FRAME); // animation frames (not animated)
    //image2.setCurrentFrame(IMAGE_FRAME);        // starting frame

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Demo::update()
{
    demoX = image1.getX();
    if(input->wasKeyPressed(VK_RIGHT))              // if move right
    {
        demoX += 0.1f;
        if(demoX > 20.0f)
            demoX = 20.0f;
    }
    if(input->wasKeyPressed(VK_LEFT))               // if move left
    {
        demoX -= 0.1f;
        if(demoX < 0)
            demoX = 0;
    }
    image1.setX(demoX);
    image2.setX(demoX);
    image3.setX(demoX+50);

    if(input->wasKeyPressed(VK_ADD) || 
       input->wasKeyPressed(VK_OEM_PLUS))           // +
    {
        demoScale += 0.1f;
        if(demoScale > 6.0f)
            demoScale = 6.0f;
    }
    if(input->wasKeyPressed(VK_SUBTRACT) ||
       input->wasKeyPressed(VK_OEM_MINUS))          // -
    {
        demoScale -= 0.1f;
        if(demoScale < 0.1f)
            demoScale = 0.1f;
    }
    image1.setScale(demoScale);
    image2.setScale(demoScale);
    image3.setScale(demoScale);

    // Select filter type
    if(input->wasKeyPressed('F'))                   // 'F' or 'f'
    {
        switch(filter)
        {
            case D3DTEXF_POINT: filter = D3DTEXF_LINEAR; break;
            case D3DTEXF_LINEAR: filter = D3DTEXF_ANISOTROPIC; break;
            case D3DTEXF_ANISOTROPIC: filter = D3DTEXF_PYRAMIDALQUAD; break;
            case D3DTEXF_PYRAMIDALQUAD: filter = D3DTEXF_GAUSSIANQUAD; break;
            case D3DTEXF_GAUSSIANQUAD: filter = D3DTEXF_POINT;
        }
    }

    image1.update(frameTime);
    image2.update(frameTime);
    image3.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Demo::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Demo::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void Demo::render()
{
    const int BUF_SIZE = 80;
    static char buffer[BUF_SIZE];
    graphics->spriteBegin();                // begin drawing sprites

    graphics->get3Ddevice()->SetSamplerState(0,D3DSAMP_MINFILTER,filter);
    graphics->get3Ddevice()->SetSamplerState(0,D3DSAMP_MAGFILTER,filter);
    graphics->get3Ddevice()->SetSamplerState(0,D3DSAMP_MIPFILTER,filter);

    image1.draw();                          // add the image to the scene
    image2.draw();                          // add the image to the scene
    image3.draw();                          // add the image to the scene
    _snprintf_s(buffer, BUF_SIZE, "x=%.2f, scale=%.2f ", demoX, demoScale);
    dxFont.print(buffer,1,1);
    dxFont.print("<- ->    + -",1,9);
    dxFont.print("(F)ilter",50,16);
    switch(filter)
    {
        case D3DTEXF_POINT: dxFont.print("Point",50,25); break;
        case D3DTEXF_LINEAR: dxFont.print("Linear",50,25); break;
        case D3DTEXF_ANISOTROPIC: dxFont.print("Anisotropic",50,25); break;
        case D3DTEXF_PYRAMIDALQUAD: dxFont.print("Pyramidalquad",50,25); break;
        case D3DTEXF_GAUSSIANQUAD: dxFont.print("Gaussianquad",50,25);
    }

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Demo::releaseAll()
{
    image3Texture.onLostDevice();
    image2Texture.onLostDevice();
    image1Texture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Demo::resetAll()
{
    image1Texture.onResetDevice();
    image2Texture.onResetDevice();
    image3Texture.onResetDevice();
    Game::resetAll();
    return;
}
