// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.h Parallax Scroll Demo v1.0

#ifndef _SPACEWAR_H             // Prevent multiple definitions if this 
#define _SPACEWAR_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "ship.h"

namespace spacewarNS
{
    const char FONT[] = "Arial Bold";  // font
    const int FONT_BIG_SIZE = 256;     // font height
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const int COUNT_DOWN_X = GAME_WIDTH/2 - FONT_BIG_SIZE/4;
	const int COUNT_DOWN_Y = GAME_HEIGHT/2 - FONT_BIG_SIZE/2;
    const int COUNT_DOWN = 2;               // count down
    const int BUF_SIZE = 20;
    const int ROUND_TIME = 5;               // time until new round starts
}

// Spacewar is the class we create, it inherits from the Game class
class Spacewar : public Game
{
private:
    // game items
    TextureManager menuTexture, spaceTexture, textures1;   // textures
    Ship    ship1;              // spaceship
    Image   space;              // backdrop image
    Image   planet;
    Image   moons[4];           // moons
    Image   menu;               // menu image
    bool    menuOn;
    bool    countDownOn;        // true when count down is displayed
    float   countDownTimer;
    TextDX  fontBig;            // DirectX font for game banners
    char buffer[spacewarNS::BUF_SIZE];
    bool    roundOver;          // true when round is over
    float   roundTimer;         // time until new round starts

public:
    // Constructor
    Spacewar();
    // Destructor
    virtual ~Spacewar();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void roundStart();  // start a new round of play
    void releaseAll();
    void resetAll();
};

#endif
