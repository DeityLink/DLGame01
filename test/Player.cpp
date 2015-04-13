
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>

#include "Weapon.h"
#include "Enemy.h"
#include "Blade.h"
#include "LTexture.h"
#include "Particle.h"
#include "Platform.h"

#include "Player.h"

extern LTexture units;
extern SDL_Rect playerClips[3];

//extern int SCREEN_WIDTH;
//extern int SCREEN_HEIGHT;


Player::Player()
{
	//Initialize the offsets
	posX = 100;
	posY = 100;
	posPrevX = 100;
	posPrevY = 100;
	radius = 16;
	//Initialize the velocity
	speedX_Right = 0;
	speedX_Left = 0;
	speedY = 0;

	orientation = SDL_FLIP_NONE;

	keyDownLeft = false;
	keyDownRight = false;
	keyDownUp = false;

	crouched = false;

	firing = false;
	slashing = false;

	weapon = nullptr;
	blade = nullptr;

	aim = 0;

	falling = true;

	freeze = false;

	jump = 0;

	maxHealth = 20;
	health = maxHealth;
}

Player::Player(int x, int y)
{
	//Initialize the offsets
	posX = x;
	posY = y;
	posPrevX = x;
	posPrevY = y;
	radius = 16;
	//Initialize the velocity
	speedX_Right = 0;
	speedX_Left = 0;
	speedY = 0;

	orientation = SDL_FLIP_NONE;

	keyDownLeft = false;
	keyDownRight = false;
	keyDownUp = false;

	crouched = false;

	firing = false;
	slashing = false;

	weapon = nullptr;
	blade = nullptr;

	aim = 0;

	falling = true;

	freeze = false;

	jump = 0;

	maxHealth = 20;
	health = maxHealth;
}

Player::~Player()
{
	//Deallocate
}

void Player::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			keyDownUp = true;
			break;
		case SDLK_DOWN:
			crouched = true;
			break;
		case SDLK_LEFT:
			orientation = SDL_FLIP_HORIZONTAL;
			keyDownLeft = true;
			break;
		case SDLK_RIGHT:
			orientation = SDL_FLIP_NONE;
			keyDownRight = true;
			break;
		case SDLK_w:
			firing = true;
			break;
		case SDLK_x:
			slashing = true;
			break;
		case SDLK_LCTRL:
			freeze = true;
			break;
		case SDLK_SPACE:
			if (!falling)
			{
				falling = true;
				jump = 10;
			}
			break;
		//DEBUG
		case SDLK_e:
			Enemy* e = new Enemy(posX + 64, posY - 64);
			break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
			keyDownUp = false;
			break;
		case SDLK_DOWN:
			crouched = false;
			break;
		case SDLK_LEFT:
			keyDownLeft = false;
			if (keyDownRight)
				orientation = SDL_FLIP_NONE;
			break;
		case SDLK_RIGHT:
			keyDownRight = false;
			if (keyDownLeft)
				orientation = SDL_FLIP_HORIZONTAL;
			break;
		case SDLK_w:
			firing = false;
			break;
		case SDLK_x:
			slashing = false;
			break;
		case SDLK_LCTRL:
			freeze = false;
			break;
		case SDLK_SPACE:
			jump = 0;
		}
	}
}

void Player::move()
{
	if (falling && (knockback > 0))
	{
		knockback--;
		if (crouched)
			knockback--;
	}
	else
	{
		knockback = 0;

		//acceleration
		if ((!freeze && keyDownLeft) && speedX_Left < (maxSpeed - (crouched*maxSpeed / 2)))
			speedX_Left = speedX_Left + (maxSpeed / 10);
		if ((!freeze && keyDownRight) && speedX_Right < (maxSpeed - (crouched*maxSpeed / 2)))
			speedX_Right = speedX_Right + (maxSpeed / 10);

		//speed limit
		if (speedX_Left > maxSpeed)
			speedX_Left = maxSpeed;
		if (speedX_Right > maxSpeed)
			speedX_Right = maxSpeed;

		//deceleration
		if (((freeze && !falling) || (!keyDownLeft && !freeze)) && (speedX_Left > 0))
			speedX_Left = speedX_Left - (maxSpeed / 10);
		if (((freeze && !falling) || (!keyDownRight && !freeze)) && (speedX_Right > 0))
			speedX_Right = speedX_Right - (maxSpeed / 10);
	}

	//storing previous position
	posPrevX = posX;
	posPrevY = posY;

	//gravity
	if (falling)
	{
		if (jump > 0)
		{
			speedY = -10;
			jump--;
		}
		else if (speedY < PLAYER_TERMINAL_VEL)
			speedY++;
	}
	else
		speedY = 0;

	if (!knockback)
	{
		if (speedX >= 1)
			speedX--;
		else if (speedX <= -1)
			speedX++;
		else
			speedX = 0;
	}

	//actual movement
	posX += speedX;
	posX += speedX_Right;
	posX -= speedX_Left;
	posY += speedY;

	/*
	//screen collision
	if (posX < (PLAYER_WIDTH/2))
	{
		posX = PLAYER_WIDTH/2;
	}
	if (posX + (PLAYER_WIDTH/2) > SCREEN_WIDTH)
	{
		posX = SCREEN_WIDTH - (PLAYER_WIDTH/2);
	}
	*/
}

void Player::handleCollisions(Platform* P)
{
	SDL_Rect* R = P->getRekt();

	//LR check
	bool LR = true;
	if ((((posX - PLAYER_WIDTH / 2) >= (R->x + R->w)) && ((posX + PLAYER_WIDTH / 2) >= (R->x + R->w))) || (((posX - PLAYER_WIDTH / 2) <= (R->x)) && ((posX + PLAYER_WIDTH / 2) <= (R->x))))
		if ((((posPrevX - PLAYER_WIDTH / 2) >= (R->x + R->w)) && ((posPrevX + PLAYER_WIDTH / 2) >= (R->x + R->w))) || (((posPrevX - PLAYER_WIDTH / 2) <= (R->x)) && ((posPrevX + PLAYER_WIDTH / 2) <= (R->x))))
			LR = false;

	//UD check
	bool UD = true;
	if ((((posY - PLAYER_HEIGHT / 2) > (R->y + R->h)) && ((posY + PLAYER_HEIGHT / 2) > (R->y + R->h))) || (((posY - PLAYER_HEIGHT / 2) < (R->y)) && ((posY + PLAYER_HEIGHT / 2) < (R->y))))
		if ((((posPrevY - PLAYER_HEIGHT / 2) > (R->y + R->h)) && ((posPrevY + PLAYER_HEIGHT / 2) > (R->y + R->h))) || (((posPrevY - PLAYER_HEIGHT / 2) < (R->y)) && ((posPrevY + PLAYER_HEIGHT / 2) < (R->y))))
			UD = false;

	//downward check
	bool crossDown = false;
	if (((posY + PLAYER_HEIGHT / 2) > (R->y)) && ((posPrevY + PLAYER_HEIGHT / 2) <= (R->y)))
		crossDown = true;

	//upward check
	bool crossUp = false;
	if (((posY - PLAYER_HEIGHT / 2) < (R->y + R->h)) && ((posPrevY - PLAYER_HEIGHT / 2) >= (R->y + R->h)))
		crossUp = true;

	//leftward check
	bool crossLeft = false;
	if (((posX + PLAYER_WIDTH / 2) > (R->x)) && ((posPrevX + PLAYER_WIDTH / 2) <= (R->x)))
		crossLeft = true;

	//rightward check
	bool crossRight = false;
	if (((posX - PLAYER_WIDTH / 2) < (R->x + R->w)) && ((posPrevX - PLAYER_WIDTH / 2) >= (R->x + R->w)))
		crossRight = true;

	//hug top of the rectangle (walking on top of a platform)
	if (LR && crossDown)
	{
		posY = (R->y) - PLAYER_HEIGHT / 2;
		if (falling && bladeSwing && (blade->getSwingDir() == SOUTH))
			Particle* Pa = new Particle("bulletImpact", posX, R->y, 90);
		falling = false;
		Support = P;
	}

	//hug bottom of the rectangle (headbutting the ceiling)
	if (LR && crossUp)
	{
		posY = (R->y + R->h) + PLAYER_HEIGHT / 2;
		jump = 0;
		speedY = 0;
	}

	//hug left side of the rectangle
	if (UD && crossLeft)
		posX = (R->x) - PLAYER_WIDTH / 2;

	//hug right side of the rectangle
	if (UD && crossRight)
		posX = (R->x + R->w) + PLAYER_WIDTH / 2;

	if ((Support == P) && !LR)
	{
		Support = nullptr;
		falling = true;
	}

}

int Player::GetDirection()
{
	int d = 0;
	if (orientation == SDL_FLIP_NONE)
		d = EAST;
	if (orientation == SDL_FLIP_HORIZONTAL)
		d = WEST;

	return d;
}

bool Player::isFalling()
{
	return falling;
}

float Player::getPosX()
{
	return posX;
}

float Player::getPosY()
{
	return posY;
}

int Player::takeAim()
{
	aim = 0;
	if (keyDownLeft && !keyDownRight)
		aim += WEST;
	if (!keyDownLeft && keyDownRight)
		aim += EAST;
	if (keyDownUp && !crouched)
		aim += NORTH;
	if (!keyDownUp && crouched)
		aim += SOUTH;

	return aim;
}

void Player::takeDamage(float dam)
{
	pain += 15 * dam;
	if (pain > 255)
		pain = 255;
	health -= dam;
}

void Player::bump(float x, float y, float a, int str)
{
	if (!falling)
	{
		falling = true;
		speedY -= str;
		posY -= 1;
		speedX = ((float)cos(a*(M_PI / 180)) * str);
	}
	else
	{
		speedX += ((float)cos(a*(M_PI / 180)))* str;
		speedY += ((float)sin(a*(M_PI / 180)))* str;
	}
	knockback = max(knockback, str*2);
}

int Player::getKnock()
{
	return knockback;
}

void Player::update()
{
	if (weapon != nullptr)
	{
		if (firing && ((bladeBack) <= 0))
			weapon->fire();
		weapon->update();
	}
	if (blade != nullptr)
	{
		if (slashing && (bladeSwing == 0))
		{
			blade->slash();
			bladeSwing = blade->getSwingSpeed();
			bladeBack = blade->getBackSwing();

			if (weapon != nullptr)
			{
				weapon->offhandSwitch(true);
			}
		}
		
		blade->update();
	}

	if (pain > 6)
		pain -= 6;
	else
		pain = 0;

	if (bladeSwing > 0)
		bladeSwing--;
	else
	{
		bladeSwing = 0;


		if (bladeBack > 0)
			bladeBack--;
		else
			bladeBack = 0;

		if (weapon != nullptr)
		{
			weapon->offhandSwitch(false);
		}
	}
}

void Player::render()
{
	units.setColor(255, 255 - pain, 255 - pain);
	units.render(posX - (playerClips[0].w / 2), posY - (playerClips[0].h / 2), &playerClips[0], NULL, NULL, orientation);
}
