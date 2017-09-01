// Programming 2D Games
// Copyright (c) 2014 by: 
// Charles Kelly
// particleDemo.h v2.0
// Last modification: December-25-2014  Merry Christmas


#ifndef _PARTICLEDEMO_H         // Prevent multiple definitions if this 
#define _PARTICLEDEMO_H         // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "particleEmitter.h"

// graphic images
const char MENU_TEXTURE[] = "pictures\\menu.png";        // menu
const char PARTICLE1_TEXTURE[] =  "pictures\\particle1.png";      // particle1 texture
const char BOX1_TEXTURE[] = "pictures\\box1.png";        // box1 texture
const char BOX2_TEXTURE[] = "pictures\\box2.png";        // box2 texture

namespace particleDemoNS
{
    const int MENU_FADE_RATE = 255; // amount to fade menu per second, 255 = 1 second to fade out
    const float EMITTER_X = 100;
    const float EMITTER_Y = 100;
    const float EMITTER_SPEED = 50;         // movement speed
    const float MENU_DISPLAY_TIME = 3.0f;   // time to display menu before fade

    const int BOX1_SIZE = 64;       // size in pixels
    const float BOX1_MASS = 3;
    const RECT COLLISION_BOX1 = {-32,-32,32,32};

    const int BOX2_WIDTH = 64;
    const int BOX2_HEIGHT = 16;
    const float BOX2_MASS = 5;
    const RECT COLLISION_BOX2 = {-32,-8,32,8};
    const float BOX2_ROTATION_RATE = 90;    // degrees per second

    const int BOX1_COUNT = 6;
    const int BOX2_COUNT = 5;

    const float GRAVITY = 1000.0f;  // gravity effect
}

//=============================================================================
// This class is the core of the game
//=============================================================================
class ParticleDemo : public Game
{
private:
    // game items
    TextureManager menuTexture;         // Menu texture
    TextureManager boxTexture;          // Box texture
    TextureManager box2Texture;         // Box2 texture
    TextureManager particleTextures;    // Particle texture
    Image   menu;                       // Menu Image object
    bool    menuOn;                     // true to display menu
    float   menuDisplay;                // time to display menu
    float   menuAlpha;                  // used to fade out menu
    Entity box1[particleDemoNS::BOX1_COUNT];  // Square Boxes
    Entity box2[particleDemoNS::BOX2_COUNT];  // Rectangle Boxes
    //Entity  box1A, box1B, box1C, box1D; // Square Boxes
    //Entity  box2A, box2B, box2C;        // Rectangle Boxes
    ParticleEmitter particleEmitter1;   // ParticleEmitter object
    float   emitterX;                   // X screen location
    float   emitterY;                   // Y screen location

public:
    // Constructor
    ParticleDemo();
    // Destructor
    virtual ~ParticleDemo();
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
