// Programming 2D Games
// Copyright (c) 2014 by: 
// Charles Kelly
// particleDemo.cpp v2.1
// Last modification: December-27-2014

#include "particleDemo.h"
using namespace particleDemoNS;

//=============================================================================
// Constructor
//=============================================================================
ParticleDemo::ParticleDemo()
{
    menuOn = true;
    menuAlpha = 255;        // used to fade menu, 255 = full on
    menuDisplay = MENU_DISPLAY_TIME;    // time to display menu before fade
    emitterX = EMITTER_X;
    emitterY = EMITTER_Y;
}

//=============================================================================
// Destructor
//=============================================================================
ParticleDemo::~ParticleDemo()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void ParticleDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    graphics->setBackColor(graphicsNS::BLACK);

    // Menu texture
    if (!menuTexture.initialize(graphics,MENU_TEXTURE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
    // Menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // Box texture
    if (!boxTexture.initialize(graphics,BOX1_TEXTURE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box1 texture"));

    // Box1 Entities
    for (int i = 0; i < BOX1_COUNT; i++)
    {
        if (!box1[i].initialize(this, BOX1_SIZE, BOX1_SIZE, 0, &boxTexture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box1"));
        box1[i].setMass(BOX1_MASS);
        box1[i].setCollisionType(entityNS::BOX);
        box1[i].setEdge(COLLISION_BOX1);
    }
    box1[0].setX(600);
    box1[0].setY(300);
    box1[1].setX(500);
    box1[1].setY(400);
    box1[2].setX(700);
    box1[2].setY(400);
    box1[3].setX(400);
    box1[3].setY(500);
    box1[4].setX(600);
    box1[4].setY(500);
    box1[5].setX(800);
    box1[5].setY(500);

    // Box2 texture
    if (!box2Texture.initialize(graphics,BOX2_TEXTURE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box2 texture"));

    // Box2 Entities
    for (int i = 0; i < BOX2_COUNT; i++)
    {
        if (!box2[i].initialize(this, BOX2_WIDTH, BOX2_HEIGHT, 0, &box2Texture))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing box2"));
        box2[i].setMass(BOX2_MASS);
        box2[i].setCollisionType(entityNS::ROTATED_BOX);
        box2[i].setEdge(COLLISION_BOX2);
    }
    box2[0].setX(500);
    box2[0].setY(325);
    box2[0].setDegrees(45);
    box2[1].setX(700);
    box2[1].setY(325);
    box2[1].setDegrees(-45);
    box2[2].setX(600);
    box2[2].setY(425);
    box2[2].setDegrees(-45);
    box2[3].setX(500);
    box2[3].setY(525);
    box2[3].setDegrees(45);
    box2[4].setX(700);
    box2[4].setY(525);
    box2[4].setDegrees(-45);

    // Particle texture
    if (!particleTextures.initialize(graphics,PARTICLE1_TEXTURE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing particle texture"));
    // Particle emitter
    particleEmitter1.initialize(this, &particleTextures);
    particleEmitter1.setX(EMITTER_X);
    particleEmitter1.setY(EMITTER_Y);
    particleEmitter1.setGravity(GRAVITY);
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void ParticleDemo::update()
{
    if (menuOn)
    {
        if(menuDisplay > 0)                 // if menu display timer > 0
            menuDisplay -= frameTime;       // subtract elapsed time from timer
        else
        {
            menuAlpha -= frameTime*MENU_FADE_RATE;  // fade out menu
            if(menuAlpha < 0)
                menuOn = false;             // do not display
        }
    }     
    
    for (int i = 0; i < BOX1_COUNT; i++)
        box1[i].update(frameTime);

    for (int i = 0; i < BOX2_COUNT; i++)
        box2[i].update(frameTime);
    box2[2].setDegrees(box2[2].getDegrees() + BOX2_ROTATION_RATE * frameTime);
    

    if(input->isKeyDown(VK_RIGHT))          // if right arrow
        emitterX += frameTime * EMITTER_SPEED;
    else if(input->isKeyDown(VK_LEFT))      // if left arrow
        emitterX -= frameTime * EMITTER_SPEED;
    if(input->isKeyDown(VK_UP))             // if up arrow
        emitterY -= frameTime * EMITTER_SPEED;
    else if(input->isKeyDown(VK_DOWN))      // if down arrow
        emitterY += frameTime * EMITTER_SPEED;

    if(emitterX > GAME_WIDTH + 50)
        emitterX = -50;
    else if(emitterX < -50)
        emitterX = GAME_WIDTH + 50;
    if(emitterY > GAME_HEIGHT + 50)
        emitterY = -50;
    else if(emitterY < -50)
        emitterY = GAME_HEIGHT + 50;
    particleEmitter1.setX(emitterX);        // position emitter
    particleEmitter1.setY(emitterY);
    particleEmitter1.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void ParticleDemo::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void ParticleDemo::collisions()
{
    for (int i = 0; i < BOX1_COUNT; i++)
        particleEmitter1.collidesWith(box1[i]);

    for (int i = 0; i < BOX2_COUNT; i++)
        particleEmitter1.collidesWith(box2[i]);
}

//=============================================================================
// Render game items
//=============================================================================
void ParticleDemo::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    for (int i = 0; i < BOX1_COUNT; i++)
        box1[i].draw(graphicsNS::NAVY);

    for (int i = 0; i < BOX2_COUNT; i++)
    {
        if (i == 2)
            box2[i].draw(graphicsNS::WHITE);
        else
            box2[i].draw(graphicsNS::GRAY);
    }

    particleEmitter1.draw();
    if(menuOn)
        menu.draw(SETCOLOR_ARGB(static_cast<int>(menuAlpha),255,255,255));

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void ParticleDemo::releaseAll()
{
    particleTextures.onLostDevice();
    box2Texture.onLostDevice();
    boxTexture.onLostDevice();
    menuTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void ParticleDemo::resetAll()
{
    Game::resetAll();
    menuTexture.onResetDevice();
    boxTexture.onResetDevice();
    box2Texture.onResetDevice();
    particleTextures.onResetDevice();
    return;
}
