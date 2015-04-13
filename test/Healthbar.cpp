#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

#include "Living.h"
#include "HUD.h"
#include "LTexture.h"
#include "Player.h"
#include "Camera.h"

#include "Healthbar.h"

using namespace std;

extern SDL_Renderer* gRenderer;
extern LTexture HUDbars;
extern SDL_Rect HUDbarClips[2];

extern vector<Healthbar*> healthbars;

extern Player* player;
extern Camera* camera;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Healthbar::Healthbar()
{
	healthbars.push_back(this);

}

Healthbar::Healthbar(Living* x)
{
	healthbars.push_back(this);

	x->healthbar = this;
	holder = x;
}

Healthbar::~Healthbar()
{
	//Deallocate
}

Living* Healthbar::getHolder()
{
	return holder;
}

void Healthbar::update()
{
	if (camera->getFocus() == holder)
	{
		HUDbar = true;
	}
	posX = holder->getPosX();
	posY = holder->getPosY();
	radius = holder->getRadius();
	health = holder->getHealth();
	maxHealth = holder->getMaxHealth();

}

void Healthbar::render()
{
	if (HUDbar)
	{
		SDL_Rect renderQuad1 = { 0, 24, 179, 24 };
		HUDbars.render(67, 484, &renderQuad1, NULL, NULL, SDL_FLIP_NONE, true);
		SDL_Rect renderQuad2 = { 0, 0, (health * 179) / maxHealth, 24 };
		HUDbars.render(67, 484, &renderQuad2, NULL, NULL, SDL_FLIP_NONE, true);
	}
	else
	{
		SDL_Rect* newRect1 = new SDL_Rect{ posX - player->getPosX() + (SCREEN_WIDTH / 2) - 18, posY - player->getPosY() + (SCREEN_HEIGHT / 2) - 20, maxHealth * 2, 3 };
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, newRect1);
		SDL_Rect* newRect2 = new SDL_Rect{ posX - player->getPosX() + (SCREEN_WIDTH / 2) - 18, posY - player->getPosY() + (SCREEN_HEIGHT / 2) - 20, health * 2, 3 };
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderFillRect(gRenderer, newRect2);
	}
}