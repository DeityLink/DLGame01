
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

#include "Projectile.h"
#include "Particle.h"
#include "LTexture.h"
#include "Player.h"

#include "Weapon.h"

extern LTexture weapons;
extern SDL_Rect gunClips[3];

Weapon::Weapon()
{
	gunner = nullptr;

	fireState = 0;
	fireDelay = 0;
	center = { 29, 18 };
	angle = 0;
	radius = 20;
}

Weapon::Weapon(Player* holder)
{
	gunner = holder;
	holder->weapon = this;

	fireState = 0;
	fireDelay = 0;
	center = { 7, 18 };
	projectileType = "deagle";
	gunType = "deagle";
	ammo = 999;
	angle = 0;
	flipSide = SDL_FLIP_NONE;
	xOffset = 27;
	yOffset = 0;
	recoil = -1;
	radius = 20;
}

Weapon::~Weapon()
{
	//Deallocate
}

float Weapon::getBarrelPos()
{
	int i = -1;
	if (flipSide == SDL_FLIP_HORIZONTAL)
		i = 1;
	return (posX + (21 - center.x)*(float)cos(angle*(M_PI / 180)) + (15 - center.y)*i*(float)sin(angle*(M_PI / 180)) + (i * 8));
}

float Weapon::getPosY()
{
	return posY + yOffset;
}

float Weapon::getAngle()
{
	if (flipSide == SDL_FLIP_NONE)
		return angle;
	else
		return (-1 * angle + 180);
}

float Weapon::getSide()
{
	if (flipSide == SDL_FLIP_NONE)
		return 1;
	else
		return -1;
}

int Weapon::getAmmo()
{
	return ammo;
}

string Weapon::getType()
{
	return gunType;
}

void Weapon::fire()
{
	if (ammo && !fireDelay)
	{
		fireDelay = 20;
		fireState = 10;

		int i = 1;
		if (flipSide == SDL_FLIP_NONE)
			i = -1;

		Projectile* p = new Projectile(*this);
		Particle* Pa = new Particle("bulletCase", posX + 8*i, posY, 0);

		ammo--;
	}
}

void Weapon::update()
{
	int a = gunner->takeAim();
	if (a)
	{
		int b = gunner->GetDirection();

		switch (a)
		{
			case NORTH:
				targetAngle = -90;
				yTargetOffset = -16;

				if (b == EAST)
					xTargetOffset = 16;
				else
					xTargetOffset = -16;

				break;
			case SOUTH:
				targetAngle = 90;
				yTargetOffset = 16;

				if (b == EAST)
					xTargetOffset = 16;
				else
					xTargetOffset = -16;

				break;
			case EAST:
				targetAngle = 0;
				xTargetOffset = 27;
				yTargetOffset = 0;
				center = { 7, 18 };
				recoil = -1;
				flipSide = SDL_FLIP_NONE;
				break;
			case WEST:
				targetAngle = 0;
				xTargetOffset = -27;
				yTargetOffset = 0;
				center = { 25, 18 };
				recoil = 1;
				flipSide = SDL_FLIP_HORIZONTAL;
				break;
			case NORTHEAST:
				targetAngle = -45;
				xTargetOffset = 27;
				yTargetOffset = -8;
				center = { 7, 18 };
				recoil = -1;
				flipSide = SDL_FLIP_NONE;
				break;
			case NORTHWEST:
				targetAngle = -45;
				xTargetOffset = -27;
				yTargetOffset = -8;
				center = { 25, 18 };
				recoil = 1;
				flipSide = SDL_FLIP_HORIZONTAL;
				break;
			case SOUTHEAST:
				targetAngle	= 45;
				xTargetOffset = 27;
				yTargetOffset = 8;
				center = { 7, 18 };
				recoil = -1;
				flipSide = SDL_FLIP_NONE;
				break;
			case SOUTHWEST:
				targetAngle = 45;
				xTargetOffset = -27;
				yTargetOffset = 8;
				center = { 25, 18 };
				recoil = 1;
				flipSide = SDL_FLIP_HORIZONTAL;
				break;
		}
	}
	else
	{
		targetAngle = 0;
		a = gunner->GetDirection();
		int i = 1;

		switch (a)
		{
			case WEST:
				i = -1;
				flipSide = SDL_FLIP_HORIZONTAL;
				break;
			case EAST:
				flipSide = SDL_FLIP_NONE;
				break;
		}

		xTargetOffset = 27 * i;
		yTargetOffset = 0;
	}
	if (offhand)
	{
		targetAngle = 0;
		xTargetOffset = 0;
		yTargetOffset = 8;
	}

	//smoothing xOffset
	int xA = (abs(xOffset) - abs(xTargetOffset));

	if ((xA < 2) && (xA > -2))
		xOffset = xTargetOffset;
	else
	{
		if (xOffset > xTargetOffset)
			xOffset = xTargetOffset + ((xOffset - xTargetOffset) / 2);
		else
			xOffset = xTargetOffset - ((xTargetOffset - xOffset) / 2);
	}

	//smoothing yOffset
	int yA = (abs(yOffset) - abs(yTargetOffset));

	if ((yA < 2) && (yA > -2))
		yOffset = yTargetOffset;
	else
	{
		if (yOffset > yTargetOffset)
			yOffset = yTargetOffset + ((yOffset - yTargetOffset) / 2);
		else
			yOffset = yTargetOffset - ((yTargetOffset - yOffset) / 2);
	}

	//smoothing angle
	int zA = (abs(angle) - abs(targetAngle));

	if ((zA < 2) && (zA > -2))
		angle = targetAngle;
	else
	{
		if (angle > targetAngle)
			angle = targetAngle + ((angle - targetAngle) / 2);
		else
			angle = targetAngle - ((targetAngle - angle) / 2);
	}

	//moving the gun onto the player
	posX = gunner->getPosX() + xOffset;
	posY = gunner->getPosY() + yOffset;

	if (fireState > 0)
		fireState--;
	if (fireDelay > 0)
		fireDelay--;
}

void Weapon::offhandSwitch(bool state)
{
	offhand = state;
}

void Weapon::render()
{
	int i = 0;
	if (fireState > 2)
		i = 1;
	if (fireState > 5)
		i = 2;

	weapons.render(posX - (gunClips[i].w / 2), posY - (gunClips[i].h / 2), &gunClips[i], (recoil*fireState) + (-1 * recoil*angle), &center, flipSide);
}
