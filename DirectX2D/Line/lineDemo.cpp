// Programming 2D Games
// Copyright (c) 2011, 2014 by: 
// Charles Kelly
// lineDemo.cpp v1.0

#include "lineDemo.h"

//=============================================================================
// Constructor
//=============================================================================
LineDemo::LineDemo()
{}

//=============================================================================
// Destructor
//=============================================================================
LineDemo::~LineDemo()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void LineDemo::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError
    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void LineDemo::update()
{}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void LineDemo::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void LineDemo::collisions()
{}

//=============================================================================
// Render game items
//=============================================================================
void LineDemo::render()
{
    graphics->createLine(&line);            // create the line

    VECTOR2 back[] = { VECTOR2(250.0f, 50.0f), VECTOR2(450.0f, 50.0f), 
        VECTOR2(450.0f, 250.0f), VECTOR2(250.0f, 250.0f), VECTOR2(250.0f, 50.0f) };
    VECTOR2 front[] = { VECTOR2(150.0f, 150.0f), VECTOR2(350.0f, 150.0f),
        VECTOR2(350.0f, 350.0f), VECTOR2(150.0f, 350.0f), VECTOR2(150.0f, 150.0f) };
    VECTOR2 edge0[] = { VECTOR2(150.0f, 150.0f), VECTOR2(250.0f, 50.0f) };
    VECTOR2 edge1[] = { VECTOR2(350.0f, 150.0f), VECTOR2(450.0f, 50.0f) };
    VECTOR2 edge2[] = { VECTOR2(350.0f, 350.0f), VECTOR2(450.0f, 250.0f) };
    VECTOR2 edge3[] = { VECTOR2(150.0f, 350.0f), VECTOR2(250.0f, 250.0f) };

    line->SetWidth(5.0f);                   // SetWidth must be called before Begin
    line->Begin();                          // begin drawing lines
    line->Draw(back, 5, graphicsNS::GRAY);  // draw back, 5 vertices, GRAY
    line->Draw(front, 5, graphicsNS::WHITE);    // draw front, 5 vertices, WHITE
    line->End();                            // end drawing lines

    line->SetWidth(1.0f);                   // SetWidth must be called before Begin
    line->Begin();                          // begin drawing lines
    line->Draw(edge0, 2, graphicsNS::RED);  // draw edge, 2 vertices, RED
    line->End();                            // end drawing lines

    line->SetWidth(2.0f);                   // SetWidth must be called before Begin
    line->Begin();                          // begin drawing lines
    line->Draw(edge1, 2, graphicsNS::YELLOW); // draw edge, 2 vertices, YELLOW
    line->End();                            // end drawing lines

    line->SetWidth(3.0f);                   // SetWidth must be called before Begin
    line->Begin();                          // begin drawing lines
    line->Draw(edge2, 2, graphicsNS::GREEN); // draw edge, 2 vertices, GREEN
    line->End();                            // end drawing lines

    line->SetWidth(4.0f);                   // SetWidth must be called before Begin
    line->Begin();                          // begin drawing lines
    line->Draw(edge3, 2, graphicsNS::BLUE); // draw edge, 2 vertices, BLUE
    line->End();                            // end drawing lines

    safeRelease(line);                      // release the line
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void LineDemo::releaseAll()
{
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void LineDemo::resetAll()
{
    Game::resetAll();
    return;
}
