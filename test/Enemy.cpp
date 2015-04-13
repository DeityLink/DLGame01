#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <algorithm>

#include "Platform.h"
#include "Player.h"
#include "Particle.h"
#include "LTexture.h"
#include "Projectile.h"

#include "Enemy.h"

extern LTexture units;
extern SDL_Rect enemyClips[3];

extern Player* player;
extern vector<Enemy*> enemies;
extern vector<Platform*> platforms;
extern vector<Projectile*> projectiles;

extern double distanceSquared(int x1, int y1, int x2, int y2);

Enemy::Enemy()
{
	enemies.push_back(this);
	posX = 0;
	posY = 0;
	posPrevX = 0;
	posPrevY = 0;
	radius = 16;
}

Enemy::Enemy(int x, int y)
{
	enemies.push_back(this);
	posX = x;
	posY = y;
	posPrevX = x;
	posPrevY = y;
	radius = 16;

	gravity = true;
	moving = true;

	damage = 10;

}

Enemy::~Enemy()
{

}

void Enemy::handleCollisions(Platform* P)
{
	SDL_Rect* R = P->getRekt();

	//LR check
	bool LR = true;
	if ((((posX - width / 2) >= (R->x + R->w)) && ((posX + width / 2) >= (R->x + R->w))) || (((posX - width / 2) <= (R->x)) && ((posX + width / 2) <= (R->x))))
		if ((((posPrevX - width / 2) >= (R->x + R->w)) && ((posPrevX + width / 2) >= (R->x + R->w))) || (((posPrevX - width / 2) <= (R->x)) && ((posPrevX + width / 2) <= (R->x))))
			LR = false;

	//UD check
	bool UD = true;
	if ((((posY - height / 2) > (R->y + R->h)) && ((posY + height / 2) > (R->y + R->h))) || (((posY - height / 2) < (R->y)) && ((posY + height / 2) < (R->y))))
		if ((((posPrevY - height / 2) > (R->y + R->h)) && ((posPrevY + height / 2) > (R->y + R->h))) || (((posPrevY - height / 2) < (R->y)) && ((posPrevY + height / 2) < (R->y))))
			UD = false;

	//downward check
	bool crossDown = false;
	if (((posY + height / 2) > (R->y)) && ((posPrevY + height / 2) <= (R->y)))
		crossDown = true;

	//upward check
	bool crossUp = false;
	if (((posY - height / 2) < (R->y + R->h)) && ((posPrevY - height / 2) >= (R->y + R->h)))
		crossUp = true;

	//leftward check
	bool crossLeft = false;
	if (((posX + width / 2) > (R->x)) && ((posPrevX + width / 2) <= (R->x)))
		crossLeft = true;

	//rightward check
	bool crossRight = false;
	if (((posX - width / 2) < (R->x + R->w)) && ((posPrevX - width / 2) >= (R->x + R->w)))
		crossRight = true;

	//hug top of the rectangle (walking on top of a platform)
	if (LR && crossDown)
	{
		posY = (R->y) - height / 2;
		falling = false;
		Support = P;
	}

	//hug bottom of the rectangle (headbutting the ceiling)
	if (LR && crossUp)
	{
		posY = (R->y + R->h) + height / 2;
		speedY = 0;
	}

	//hug left side of the rectangle
	if (UD && crossLeft)
	{
		posX = (R->x) - width / 2;
		facing = -1;
		speedX = 0;
	}

	//hug right side of the rectangle
	if (UD && crossRight)
	{
		posX = (R->x + R->w) + width / 2;
		facing = 1;
		speedX = 0;
	}

	if ((Support == P) && !LR)
	{
		Support = nullptr;
		falling = true;
	}
}

void Enemy::handleCollisions(Projectile* P)
{
	if ((distanceSquared(P->getPosX(), P->getPosY(), posX, posY)) < ((width / 2)*(width / 2) + (P->getRadius())*(P->getRadius())))
	{
		P->terminate();
		takeDamage(P->getDamage());
		bump(P->getPosX(), P->getPosY(), P->getAngle(), P->getDamage());
		Particle* Pa = new Particle("bloodGush", P->getPosX(), P->getPosY(), rand() % 360);
	}
}

void Enemy::handleCollisions(Player* P)
{
	float distance = (distanceSquared(P->getPosX(), P->getPosY(), posX, posY)) - ((width / 2)*(width / 2) + (P->getRadius())*(P->getRadius()));
	if (distance < 0)
	{
		if ((cooldown == 0) && (!invulnBlade))
		{
			float ang = atan2((posY - P->getPosY()), (posX - P->getPosX())) * M_PI / 180;
			if (facing == -1)
				ang = (-1 * ang + 180);

			cooldown = attackCooldown;
			attackState = 2;
			P->takeDamage(damage);
			P->bump(posX, posY, ang, damage);
		}
	}
	else if (distance < 5000)
	{
		if ((cooldown == 0) && (((P->getPosX() - posX) * facing) > 0 ))
		{
			attackState = 1;
			dash = true;
		}
		else
		{
			dash = false;
		}
	}
	else
	{
		attackState = 0;
		dash = false;
	}
}

bool Enemy::getInvuln(int type)
{
	bool result = false;
	switch (type)
	{
	case 1:
		if (invulnBlade > 0)
			result = true;
		break;
	default:
		break;
	}

	return result;
}

void Enemy::setInvuln(int type, int length)
{
	switch (type)
	{
	case 1:
		invulnBlade = max(invulnBlade, length);
		break;
	default:
		break;
	}
}

float Enemy::getFacing()
{
	return facing;
}

float Enemy::updateFacing()
{
	if (facing == 1)
		orientation = SDL_FLIP_NONE;
	else
		orientation = SDL_FLIP_HORIZONTAL;
	return facing;
}

void Enemy::move()
{
	if (moving)
	{
		updateFacing();

		if (facing == 1)
		{
			if (speedX < (walkspeed - 1 + (dash*2)))
				speedX = speedX + 1  + (dash*2);
			else if (speedX >(walkspeed + 1))
				speedX--;
			else
				speedX = walkspeed;
		}
		if (facing == -1)
		{
			if (speedX > ((walkspeed * -1) + 1 - (dash*2)))
				speedX = speedX - 1 - (dash*2);
			else if (speedX < ((walkspeed * -1) - 1))
				speedX++;
			else
				speedX = (walkspeed * -1);
		}
	}

	posPrevX = posX;
	posPrevY = posY;

	if (gravity && falling)
	{
		if (speedY < ENEMY_TERMINAL_VEL)
			speedY++;
	}
	else
		speedY = 0;

	//actual movement
	posX += speedX;
	posY += speedY;


	//Check for Platform collisions
	for (int n = 0; n < (int)platforms.size(); n++)
	{
		handleCollisions(platforms[n]);
	}
}

void Enemy::takeDamage(float dam)
{
	pain += 15 * dam;
	if (pain > 255)
		pain = 255;
	health -= dam;
}

void Enemy::bump(float x, float y, float a, int str)
{
	if (!falling)
	{
		falling = true;
		speedY -= str;
		posY -= 1;
		speedX += ((float)cos(a*(M_PI / 180)) * str);
	}
	else
	{
		speedX += ((float)cos(a*(M_PI / 180)))* str;
		speedY += ((float)sin(a*(M_PI / 180)))* str;
	}

	if (((x - posX) * facing) < 0)
		facing = facing *-1;
}

void Enemy::update()
{
	if (invulnBlade > 0)
		invulnBlade--;

	if (cooldown > 0)
		cooldown--;

	if (pain > 6)
		pain -= 6;
	else
		pain = 0;
	move();

	for (int n = 0; n < (int)projectiles.size(); n++)
	{
		handleCollisions(projectiles[n]);
	}

	handleCollisions(player);
}

void Enemy::render()
{
	units.setColor(255, 255 - pain, 255 - pain);
	units.render(posX - (enemyClips[attackState].w / 2), posY - (enemyClips[attackState].h / 2), &enemyClips[attackState], NULL, NULL, orientation);
}
