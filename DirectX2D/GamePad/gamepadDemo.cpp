// Programming 2D Games
// Copyright (c) 2013 by: 
// Charles Kelly
// gamepadDemo.cpp v1.0

#include "gamepadDemo.h"

//=============================================================================
// Constructor
//=============================================================================
GamepadDemo::GamepadDemo()
{
    dxFont = new TextDX();  // DirectX font
}

//=============================================================================
// Destructor
//=============================================================================
GamepadDemo::~GamepadDemo()
{
    releaseAll();           // call onLostDevice() for every graphics item
    safeDelete(dxFont);
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void GamepadDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // menu texture
    if (!menuTexture.initialize(graphics,MENU_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
    // controller texture
    if (!controllerTexture.initialize(graphics,CONTROLLER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing controller texture"));

    // menu image
    if (!menu.initialize(graphics,0,0,0,&menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu image"));
    // controller image
    if (!controller.initialize(graphics,0,0,0,&controllerTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing controller image"));

    // initialize DirectX font
    // 18 pixel high Arial
    if(dxFont->initialize(graphics, 12, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

    graphics->setBackColor(graphicsNS::WHITE);  // set background color
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void GamepadDemo::update()
{}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void GamepadDemo::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void GamepadDemo::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void GamepadDemo::render()
{
    graphics->spriteBegin();                // begin drawing sprites
    drawController(0,0,0);
    drawController(1,320,0);
    drawController(2,0,240);
    drawController(3,320,240);
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// Draw controller n with state of inputs at screen x,y
//=============================================================================
void GamepadDemo::drawController(int n, int x, int y)
{
    controller.setX((float)x);
    controller.setY((float)y);
    dxFont->setFontColor(graphicsNS::BLACK);
    if(input->getGamepadConnected(n) == false)  // if controller n not connected
    {
        dxFont->print("No Controller",x+140,y+90);
        return;
    }
    controller.draw();
    // get trigger and thumbstick positions
    leftTrigger = input->getGamepadLeftTrigger(n);
    leftTriggerUndead = input->getGamepadLeftTriggerUndead(n);
    rightTrigger = input->getGamepadRightTrigger(n);
    rightTriggerUndead = input->getGamepadRightTriggerUndead(n);
    leftThumbX = input->getGamepadThumbLX(n);
    leftThumbXUndead = input->getGamepadThumbLXUndead(n);
    leftThumbY = input->getGamepadThumbLY(n);
    leftThumbYUndead = input->getGamepadThumbLYUndead(n);
    rightThumbX = input->getGamepadThumbRX(n);
    rightThumbXUndead = input->getGamepadThumbRXUndead(n);
    rightThumbY = input->getGamepadThumbRY(n);
    rightThumbYUndead = input->getGamepadThumbRYUndead(n);
    input->gamePadVibrateLeft(n,leftTrigger<<8,0.1f);   // vibrate left
    input->gamePadVibrateRight(n,rightTrigger<<8,0.1f); // vibtate right
    // Display triggers and thumbsticks
    message.str("");
    message << leftTrigger;
    dxFont->print(message.str(),x+90,y+15);
    message.str("");
    message << "Undead= " << leftTriggerUndead;
    dxFont->print(message.str(),x+44,y+5);
    message.str("");
    message << rightTrigger;
    dxFont->print(message.str(),x+216,y+15);
    message.str("");
    message << "Undead= " << rightTriggerUndead;
    dxFont->print(message.str(),x+170,y+5);
    message.str("");
    message << "X= " << leftThumbX;
    dxFont->print(message.str(),x+15,y+195);
    message.str("");
    message << "Undead X= " << leftThumbXUndead;
    dxFont->print(message.str(),x+15,y+205);
    message.str("");
    message << "Y= " << leftThumbY;
    dxFont->print(message.str(),x+15,y+215);
    message.str("");
    message << "Undead Y= " << leftThumbYUndead;
    dxFont->print(message.str(),x+15,y+225);
    message.str("");
    message << "X= " << rightThumbX;
    dxFont->print(message.str(),x+180,y+190);
    message.str("");
    message << "Undead X= " << rightThumbXUndead;
    dxFont->print(message.str(),x+180,y+200);
    message.str("");
    message << "Y= " << rightThumbY;
    dxFont->print(message.str(),x+180,y+210);
    message.str("");
    message << "Undead Y= " << rightThumbYUndead;
    dxFont->print(message.str(),x+180,y+220);
    // Display button states
    if(input->getGamepadStart(n))
        dxFont->print("Start",x+180,y+35);
    if(input->getGamepadRightShoulder(n))
        dxFont->print("Shoulder",x+250,y+35);
    if(input->getGamepadA(n))
        dxFont->print("A",x+284,y+75);
    if(input->getGamepadB(n))
        dxFont->print("B",x+293,y+75);
    if(input->getGamepadX(n))
        dxFont->print("X",x+302,y+75);
    if(input->getGamepadY(n))
        dxFont->print("Y",x+311,y+75);
    if(input->getGamepadDPadUp(n))
        dxFont->print("Up",x+90,y+195);
    if(input->getGamepadDPadRight(n))
        dxFont->print("Right",x+104,y+195);
    if(input->getGamepadDPadDown(n))
        dxFont->print("Down",x+130,y+195);
    if(input->getGamepadDPadLeft(n))
        dxFont->print("Left",x+160,y+195);
    if(input->getGamepadLeftShoulder(n))
        dxFont->print("Shoulder",x+20,y+35);
    if(input->getGamepadBack(n))
        dxFont->print("Back",x+120,y+35);
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void GamepadDemo::releaseAll()
{
    dxFont->onLostDevice();
    controllerTexture.onLostDevice();
    menuTexture.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void GamepadDemo::resetAll()
{
    menuTexture.onResetDevice();
    controllerTexture.onResetDevice();
    dxFont->onResetDevice();
    Game::resetAll();
    return;
}
