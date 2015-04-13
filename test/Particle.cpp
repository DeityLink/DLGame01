#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <vector>

#include "LTexture.h"
#include "Platform.h"

#include "Particle.h"

using namespace std;

extern LTexture particles_sheet;
extern SDL_Rect particleClips[2];

extern vector<Particle*> particles;
extern vector<Platform*> platforms;

extern bool compareStrings(string A, string B);

Particle::Particle()
{
	particles.push_back(this);

	posX = 0;
	posY = 0;
	posPrevX = 0;
	posPrevY = 0;
	velX = 0;
	velY = 0;
	angle = 0;
	angleVel = 0;
	startAlpha = 0;
	alpha = 0;
	lifetimeInit = 0;
	lifetime = 0;
	clip = 0;
	radius = 1;
	gravity = false;

	killSelf = false;
}

Particle::Particle(string s, float x, float y, float a)
{
	particles.push_back(this);

	posX = x;
	posY = y;
	posPrevX = x;
	posPrevY = y;
	velX = 0;
	velY = 0;
	angle = a;
	angleVel = 0;
	alpha = 0;
	radius = 1;
	lifetimeInit = 0;
	lifetime = 0;
	gravity = false;
	inertia = false;
	solid = false;

	killSelf = false;

	string particle1 = "bulletCase";
	string particle2 = "bulletImpact";
	string particle3 = "bloodGush";

	if (compareStrings(s, particle1))
	{
		clip = 0;
		lifetimeInit = 200;
		startAlpha = 250;
		gravity = true;
		inertia = true;
		solid = true;
		velX = (fmod(rand(),6))-3;
		velY = -16;
		angleVel = 10;
		radius = 1;
	}
	else if (compareStrings(s, particle2))
	{
		clip = 1;
		lifetimeInit = 100;
		startAlpha = 220;
	}
	else if (compareStrings(s, particle3))
	{
		clip = 2;
		frameSpeed = 2;
		once = true;
		frames = 3;
		lifetimeInit = 100;
		startAlpha = 220;
	}

	alpha = startAlpha;
	lifetime = lifetimeInit;
}

Particle::~Particle()
{
	//Deallocate
}

void Particle::currentAlpha()
{
	alpha = (startAlpha * lifetime) / lifetimeInit;
}

bool Particle::isKill()
{
	return killSelf;
}

void Particle::handleCollisions(Platform* P)
{
	SDL_Rect* R = P->getRekt();

	//LR check
	bool LR = true;
	if ((((posX - radius) >= (R->x + R->w)) && ((posX + radius) >= (R->x + R->w))) || (((posX - radius) <= (R->x)) && ((posX + radius) <= (R->x))))
		if ((((posPrevX - radius) >= (R->x + R->w)) && ((posPrevX + radius) >= (R->x + R->w))) || (((posPrevX - radius) <= (R->x)) && ((posPrevX + radius) <= (R->x))))
			LR = false;

	//UD check
	bool UD = true;
	if ((((posY - radius) > (R->y + R->h)) && ((posY + radius) > (R->y + R->h))) || (((posY - radius) < (R->y)) && ((posY + radius) < (R->y))))
		if ((((posPrevY - radius) > (R->y + R->h)) && ((posPrevY + radius) > (R->y + R->h))) || (((posPrevY - radius) < (R->y)) && ((posPrevY + radius) < (R->y))))
			UD = false;

	//downward check
	bool crossDown = false;
	if (((posY + radius) > (R->y)) && ((posPrevY + radius) <= (R->y)))
		crossDown = true;

	//upward check
	bool crossUp = false;
	if (((posY + radius - radius) < (R->y + R->h)) && ((posPrevY + radius - radius) >= (R->y + R->h)))
		crossUp = true;

	//leftward check
	bool crossLeft = false;
	if (((posX + radius) > (R->x)) && ((posPrevX + radius) <= (R->x)))
		crossLeft = true;

	//rightward check
	bool crossRight = false;
	if (((posX - radius) < (R->x + R->w)) && ((posPrevX - radius) >= (R->x + R->w)))
		crossRight = true;

	//bounce off the top of the rectangle
	if (LR && crossDown)
	{
		posY = (R->y) - radius;
		velY = -1 * velY;
		if (abs(velY) <= 1)
		{
			gravity = false;
			angleVel = 0;
			velX = 0;
		}
	}

	//bounce off the bottom of the rectangle
	if (LR && crossUp)
	{
		posY = (R->y + R->h) + (particleClips[clip].h / 2);
		velY = -1 * velY;
	}

	//bounce off the left side of the rectangle
	if (UD && crossLeft)
	{
		posX = (R->x) - radius;
		velX = -1 * velX;
	}

	//bounce off the right side of the rectangle
	if (UD && crossRight)
	{
		posX = (R->x + R->w) + radius;
		velX = -1 * velX;
	}
}

void Particle::update()
{
	angle += angleVel*(abs(velX) + abs(velY));
	angle = fmod(angle, 360);

	tick++;
	if (tick == frameSpeed)
	{
		frame++;
		tick = 0;
	}

	if (frame == frames)
	{
		if (once)
			killSelf = true;
		else
			frame = 0;
	}

	if (inertia)
	{
		if ((velX > 0) && (!gravity))
			velX--;
		if ((velX < 0) && (!gravity))
			velX++;

		if ((velY > 0) && (!gravity))
			velY--;
		if (velY < 0)
			velY++;
	}

	if (gravity)
	{
		velY++;
	}

	posPrevX = posX;
	posPrevY = posY;

	posX += velX;
	posY += velY;

	lifetime--;
	if (lifetime <= 0)
		killSelf = true;

	currentAlpha();

	//Check for Particle-Platform collisions
	if (solid)
	{
		for (int n = 0; n < (int)platforms.size(); n++)
		{
			handleCollisions(platforms[n]);
		}
	}
}

void Particle::render()
{
	particles_sheet.setAlpha(alpha);
	particles_sheet.render(posX-16, posY-16, &particleClips[clip+frame], angle, NULL, SDL_FLIP_NONE);
}