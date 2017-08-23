#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

class Game;

#include <Windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"

/*
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
return (game->messageHandler(hwnd, msg, wParam, lParam));
}
*/

class Game
{
protected:
	// Common Attributes
	Graphics* graphics;
	Input* input;
	HWND hwnd;
	HRESULT hr; // Standard Return Type
	LARGE_INTEGER timeStart; // Performance Counter's start value
	LARGE_INTEGER timeEnd; // Performance Counter's end value
	LARGE_INTEGER timerFreq; // // Performance Counter's Frequency
	float frameTime; // Time For Last Frame
	float fps;
	DWORD sleepTime;
	bool paused;
	bool initialized;
public:
	Game();
	virtual ~Game();
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void initialize(HWND hwnd);
	virtual void run(HWND);
	virtual void releaseAll() {};
	virtual void resetAll() {};
	virtual void deleteAll() {};
	virtual void renderGame();
	virtual void handleLostGraphicsDevice();
	Graphics* getGraphics() { return graphics; }
	Input* getInput() { return input; }
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }
	virtual void update() = 0;
	virtual void ai() = 0;
	virtual void collisions() = 0;
	virtual void render() = 0;

};
#endif