#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <vector>

#include "Weapon.h"
#include "LTexture.h"
#include "Platform.h"
#include "Particle.h"

#include "Projectile.h"

using namespace std;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern LTexture weapons;
extern SDL_Rect bulletClips[1];

extern vector<Projectile*> projectiles;
extern vector<Platform*> platforms;

extern double distanceSquared(int x1, int y1, int x2, int y2);

Projectile::Projectile()
{
	projectiles.push_back(this);
	posX = 0;
	posY = 0;
	angle = 0;

	radius = 1;
	speed = 0;

	killself = true;

	col.r = radius;
	moveCol();
}

Projectile::Projectile(Weapon source)
{
	projectiles.push_back(this);
	posX = source.getBarrelPos();
	posY = source.getPosY();
	angle = source.getAngle();

	damage = 2;
	radius = 1;
	speed = 10;

	sourceGunX = source.getBarrelPos();
	sourceGunY = posY;

	killself = false;

	col.r = radius;
	moveCol();

	flipSide = SDL_FLIP_NONE;
	if ((angle > 90) || (angle < -90))
		flipSide = SDL_FLIP_VERTICAL;
}

Projectile::~Projectile()
{
	//Deallocate
}

bool Projectile::isKill()
{
	return killself;
}

void Projectile::move()
{
	posPrevX = posX;
	posPrevY = posY;

	posX = posX + ((float)cos(angle*(M_PI / 180)) * speed);
	posY = posY + ((float)sin(angle*(M_PI / 180)) * speed);

	moveCol();

	//Check for Projectile-Platform collisions
	for (int n = 0; n < (int)platforms.size(); n++)
	{
		handleCollisions(platforms[n]);
	}

	/*
	if ((posX < 0) || (posX > SCREEN_WIDTH))
		killself = true;
	if ((posY < 0) || (posY > SCREEN_HEIGHT))
		killself = true;
	*/
}

int Projectile::getAngle()
{
	return angle;
}

void Projectile::handleCollisions(Platform* P)

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
		killself = true;
		Particle* Pa = new Particle("bulletImpact", posX, R->y, 90);
	}

	//bounce off the bottom of the rectangle
	if (LR && crossUp)
	{
		killself = true;
		Particle* Pa = new Particle("bulletImpact", posX, (R->y + R->h)-1, -90);
	}

	//bounce off the left side of the rectangle
	if (UD && crossLeft)
	{
		killself = true;
		Particle* Pa = new Particle("bulletImpact", R->x, posY, 0);
	}

	//bounce off the right side of the rectangle
	if (UD && crossRight)
	{
		killself = true;
		Particle* Pa = new Particle("bulletImpact", (R->x + R->w)-1, posY, 180);
	}

	checkCollision(R);
}

void Projectile::terminate()
{
	killself = true;
}

void Projectile::checkCollision(SDL_Rect* b)
{
	Circle& a = col;

	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (a.x < b->x)
	{
		cX = b->x;
	}
	else if (a.x > b->x + b->w)
	{
		cX = b->x + b->w;
	}
	else
	{
		cX = a.x;
	}

	//Find closest y offset
	if (a.y < b->y)
	{
		cY = b->y;
	}
	else if (a.y > b->y + b->h)
	{
		cY = b->y + b->h;
	}
	else
	{
		cY = a.y;
	}

	//If the closest point is inside the circle
	if (distanceSquared(a.x, a.y, cX, cY) < 1)
	{

		if (!killself)
		{
			int A = abs(abs(sourceGunX) - (b->x));
			int B = abs(abs(sourceGunX) - (b->x + b->w));
			int C = abs(abs(sourceGunY) - (b->y));
			int D = abs(abs(sourceGunY) - (b->y + b->h));

			int E = min(min(A, B), min(C, D));

			if (E == B)
				Particle* Pa = new Particle("bulletImpact", (b->x + b->w) - 1, posY, 180);
			else if (E == A)
				Particle* Pa = new Particle("bulletImpact", b->x, posY, 0);
			else if (E == C)
				Particle* Pa = new Particle("bulletImpact", posX, b->y, 90);
			else if (E == D)
				Particle* Pa = new Particle("bulletImpact", posX, (b->y + b->h) - 1, -90);
		}

		killself = true;

		onCollision();
	}
}

int Projectile::getRadius()
{
	return radius;
}

int Projectile::getDamage()
{
	return damage;
}

void Projectile::moveCol()
{
	//Align collider to center of dot
	col.x = posX;
	col.y = posY;
}

void Projectile::onCollision()
{

}

void Projectile::render()
{
	weapons.render(posX - (bulletClips[0].w / 2), posY - (bulletClips[0].h / 2), &bulletClips[0], angle, NULL, flipSide);
}
