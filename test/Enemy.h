#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Living.h"

using namespace std;

class Player;
class Platform;
class Projectile;

class Enemy : public Living
{
public:

	Enemy();
	Enemy(int x, int y);

	~Enemy();

	void handleCollisions(Platform* P);

	void handleCollisions(Projectile* P);

	void handleCollisions(Player* P);

	void update();

	void render();

	void takeDamage(float dam);

	void bump(float x, float y, float a, int str);

	float getFacing();

	bool getInvuln(int type);
	void setInvuln(int type, int length);

private:

	SDL_RendererFlip orientation = SDL_FLIP_NONE;

	const int ENEMY_MAX_VEL = 3;
	const int ENEMY_TERMINAL_VEL = 16;

	Platform* Support;

	void move();

	int posPrevX;
	int posPrevY;

	float walkspeed = 3;

	bool dash = false;

	float speedX = 0;
	float speedY = 0;

	int invulnBlade = 0;

	float facing = 1;
	float updateFacing();

	int height = 32;
	int width = 32;

	int pain = 0;

	int damage = 0;

	int attackState = 0;
	int attackCooldown = 40;
	int cooldown = 0;

	bool gravity = false;
	bool falling = true;

	bool moving = false;
};