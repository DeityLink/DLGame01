
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

#include "LTexture.h"
#include "Camera.h"

#include "Platform.h"

using namespace std;

extern SDL_Renderer* gRenderer;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern Camera* camera;

Platform::Platform()
{

}

Platform::Platform(int X, int Y, int W, int H)
{
	x = X;
	y = Y;
	w = W;
	h = H;

	fillRect = new SDL_Rect{ x, y, w, h };
}

Platform::~Platform()
{
	//Deallocate
}

SDL_Rect* Platform::getRekt()
{
	return fillRect;
}

void Platform::render()
{
	SDL_Rect* newRect = new SDL_Rect{ x - camera->getCameraX() + (SCREEN_WIDTH / 2), y - camera->getCameraY() + (SCREEN_HEIGHT / 2), w, h };
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, newRect);
}