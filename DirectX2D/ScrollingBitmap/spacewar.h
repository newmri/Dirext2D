// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacewar.h Scrolling Bitmap Demo

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
    const int FONT_SCORE_SIZE = 48;
    const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
    const int SCORE_Y = 10;
    const int SCORE1_X = 60;
    const int HEALTHBAR_Y = 40;
    const int SHIP1_HEALTHBAR_X = 40;
    const int HEALTHBAR_START_FRAME = 22;
    const int HEALTHBAR_END_FRAME = 22;
    const int HEALTHBAR_HEIGHT = 32;
    const int HEALTHBAR_WIDTH = 64;
    const int COUNT_DOWN_X = GAME_WIDTH/2 - FONT_BIG_SIZE/4;
    const int COUNT_DOWN_Y = GAME_HEIGHT/2 - FONT_BIG_SIZE/2;
    const int COUNT_DOWN = 2;           // count down
    const int BUF_SIZE = 20;
    const int ROUND_TIME = 5;           // time until new round starts
    const int HEALTHBAR_TEXTURE_COLS = 4;   // health bar is a 4 column texture
}

// Spacewar is the class we create, it inherits from the Game class
class Spacewar : public Game
{
private:
    // game items
    TextureManager menuTexture, spaceTexture, textures1;   // textures
    Ship    ship1;              // spaceships
    Image   space;              // backdrop image
    Image   menu;               // menu image
    bool    menuOn;
    bool    countDownOn;        // true when count down is displayed
    float   countDownTimer;
    TextDX  fontBig;            // DirectX font for game banners
    TextDX  fontScore;
    char buffer[spacewarNS::BUF_SIZE];
    bool    ship1Scored;        // true if ship scored during round
    bool    roundOver;          // true when round is over
    float   roundTimer;         // time until new round starts
    int     ship1Score; // scores

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
