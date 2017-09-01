// Programming 2D Games
// Copyright (c) 2012 by: 
// Charles Kelly
// asteroidRace.h v0.8

#ifndef _CREATETHIS_H           // Prevent multiple definitions if this 
#define _CREATETHIS_H           // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class AsteroidRace;

#include <string>
#include <deque>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "textDX.h"
#include "ship.h"
#include "asteroid.h"

namespace asteroidRaceNS
{
    const char FONT[] = "Arial Bold";  // font
    const int FONT_BIG_SIZE = 128;     // font height
    const int FONT_SCORE_SIZE = 96;
    const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const COLOR_ARGB SHIP_COLOR = graphicsNS::BLUE;
    const int SCORE_Y = 10;
    const int SCORE_X = 60;
    const int HEALTHBAR_Y = 40;
    const int SHIP_HEALTHBAR_X = 40;
    const int COUNT_DOWN_X = GAME_WIDTH/2 - FONT_BIG_SIZE/4;
    const int COUNT_DOWN_Y = GAME_HEIGHT/2 - FONT_BIG_SIZE/2;
    const int COUNT_DOWN = 5;           // count down from 5
    const int BUF_SIZE = 20;
    const int ROUND_TIME = 5;           // time until new round starts
    const int NUMA = 100;               // number of asteroids
    const int TURN_SPEED = 200;         // pixels/sec turn speed of ship
}

//=============================================================================
// This class is the core of the game
//=============================================================================
class AsteroidRace : public Game
{
private:
    // game items
    TextureManager menuTexture, nebulaTexture, gameTextures, asteroidTextures; // textures
    Image   menu;               // menu image
    Image   nebula;             // backdrop image
    Ship    ship;                   // the ship
    //Asteroid asteroids[asteroidRacerNS::NUMA];   // the asteroids
    std::deque<Asteroid *> asteroids;   // the asteroids
    TextDX  fontBig;            // DirectX font for game banners
    TextDX  fontScore;
    std::string  message;
    float   messageY;
    bool    menuOn;
    bool    countDownOn;        // true when count down is displayed
    float   countDownTimer;
    char    buffer[asteroidRaceNS::BUF_SIZE];
    bool    roundOver;          // true when round is over
    float   roundTimer;         // time until new round starts
    //bool    shipScored;         // true if ship scored during round
    int     shipScore;          // score

public:
    // Constructor
    AsteroidRace();
    // Destructor
    virtual ~AsteroidRace();
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
