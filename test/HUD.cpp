#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "LTexture.h"
#include "Player.h"
#include "Weapon.h"

#include "HUD.h"

extern Player* player;

extern LTexture HUDshell;
extern SDL_Rect HUDshellClips[1];

extern LTexture numbers;
extern SDL_Rect numberClips[11];

using namespace std;

HUD::HUD()
{

}

HUD::~HUD()
{

}

void HUD::update()
{
	if (player->weapon != nullptr)
	{
		gun = player->weapon->getType();
		ammo = player->weapon->getAmmo();
	}
}

void HUD::render()
{
	//shell
	HUDshell.render(0, 0, &HUDshellClips[0], NULL, NULL, SDL_FLIP_NONE, true);

	//lives counter
	numbers.render(39, 478, &numberClips[lives], NULL, NULL, SDL_FLIP_NONE, true);

	//ammo counter
	if (gun == "")
	{
		numbers.render(44, 438, &numberClips[10], NULL, NULL, SDL_FLIP_NONE, true);
		numbers.render(64, 438, &numberClips[10], NULL, NULL, SDL_FLIP_NONE, true);
		numbers.render(84, 438, &numberClips[10], NULL, NULL, SDL_FLIP_NONE, true);
	}
	else
	{
		int a = ammo / 100;
		int b = (ammo / 10) % 10;
		int c = ammo % 10;
		numbers.render(44, 438, &numberClips[a], NULL, NULL, SDL_FLIP_NONE, true);
		numbers.render(64, 438, &numberClips[b], NULL, NULL, SDL_FLIP_NONE, true);
		numbers.render(84, 438, &numberClips[c], NULL, NULL, SDL_FLIP_NONE, true);
	}

}
