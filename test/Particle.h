
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

#include "PointEntity.h"

using namespace std;

class Platform;

class Particle : public PointEntity
{
public:

	Particle();
	Particle(string s, float x, float y, float a);

	~Particle();

	void update();

	void render();

	bool isKill();

private:

	void handleCollisions(Platform* P);

	int clip;
	int frames = 1;
	int frame = 0;

	bool once = false;

	int frameSpeed = 1;
	int tick = 0;

	int radius;

	float posPrevX;
	float posPrevY;

	float velX;
	float velY;

	float angle;
	float angleVel;

	float startAlpha;
	float alpha;
	void currentAlpha();

	int lifetimeInit;
	int lifetime;

	bool solid;
	bool gravity;
	bool inertia;

	bool killSelf;
};