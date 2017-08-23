#include "game.h"

Game::Game()
{
	input = new Input();
	paused = false;
	graphics = NULL;
	initialized = false;
}

Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized) {
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0); return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			input->keyDown(wParam); return 0;
		case WM_KEYUP: case WM_SYSKEYUP:
			input->keyUp(wParam); return 0;
		case WM_CHAR:
			input->keyIn(wParam); return 0;
		case WM_MOUSEMOVE:
			input->mouseIn(lParam); return 0;
		case WM_INPUT: // Got mouse data.
			input->mouseRawIn(lParam); return 0;
		case WM_LBUTTONDOWN:
			input->setMouseLButton(true);
			input->mouseIn(lParam); return 0;
		case WM_LBUTTONUP:
			input->setMouseLButton(false);
			input->mouseIn(lParam); return 0;
		case WM_MBUTTONDOWN:
			input->setMouseMButton(true);
			input->mouseIn(lParam); return 0;
		case WM_MBUTTONUP:
			input->setMouseMButton(false);
			input->mouseIn(lParam);
		case WM_RBUTTONDOWN:
			input->setMouseRButton(true);
			input->mouseIn(lParam); return 0;
		case WM_RBUTTONUP:
			input->setMouseRButton(false);
			input->mouseIn(lParam); return 0;
		case WM_XBUTTONDOWN: case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam); return 0;
		case WM_DEVICECHANGE:
			input->checkControllers(); return 0;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam); // To Windows
}

void Game::initialize(HWND hw)
{
	hwnd = hw;
	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	input->initialize(hwnd, false); // Don't capture mouse.

	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR,
			"Error initializing high resolution timer"));
	QueryPerformanceCounter(&timeStart);
	initialized = true;
}

void Game::handleLostGraphicsDevice()
{
	hr = graphics->getDeviceState();
	if (FAILED(hr)) { // If device is not valid,
		if (hr == D3DERR_DEVICELOST) { // If device is lost and can't reset,
			Sleep(100); return;
		}
		else if (hr == D3DERR_DEVICENOTRESET) { // If device is lost, but can reset,
			releaseAll();
			hr = graphics->reset();
			if (FAILED(hr)) return;
			resetAll();
		}
		else return;
	}
}

void Game::renderGame()
{
	// Start render
	if (SUCCEEDED(graphics->beginScene())) {
		render();
		graphics->endScene();
	}

	handleLostGraphicsDevice();
	graphics->showBackbuffer();
}

void Game::run(HWND hwnd)
{
	if (graphics == NULL) return;
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;
	// Save code
	if (frameTime < MIN_FRAME_TIME) {
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1); // Request 1ms resolution for windows timer
		Sleep(sleepTime);
		timeEndPeriod(1);
		return;
	}
	if (frameTime > 0.0) fps = (fps * 0.99f) + (0.1f / frameTime); // Average FPS
	if (frameTime > MAX_FRAME_TIME) frameTime = MAX_FRAME_TIME;
	timeStart = timeEnd;
	input->readControllers();

	if (!paused) {
		update();
		ai();
		collisions();
		input->vibrateControllers(frameTime);
	}
	renderGame();
	input->clear(inputNS::KEYS_PRESSED);
}
