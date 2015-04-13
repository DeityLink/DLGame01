
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "PointEntity.h"

class Platform;

class Item : public PointEntity
{
public:
	Item();

	~Item();

	void update();

	void render();

private:

	void handleCollisions(Platform* P);

	int itemType = 0;

	int frames = 1;
	int frame = 0;
	int frameSpeed = 1;
	int tick = 0;

	float alpha = 255;

	int pickupRadius = 16;

	float posPrevX;
	float posPrevY;

	float velX = 0;
	float velY = 0;

	bool pickedUp = false;

	bool gravity;
	bool inertia;
};