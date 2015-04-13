#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "PointEntity.h"

class Player;
class Enemy;

using namespace std;

class Blade : public PointEntity
{
public:

	Blade();
	Blade(Player* holder);

	void handleCollisions(Enemy* P);

	~Blade();

	void slash();

	float getSwingSpeed();
	float getBackSwing();
	int getSwingDir();

	void update();

	void render();

private:

	int bladeType;

	SDL_RendererFlip flipSide;
	int xOffset;
	int yOffset;
	int xTargetOffset;
	int yTargetOffset;

	SDL_Point center;

	Player* slasher;

	float angle;
	float targetAngle;

	float getSide();

	float calculateDamage(Enemy* E);
	float calculateKnockback();

	int baseDamage = 0;

	float swing;
	float swingSpeed;
	float backswing = 0;
	int swingDir = EAST;

	float bladeLength;
	float bladeRadius;

	float getSwingPhase();

	void attackUp();
	void attackDown();
	void attackHorizontal();
	void attackHorizontalUp();
	void attackHorizontalDown();
};