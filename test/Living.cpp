#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#include "Living.h"

using namespace std;

Living::Living()
{

	healthbar = new Healthbar(this);

}

Living::~Living()
{

}

bool Living::isKill()
{
	return (health <= 0);
}

int Living::getHealth()
{
	return health;
}

int Living::getMaxHealth()
{
	return maxHealth;
}
