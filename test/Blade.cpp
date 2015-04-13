#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "LTexture.h"
#include "Particle.h"

#include "Blade.h"

using namespace std;

extern LTexture blades;
extern SDL_Rect bladeClips[1];

extern vector<Enemy*> enemies;

extern double distPointToLine(float xA, float yA, float xB, float yB, float xP, float yP);
extern double distanceSquared(int x1, int y1, int x2, int y2);

Blade::Blade()
{
	bladeType = 0;

	flipSide = SDL_FLIP_NONE;

	center = { 16, 16 };

	angle = 0;

	swing = 0;
	swingSpeed = 0;
	bladeLength = 0;
	bladeRadius = 0;
	radius = 20;
}

Blade::Blade(Player* holder)
{
	slasher = holder;
	holder->blade = this;

	bladeType = 0;

	flipSide = SDL_FLIP_NONE;

	center = { 16, 16 };

	angle = 0;

	swing = 0;
	swingSpeed = 30;
	backswing = 10;
	bladeLength = 12;
	bladeRadius = 12;
	baseDamage = 2;
	radius = 20;
}

Blade::~Blade()
{

}

void Blade::slash()
{
	swing = swingSpeed;

	swingDir = slasher->takeAim();
}

float Blade::getSwingPhase()
{
	return ((10 * swing) / swingSpeed);
}

void Blade::attackUp()
{
	attackHorizontalUp();
}

void Blade::attackDown()
{
	if (slasher->isFalling())
	{
		if (getSwingPhase() >= 9.5)
		{
			targetAngle = 45 * getSide();
			xTargetOffset += 10 * getSide();
			yTargetOffset += 10;
		}
		else if (getSwingPhase() >= 7.5)
		{
			targetAngle = 90 * getSide();
			xTargetOffset += 2 * getSide();
			yTargetOffset += 24;
		}
		else if (getSwingPhase() >= 2.5)
		{
			targetAngle = 90 * getSide();
			xTargetOffset += 2 * getSide();
			yTargetOffset += 24;
		}
		else
		{
			targetAngle = 90 * getSide();
			xTargetOffset += 2 * getSide();
			yTargetOffset += 10;
		}
	}
	else
	{
		attackHorizontalDown();
	}
}

void Blade::attackHorizontal()
{
	if (getSwingPhase() >= 9.5)
	{
		xTargetOffset += 8 * getSide();
	}
	else if (getSwingPhase() >= 7.5)
	{
		xTargetOffset += 30 * getSide();
	}
	else if (getSwingPhase() >= 2.5)
	{
		xTargetOffset += 30 * getSide();
	}
	else
	{
		xTargetOffset += 8 * getSide();
	}
	yTargetOffset = 0;
	targetAngle = 0;
}

void Blade::attackHorizontalUp()
{
	if (getSwingPhase() >= 9.5)
	{
		targetAngle = 30 * getSide();
		xTargetOffset += 12 * getSide();
		yTargetOffset += 12;
	}
	else if (getSwingPhase() >= 7.5)
	{
		targetAngle = 30 * getSide();
		xTargetOffset += 20 * getSide();
		yTargetOffset += 20;
	}
	else if (getSwingPhase() >= 2.5)
	{
		targetAngle = -45 * getSide();
		xTargetOffset += 20 * getSide();
		yTargetOffset += -20;
	}
	else
	{
		targetAngle = -45 * getSide();
		xTargetOffset += 12 * getSide();
		yTargetOffset += -12;
	}
	if (flipSide == SDL_FLIP_HORIZONTAL)
		flipSide = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	else
		flipSide = SDL_FLIP_VERTICAL;
}

void Blade::attackHorizontalDown()
{
	if (getSwingPhase() >= 9.5)
	{
		targetAngle = -30 * getSide();
		xTargetOffset += 12 * getSide();
		yTargetOffset += -12;
	}
	else if (getSwingPhase() >= 7.5)
	{
		targetAngle = -30 * getSide();
		xTargetOffset += 20 * getSide();
		yTargetOffset += -20;
	}
	else if (getSwingPhase() >= 2.5)
	{
		targetAngle = 45 * getSide();
		xTargetOffset += 20 * getSide();
		yTargetOffset += 20;
	}
	else
	{
		targetAngle = 45 * getSide();
		xTargetOffset += 12 * getSide();
		yTargetOffset += 12;
	}
}

float Blade::getSide()
{
	if (flipSide == SDL_FLIP_NONE)
		return 1;
	else
		return -1;
}

int Blade::getSwingDir()
{
	return swingDir;
}

void Blade::update()
{
	int a = slasher->GetDirection();

	switch (a)
	{
	case WEST:
		flipSide = SDL_FLIP_HORIZONTAL;
		break;
	case EAST:
		flipSide = SDL_FLIP_NONE;
		break;
	}

	xTargetOffset = 0;
	yTargetOffset = 0;


	if (swing > 0)
	{
		switch (swingDir)
		{
		case NORTH:
			attackUp();
			break;
		case SOUTH:
			attackDown();
			break;
		case EAST:
			attackHorizontal();
			break;
		case WEST:
			attackHorizontal();
			break;
		case NORTHEAST:
			attackHorizontalUp();
			break;
		case NORTHWEST:
			attackHorizontalUp();
			break;
		case SOUTHEAST:
			attackHorizontalDown();
			break;
		case SOUTHWEST:
			attackHorizontalDown();
			break;
		default:
			int i = (rand() % 3);
			if (getSide() == 1)
			{
				if (i == 0)
				{
					swingDir = EAST;
					attackHorizontal();
				}
				else if (i == 1)
				{
					swingDir = NORTHEAST;
					attackHorizontalUp();
				}
				else if (i == 2)
				{
					swingDir = SOUTHEAST;
					attackHorizontalDown();
				}
			}
			else
			{
				if (i == 0)
				{
					swingDir = WEST;
					attackHorizontal();
				}
				else if (i == 1)
				{
					swingDir = NORTHWEST;
					attackHorizontalUp();
				}
				else if (i == 2)
				{
					swingDir = SOUTHWEST;
					attackHorizontalDown();
				}
			}
			break;
		}

		for (int n = 0; n < (int)enemies.size(); n++)
		{
			handleCollisions(enemies[n]);
		}

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
	posX = slasher->getPosX() + xOffset;
	posY = slasher->getPosY() + yOffset;

	if (swing > 0)
		swing--;
	else
		swing = 0;
}

void Blade::handleCollisions(Enemy* E)
{
	float procAng = angle;
	if (flipSide == SDL_FLIP_HORIZONTAL)
		procAng = (-1 * angle + 180);

	float LAx = (float)cos((procAng + 180)*(M_PI / 180)) * 6 + posX;
	float LAy = (float)sin((procAng + 180)*(M_PI / 180)) * 6 + posY;
	float LBx = (float)cos(procAng*(M_PI / 180)) * bladeLength + posX;
	float LBy = (float)sin(procAng*(M_PI / 180)) * bladeLength + posY;
	float Px = E->getPosX();
	float Py = E->getPosY();

	/*
	Particle* Pa = new Particle("bulletImpact", LAx, LAy, 0);
	Particle* Pb = new Particle("bulletImpact", LBx, LBy, 0);
	Particle* Pc = new Particle("bulletImpact", Px, Py, 0);
	*/

	float dist = distPointToLine(LAx, LAy, LBx, LBy, Px, Py);
	if (dist != -1)
	{
		if (dist <= (bladeRadius + E->getRadius()))
		{
			if (!E->getInvuln(1))
			{
				float ang = atan2((slasher->getPosY() - E->getPosY()), (slasher->getPosX() - E->getPosX())) * M_PI / 180;
				if (flipSide == SDL_FLIP_HORIZONTAL)
					ang = (-1 * ang + 180);

				E->setInvuln(1, 30);
				E->takeDamage(calculateDamage(E));
				E->bump(slasher->getPosX(), slasher->getPosY(), ang, calculateKnockback());
			}
		}
	}
	else
	{
		if (sqrt(distanceSquared(LBx, LBy, Px, Py)) <= E->getRadius())
		{
			if (!E->getInvuln(1))
			{
				float ang = atan2((slasher->getPosY() - E->getPosY()), (slasher->getPosX() - E->getPosX())) * M_PI / 180;
				if (flipSide == SDL_FLIP_HORIZONTAL)
					ang = (-1 * ang + 180);

				E->setInvuln(1, 30);
				E->takeDamage(calculateDamage(E));
				E->bump(slasher->getPosX(), slasher->getPosY(), ang, calculateKnockback());
			}
		}
	}
}

float Blade::calculateDamage(Enemy* E)
{
	float damageDealt = baseDamage;
	if (getSwingPhase() >= 7.5)
		damageDealt += baseDamage*0.2;
	else if (getSwingPhase() <= 2.5)
		damageDealt -= baseDamage*0.2;
	if ((swingDir == SOUTH) && slasher->isFalling())
		damageDealt += baseDamage*0.5;
	if (((E->getPosX() - slasher->getPosX()) * E->getFacing()) < 0)
		damageDealt += baseDamage;

	return damageDealt;
}

float Blade::calculateKnockback()
{
	float knockback = baseDamage;
	if (getSwingPhase() >= 7.5)
		knockback += baseDamage*0.2;
	else if (getSwingPhase() <= 2.5)
		knockback -= baseDamage*0.2;
	if ((swingDir == EAST) || (swingDir == WEST))
		knockback += baseDamage*0.7;

	return knockback;
}

float Blade::getSwingSpeed()
{
	return swingSpeed;
}

float Blade::getBackSwing()
{
	return backswing;
}

void Blade::render()
{
	if (swing > 0)
		blades.render(posX - (bladeClips[bladeType].w / 2), posY - (bladeClips[bladeType].h / 2), &bladeClips[bladeType], angle, &center, flipSide);
}