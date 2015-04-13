#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Healthbar.h"

#include "PointEntity.h"

using namespace std;

#ifndef LIVING_H
#define LIVING_H
class Living : public PointEntity
{
public:

	Living();

	~Living();

	Healthbar* healthbar;

	bool isKill();

	int getHealth();
	int getMaxHealth();

protected:

	int knockback = 0;

	int maxHealth = 10;

	int health = maxHealth;
};
#endif /* LIVING_H */