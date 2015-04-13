
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

#include "PointEntity.h"

class Weapon;
class Platform;

struct Circle
{
	int x, y;
	int r;
};

class Projectile : public PointEntity
{
public:

	Projectile();
	Projectile(Weapon source);
	~Projectile();

	void onCollision();

	bool isKill();

	void move();

	void render();

	int getRadius();
	int getAngle();

	void terminate();

	int getDamage();

	void handleCollisions(Platform* P);

	void checkCollision(SDL_Rect* b);

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	SDL_RendererFlip flipSide;

	int damage = 0;

	int speed;

	bool killself;

	//Coordinates
	float posPrevX;
	float posPrevY;

	float sourceGunX;
	float sourceGunY;

	int angle;

	Circle col;
	void moveCol();
};