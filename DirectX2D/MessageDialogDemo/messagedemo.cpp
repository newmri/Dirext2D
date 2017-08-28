// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 9 MessageDemo.cpp v1.0

#include "messagedemo.h"

//=============================================================================
// Constructor
//=============================================================================
MessageDemo::MessageDemo()
{
    messageDialog = NULL;
}

//=============================================================================
// Destructor
//=============================================================================
MessageDemo::~MessageDemo()
{
    releaseAll();				// call deviceLost() for every graphics item
    SAFE_DELETE(messageDialog);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void MessageDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    reset();            // reset all game variables
    fpsOn = true;       // display frames per second

    messageDialog->setBorderColor(graphicsNS::LTGRAY);
    messageDialog->setBackColor(SETCOLOR_ARGB(255,50,50,90));
    messageDialog->print("This is a test message. This is only a test. If this "
                    "were an actual message it would contain some useful "
                    "information instead of this meaningless dribble.");
}


//=============================================================================
// Reset the game to begin play and after a score
//=============================================================================
void MessageDemo::reset()
{
    return;
}

//=============================================================================
// move all game items
// frameTime is used to regulate the speed of movement
//=============================================================================
void MessageDemo::update()
{
    messageDialog->update();    // check for button events
    if(messageDialog->getButtonClicked() == 1)
    {
        messageDialog->setButtonType(1);
        messageDialog->print("Button 1 clicked");
    }
    else if(messageDialog->getButtonClicked() == 2)
    {
        messageDialog->setButtonType(0);
        messageDialog->print("Button 2 clicked");
    }
}

//=============================================================================
// render game items and do end of round logic
//=============================================================================
void MessageDemo::render()
{
    messageDialog->draw();      // display dialog box
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void MessageDemo::releaseAll()
{
    messageDialog->onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces and reset all entities.
//=============================================================================
void MessageDemo::resetAll()
{
    messageDialog->onResetDevice();
    Game::resetAll();
    return;
}
