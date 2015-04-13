
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>

#include "PointEntity.h"

class Player;
class Projectile;

class Weapon : public PointEntity
{
public:

	Weapon();
	Weapon(Player* holder);
	~Weapon();

	void fire();

	void move();

	void render();

	float getBarrelPos();
	float getPosY();

	float getAngle();
	float getSide();
	void offhandSwitch(bool state);

	void update();

	int getAmmo();

	std::string getType();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//he who holds the gun
	Player* gunner;

	std::string gunType = "";

	//projectile type
	std::string projectileType = "";

	//ammo left
	int ammo = 0;

	//delay
	int fireDelay;

	//Coordinates

	float angle;
	int targetAngle;

	SDL_RendererFlip flipSide;
	int xOffset;
	int yOffset;
	int xTargetOffset;
	int yTargetOffset;
	int recoil;

	SDL_Point center;

	int fireState;

	
	bool offhand = false;
};