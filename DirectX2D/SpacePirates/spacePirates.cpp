// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// spacePirates.cpp v1.0
// SpacePirates is the class we create.

#include "spacePirates.h"
using namespace spacePiratesNS;

//=============================================================================
// Constructor
//=============================================================================
SpacePirates::SpacePirates()
{
    menuOn = true;
    countDownOn = false;
    roundOver = false;
    ship1Score = 0;
    initialized = false;
    srand((UINT)time(NULL));
    loadGameDialog = false;
}

//=============================================================================
// Destructor
//=============================================================================
SpacePirates::~SpacePirates()
{
    saveGame();             // save game state
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void SpacePirates::initialize(HWND hwnd)
{
    int randX, randY;

    Game::initialize(hwnd); // throws GameError

    // initialize DirectX fonts
    fontBig.initialize(graphics, FONT_BIG_SIZE, false, false, FONT);
    fontBig.setFontColor(::FONT_COLOR);
    fontScore.initialize(graphics, FONT_SCORE_SIZE, false, false, ::FONT);

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));

    // space texture
    if (!spaceTexture.initialize(graphics,SPACE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));

    // main game textures
    if (!gameTextures.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

    // space image
    if (!space.initialize(graphics,0,0,0,&spaceTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
    space.setScale((float)::SPACE_SCALE);

    // planet
    if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

    // radar
    if (!radar.initialize(graphics, RADAR_SIZE, RADAR_SIZE, 0, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
    RECT rect;
    // The radar texture does not align with the other textures so we need to
    // configure rect to draw radar manually.
    rect.left = 0;
    rect.right = RADAR_SIZE;
    rect.top = RADAR_TEXTURE_TOP;
    rect.bottom = RADAR_TEXTURE_TOP + RADAR_SIZE;       
    radar.setSpriteDataRect(rect);
    radar.setX(RADAR_X);            // screen location
    radar.setY(RADAR_Y);

    // earth blip
    if (!earthBlip.initialize(graphics, BLIP_SIZE, BLIP_SIZE, 8, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Earth blip"));
    earthBlip.setFrames(EARTH_BLIP_FRAME, EARTH_BLIP_FRAME);
    earthBlip.setCurrentFrame(EARTH_BLIP_FRAME);

    for(int i=0; i<MAX_PIRATES; i++)
    {
        // cow blip
        if (!cowBlip[i].initialize(graphics, BLIP_SIZE, BLIP_SIZE, 8, &gameTextures))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cow blip"));
        cowBlip[i].setFrames(COW_BLIP_FRAME, COW_BLIP_FRAME);
        cowBlip[i].setCurrentFrame(COW_BLIP_FRAME);
    }

    // ship1
    if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));

    ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
    ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
    ship1.setColorFilter(SETCOLOR_ARGB(255,230,230,255));   // light blue, used for shield and torpedo
    ship1.setMass(shipNS::MASS);

    // torpedo1
    if (!torpedo1.initialize(this, torpedoNS::WIDTH, torpedoNS::HEIGHT, torpedoNS::TEXTURE_COLS, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing torpedo1"));

    torpedo1.setColorFilter(SETCOLOR_ARGB(255,128,128,255));   // light blue

    for(int i=0; i<MAX_PIRATES; i++)
    {
        // ship2
        if (!ship2[i].initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
        ship2[i].setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
        ship2[i].setCurrentFrame(shipNS::SHIP2_START_FRAME);
        ship2[i].setColorFilter(SETCOLOR_ARGB(255,255,255,64));    // light yellow, used for shield
        ship2[i].setMass(shipNS::MASS);

        // torpedo2
        if (!torpedo2[i].initialize(this, torpedoNS::WIDTH, torpedoNS::HEIGHT, torpedoNS::TEXTURE_COLS, &gameTextures))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing torpedo2"));
        torpedo2[i].setColorFilter(SETCOLOR_ARGB(255,255,255,64));     // light yellow

        // cow
        if (!cow[i].initialize(this, cowNS::WIDTH, cowNS::HEIGHT, cowNS::TEXTURE_COLS, &gameTextures))
            throw(GameError(gameErrorNS::FATAL_ERROR, "Holy Cow Batman, Error initializing cow"));

        do 
        randX = rand()%10-5;
        while(randX == 0);
        do 
        randY = rand()%10-5;
        while(randY == 0);
        ship2[i].setX((float)GAME_WIDTH * randX);
        ship2[i].setY((float)GAME_HEIGHT * randY);
        cow[i].setX(ship2[i].getX() + ship2[i].getWidth());
        cow[i].setY(ship2[i].getY() + ship2[i].getHeight());
    }

    // health bar
    healthBar.initialize(graphics, &gameTextures, 0, ::HEALTHBAR_Y, 2.0f, graphicsNS::WHITE);

    // Start ship in stable clockwise orbit
    ship1.setX(GAME_WIDTH/4 - shipNS::WIDTH);
    ship1.setY(GAME_HEIGHT/2 - shipNS::HEIGHT);
    ship1.setVelocity(VECTOR2(0,-shipNS::SPEED));

    // load game?
    if(foundGame())             // if saved game found
    {
        messageDialog->setButtonType(messageDialogNS::YES_NO);
        messageDialog->print("Load last saved game?");
        loadGameDialog = true;
    }
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void SpacePirates::update()
{
    if(loadGameDialog)
    {
        if(messageDialog->getButtonClicked() == 1)  // if Yes
            loadGame();                             // load saved game
        if(messageDialog->getVisible() == false)
            loadGameDialog = false;
    }
    else if (menuOn)
    {
        if (input->anyKeyPressed())
        {
            menuOn = false;
            input->clearAll();
            roundStart();
        }
    } 
    else if(countDownOn)
    {
        countDownTimer -= frameTime;
        if(countDownTimer <= 0)
            countDownOn = false;
    } 
    else 
    {
        if (ship1.getActive())
        {
            if (input->isKeyDown(SHIP1_FORWARD_KEY) || input->getGamepadDPadUp(0))   // if engine on
            {
                ship1.setEngineOn(true);
                audio->playCue(ENGINE1);
            }
            else
            {
                ship1.setEngineOn(false);
                audio->stopCue(ENGINE1);
            }
            ship1.rotate(shipNS::NONE);
            if (input->isKeyDown(SHIP1_LEFT_KEY) || input->getGamepadDPadLeft(0))   // if turn ship1 left
                ship1.rotate(shipNS::LEFT);
            if (input->isKeyDown(SHIP1_RIGHT_KEY) || input->getGamepadDPadRight(0)) // if turn ship1 right
                ship1.rotate(shipNS::RIGHT);
            if (input->isKeyDown(SHIP1_FIRE_KEY) || input->getGamepadA(0))          // if ship1 fire
                torpedo1.fire(&ship1);                  // fire torpedo1
        }
        if(roundOver)
        {
            roundTimer -= frameTime;
            if(roundTimer <= 0)
                roundStart();
        }
    }
    ship1.gravityForce(&planet, frameTime);
    torpedo1.gravityForce(&planet, frameTime);

    // Update the entities
    planet.update(frameTime);
    ship1.update(frameTime);
    torpedo1.update(frameTime);
    for(int i=0; i<MAX_PIRATES; i++)
    {
        ship2[i].update(frameTime);
        torpedo2[i].update(frameTime);
        cow[i].gravityForce(&planet, frameTime);
        cow[i].update(frameTime);
    }

    // if ship at screen edge
    if( (ship1.getX() < SHIP_LIMIT_LEFT) || (ship1.getX() > SHIP_LIMIT_RIGHT) )
    {
        // move space
        space.setX(space.getX() - frameTime * ship1.getVelocity().x);
        // move planet
        planet.setX(planet.getX() - frameTime * ship1.getVelocity().x);
        // move torpedoes
        torpedo1.setX(torpedo1.getX() - frameTime * ship1.getVelocity().x);
        // move pirate ships and cows
        for(int i=0; i<MAX_PIRATES; i++)
        {
            ship2[i].setX(ship2[i].getX() - frameTime * ship1.getVelocity().x);
            cow[i].setX(cow[i].getX() - frameTime * ship1.getVelocity().x);
        }
        // keep ship on screen
        if(ship1.getX() < SHIP_LIMIT_LEFT)
            ship1.setX(SHIP_LIMIT_LEFT);
        if(ship1.getX() > SHIP_LIMIT_RIGHT)
            ship1.setX(SHIP_LIMIT_RIGHT);
    }
    if( (ship1.getY() < SHIP_LIMIT_TOP)  || (ship1.getY() > SHIP_LIMIT_BOTTOM) )
    {
        // move space
        space.setY(space.getY() - frameTime * ship1.getVelocity().y);
        // move planet
        planet.setY(planet.getY() - frameTime * ship1.getVelocity().y);
        // move torpedoes
        torpedo1.setY(torpedo1.getY() - frameTime * ship1.getVelocity().y);
        // move pirate ships and cows
        for(int i=0; i<MAX_PIRATES; i++)
        {
            ship2[i].setY(ship2[i].getY() - frameTime * ship1.getVelocity().y);
            cow[i].setY(cow[i].getY() - frameTime * ship1.getVelocity().y);
        }
        // keep ship on screen
        if(ship1.getY() < SHIP_LIMIT_TOP)
            ship1.setY(SHIP_LIMIT_TOP);
        if(ship1.getY() > SHIP_LIMIT_BOTTOM)
            ship1.setY(SHIP_LIMIT_BOTTOM);
    }

    // Wrap space image around at edge
    // if left edge of space > screen left edge
    if (space.getX() > 0)               
        // move space image left by SPACE_WIDTH
        space.setX(space.getX() - ::SPACE_WIDTH);
    // if space image off screen left
    if (space.getX() < -::SPACE_WIDTH)
        // move space image right by SPACE_WIDTH
        space.setX(space.getX() + ::SPACE_WIDTH);
    // if top edge of space > screen top edge
    if (space.getY() > 0)
        // move space image up by SPACE_HEIGHT
        space.setY(space.getY() - ::SPACE_HEIGHT);
    // if space image off screen top
    if (space.getY() < -::SPACE_HEIGHT)
        // move space image down by SPACE_IMAGE
        space.setY(space.getY() + ::SPACE_HEIGHT);

    // update radar image
    float rx,ry;
    rx =  planet.getX() - ship1.getX();     // planet radar X
    if(rx > 3*SECTOR_SIZE)                  // if outside radar limit
        rx = 3*SECTOR_SIZE;
    else if(rx < -3*SECTOR_SIZE)
        rx = -3*SECTOR_SIZE;
    ry = planet.getY() - ship1.getY();      // planet radar Y
    if(ry > 3*SECTOR_SIZE)                  // if outside radar limit
        ry = 3*SECTOR_SIZE;
    else if(ry < -3*SECTOR_SIZE)
        ry = -3*SECTOR_SIZE;
    rx = rx/SECTOR_SIZE * RADAR_SIZE/6;     // scale to fit radar
    ry = ry/SECTOR_SIZE * RADAR_SIZE/6;
    earthBlip.setX(rx + RADAR_CENTER_X - BLIP_SIZE/2);
    earthBlip.setY(ry + RADAR_CENTER_Y - BLIP_SIZE/2);

    for(int i=0; i<MAX_PIRATES; i++)
    {
        // cow radar blips
        rx = cow[i].getX() - ship1.getX();  // cow radar X
        if(rx > 3*SECTOR_SIZE)              // if outside radar limit
            rx = 3*SECTOR_SIZE;
        else if(rx < -3*SECTOR_SIZE)
            rx = -3*SECTOR_SIZE;
        ry = cow[i].getY() - ship1.getY();  // cow raday Y
        if(ry > 3*SECTOR_SIZE)              // if outside radar limit
            ry = 3*SECTOR_SIZE;
        else if(ry < -3*SECTOR_SIZE)
            ry = -3*SECTOR_SIZE;
        rx = rx/SECTOR_SIZE * RADAR_SIZE/6; // scale to fit radar
        ry = ry/SECTOR_SIZE * RADAR_SIZE/6;
        cowBlip[i].setX(rx + RADAR_CENTER_X - BLIP_SIZE/2);
        cowBlip[i].setY(ry + RADAR_CENTER_Y - BLIP_SIZE/2);
    }
}

//=============================================================================
// Start a new round of play
//=============================================================================
void SpacePirates::roundStart()
{
    ship1.repair();
    roundOver = false;
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void SpacePirates::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void SpacePirates::collisions()
{
    VECTOR2 collisionVector;
    // if collision between ship1 and planet
    if(ship1.collidesWith(planet, collisionVector))
    {
        ship1.toOldPosition();      // move ship out of collision
        ship1.damage(PLANET);
        input->gamePadVibrateLeft(0,65535,1.0);  // vibrate controller 0, 100%, 1.0 sec
    }
    if(torpedo1.collidesWith(planet, collisionVector))
    {
        torpedo1.setVisible(false);
        torpedo1.setActive(false);
        audio->playCue(TORPEDO_CRASH);
    }
    for(int i=0; i<MAX_PIRATES; i++)
    {
        // if collision between ship2 and planet
        if(ship2[i].collidesWith(planet, collisionVector))
        {
            ship2[i].toOldPosition();  // move ship out of collision
            ship2[i].damage(PLANET);
        }
        // if collision between ship1 and ship2
        if(ship1.collidesWith(ship2[i], collisionVector))
        {
            // bounce off other ship
            ship1.bounce(collisionVector, ship2[i]);
            ship2[i].bounce(collisionVector*-1, ship1);
            ship1.damage(SHIP);
            ship2[i].damage(SHIP);
            input->gamePadVibrateRight(0,30000,0.5);
        }
        // if collision between torpedos and ships
        if(torpedo1.collidesWith(ship2[i], collisionVector))
        {
            ship2[i].damage(TORPEDO);
            torpedo1.setVisible(false);
            torpedo1.setActive(false);
        }
        if(torpedo2[i].collidesWith(ship1, collisionVector))
        {
            ship1.damage(TORPEDO);
            torpedo2[i].setVisible(false);
            torpedo2[i].setActive(false);
            input->gamePadVibrateRight(0,20000,0.5);
        }
        // if collision between torpedos and planet
        if(torpedo2[i].collidesWith(planet, collisionVector))
        {
            torpedo2[i].setVisible(false);
            torpedo2[i].setActive(false);
            audio->playCue(TORPEDO_CRASH);
        }
        // if collision between cow and planet
        if(cow[i].collidesWith(planet, collisionVector))
        {
            cow[i].damage(PLANET);
            ship1Score++;
        }
        // if collision between ship1 and cow
        if(ship1.collidesWith(cow[i], collisionVector))
        {
            // bounce off cow
            ship1.bounce(collisionVector, cow[i]);
            cow[i].bounce(collisionVector*-1, ship1);
            ship1.damage(COW);
            cow[i].damage(SHIP);
            input->gamePadVibrateRight(0,30000,0.5);
        }
    }
}

//=============================================================================
// Render game items
//=============================================================================
void SpacePirates::render()
{
    float x = space.getX();
    float y = space.getY();
    graphics->spriteBegin();                // begin drawing sprites
    // Wrap space image around at edges
    space.draw();                           // draw at current location
    // if space image right edge visible
    if (space.getX() < -::SPACE_WIDTH + (int)GAME_WIDTH)
    {
        space.setX(space.getX() + ::SPACE_WIDTH); // wrap around to left edge
        space.draw();                           // draw again
    }
    // if space image bottom edge visible
    if (space.getY() < -::SPACE_HEIGHT + (int)GAME_HEIGHT)
    {
        space.setY(space.getY() + ::SPACE_HEIGHT); // wrap around to top edge
        space.draw();                           // draw again
        space.setX(x);                          // restore x position
        // if space image right edge visible
        // wrap around to left edge
        if (space.getX() < -::SPACE_WIDTH + (int)GAME_WIDTH)
            space.draw();                       // draw again
    }
    space.setY(y);      // restore y position

    planet.draw();                          // draw the planet

    // display scores
    fontScore.setFontColor(::SHIP1_COLOR);
    _snprintf_s(buffer, ::BUF_SIZE, "%d", (int)ship1Score);
    fontScore.print(buffer,::SCORE1_X,::SCORE_Y);

    // display health bars
    healthBar.setX((float)::SHIP1_HEALTHBAR_X);
    healthBar.set(ship1.getHealth());
    healthBar.draw(::SHIP1_COLOR);

    ship1.draw();                           // draw the spaceships
    torpedo1.draw(graphicsNS::FILTER);      // draw the torpedoes using colorFilter
    for(int i=0; i<MAX_PIRATES; i++)
    {
        ship2[i].draw();
        torpedo2[i].draw(graphicsNS::FILTER);
        cow[i].draw();
    }

    // draw the radar
    radar.draw();
    earthBlip.draw();
    for(int i=0; i<MAX_PIRATES; i++)
    {
        if(cow[i].getVisible())
            cowBlip[i].draw();
    }

    if(menuOn)
        menu.draw();
    if(countDownOn)
    {
        _snprintf_s(buffer, ::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
        fontBig.print(buffer,::COUNT_DOWN_X,::COUNT_DOWN_Y);
    }

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Save game
// Does not save torpedoes. Saves only ship1 velocity
//=============================================================================
void SpacePirates::saveGame()
{
    std::ofstream outFile(SAVE_NAME);
    if(outFile.fail())                  // error creating save file
    {
        messageDialog->print("Error creating save file.");
        return;
    }
    outFile << ship1Score << '\n'
            << ship1.getX() << '\n'
            << ship1.getY() << '\n'
            << ship1.getHealth() << '\n'
            << ship1.getVelocity().x << '\n'
            << ship1.getVelocity().y << '\n';
    for(int i=0; i<MAX_PIRATES; i++)
    {
        outFile << ship2[i].getX() << '\n'
                << ship2[i].getY() << '\n'
                << ship2[i].getHealth() << '\n'
                << cow[i].getX() << '\n'
                << cow[i].getY() << '\n';
    }
    outFile.close();
}

//=============================================================================
// Load game
//=============================================================================
void SpacePirates::loadGame()
{
    std::ifstream inFile(SAVE_NAME);
    float n, x, y;

    if(inFile.fail())                  // error opening save file
    {
        messageDialog->print("Error opening save file.");
        return;
    }
    inFile >> ship1Score;
    inFile >> n;
    ship1.setX(n);
    inFile >> n;
    ship1.setY(n);
    inFile >> n;
    ship1.setHealth(n);
    inFile >> x;
    inFile >> y;
    ship1.setVelocity(D3DXVECTOR2(x,y));

    for(int i=0; i<MAX_PIRATES; i++)
    {
        inFile >> n;
        ship2[i].setX(n);
        inFile >> n;
        ship2[i].setY(n);
        inFile >> n;
        ship2[i].setHealth(n);
        inFile >> n;
        cow[i].setX(n);
        inFile >> n;
        cow[i].setY(n);
    }
    inFile.close();
}

//=============================================================================
// Check for saved game file
// Post: returns true if saved game found, false if not
//=============================================================================
bool SpacePirates::foundGame()
{
    std::ifstream inFile(SAVE_NAME);

    if(inFile.fail())                   // error opening save file
        return false;                   // no saved game found
    inFile.close();
    return true;                        // saved game found
}

//=============================================================================
// process console commands
//=============================================================================
void SpacePirates::consoleCommand()
{
    command = console->getCommand();    // get command from console
    if(command == "")                   // if no command
        return;

    if (command == "help")              // if "help" command
    {
        console->print("Console Commands:");
        console->print("fps - toggle display of frames per second");
        console->print("gravity off - turns off planet gravity");
        console->print("gravity on - turns on planet gravity");
        console->print("planet off - disables planet");
        console->print("planet on - enables planet");
        return;
    }
    if (command == "fps")
    {
        fpsOn = !fpsOn;                 // toggle display of fps
        if(fpsOn)
            console->print("fps On");
        else
            console->print("fps Off");
    }

    if (command == "gravity off")
    {
        planet.setMass(0);
        console->print("Gravity Off");
    }else if (command == "gravity on")
    {
        planet.setMass(planetNS::MASS);
        console->print("Gravity On");
    }else if (command == "planet off")
    {
        planet.disable();
        console->print("Planet Off");
    }else if (command == "planet on")
    {
        planet.enable();
        console->print("Planet On");
    }
}


//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void SpacePirates::releaseAll()
{
    menuTexture.onLostDevice();
    spaceTexture.onLostDevice();
    gameTextures.onLostDevice();
    fontScore.onLostDevice();
    fontBig.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void SpacePirates::resetAll()
{
    fontBig.onResetDevice();
    fontScore.onResetDevice();
    gameTextures.onResetDevice();
    spaceTexture.onResetDevice();
    menuTexture.onResetDevice();

    Game::resetAll();
    return;
}
