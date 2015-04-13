#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

class Living;

#include "PointEntity.h"


#ifndef HEALTHBAR_H
#define HEALTHBAR_H
class Healthbar : public PointEntity
{
public:

	Healthbar();
	Healthbar(Living* x);

	~Healthbar();

	void update();

	void render();

	Living* getHolder();

private:

	Living* holder;

	int maxHealth = 10;

	int health = maxHealth;

	bool HUDbar = false;
};
#endif /* HEALTHBAR_H */