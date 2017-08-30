// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacepirates.h v1.0

#ifndef _SPACEPIRATES_H         // Prevent multiple definitions if this 
#define _SPACEPIRATES_H         // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <time.h>
#include <fstream>
#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "dashboard.h"
#include "planet.h"
#include "ship.h"
#include "torpedo.h"
#include "cow.h"

namespace spacePiratesNS
{
    const char FONT[] = "Arial Bold";  // font
    const int FONT_BIG_SIZE = 256;     // font height
    const int FONT_SCORE_SIZE = 48;
    const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
    const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
    const COLOR_ARGB SHIP2_COLOR = graphicsNS::YELLOW;
    const int SCORE_Y = 10;
    const int SCORE1_X = 60;
    const int HEALTHBAR_Y = 30;
    const int SHIP1_HEALTHBAR_X = 40;
    const int COUNT_DOWN_X = GAME_WIDTH/2 - FONT_BIG_SIZE/4;
    const int COUNT_DOWN_Y = GAME_HEIGHT/2 - FONT_BIG_SIZE/2;
    const int COUNT_DOWN = 5;           // count down from 5
    const int BUF_SIZE = 20;
    const int ROUND_TIME = 5;           // time until new round starts
    const int SPACE_SCALE = 2;                  // scale factor of space image
    const int SPACE_WIDTH = 640 * SPACE_SCALE;  // width of scaled space image
    const int SPACE_HEIGHT = 480 * SPACE_SCALE; // height of scaled space image
    const float SHIP_LIMIT_LEFT = GAME_WIDTH/8;
    const float SHIP_LIMIT_RIGHT = GAME_WIDTH - GAME_WIDTH/8 - shipNS::WIDTH;
    const float SHIP_LIMIT_TOP = GAME_HEIGHT/8;
    const float SHIP_LIMIT_BOTTOM = GAME_HEIGHT - GAME_HEIGHT/8 - shipNS::HEIGHT;
    const int MAX_PIRATES = 10;         // max pirates
    const int RADAR_SIZE = 96;
    const int RADAR_TEXTURE_TOP = 160;
    const float RADAR_X = GAME_WIDTH-RADAR_SIZE;    // screen position
    const float RADAR_Y = 10;
    const float RADAR_CENTER_X = RADAR_X + RADAR_SIZE/2;
    const float RADAR_CENTER_Y = RADAR_Y + RADAR_SIZE/2;
    const int BLIP_SIZE = 32;
    const int EARTH_BLIP_FRAME = 51;
    const int COW_BLIP_FRAME = 52;
    const float SECTOR_SIZE = 800;      // size of space sector
    const char SAVE_NAME[] = "SpacePiratesSave.txt";    // saved game
}

// SpacePirates is the class we create, it inherits from the Game class
class SpacePirates : public Game
{
private:
    // game items
    TextureManager menuTexture, spaceTexture, gameTextures;   // textures
    Ship    ship1, ship2[spacePiratesNS::MAX_PIRATES]; // spaceships
    Torpedo torpedo1, torpedo2[spacePiratesNS::MAX_PIRATES]; // torpedoes
    Cow     cow[spacePiratesNS::MAX_PIRATES];   // cows
    Planet  planet;             // the planet
    Image   space;              // backdrop image
    Image   menu;               // menu image
    Image   radar;              // radar image
    Image   earthBlip;          // earth on radar
    //Image   pirateBlip[spacePiratesNS::MAX_PIRATES]; // pirate on radar
    Image   cowBlip[spacePiratesNS::MAX_PIRATES];    // cow on radar
    Bar     healthBar;          // health bar for ships
    TextDX  fontBig;            // DirectX font for game banners
    TextDX  fontScore;
    bool    menuOn;
    bool    countDownOn;        // true when count down is displayed
    bool    loadGameDialog;     // load game dialog is displayed
    float   countDownTimer;
    char buffer[spacePiratesNS::BUF_SIZE];
    bool    roundOver;          // true when round is over
    float   roundTimer;         // time until new round starts
    int     ship1Score; 		// score

public:
    // Constructor
    SpacePirates();
    // Destructor
    virtual ~SpacePirates();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    // Save game
    // Does not save torpedoes or object velocities
    void saveGame();    // Save game state
    // Load game
    void loadGame();    // Load game state
    // Check for saved game file
    // Post: returns true if saved game found, false if not
    bool foundGame();
    void consoleCommand(); // process console command
    void roundStart();  // start a new round of play
    void releaseAll();
    void resetAll();
};

#endif
