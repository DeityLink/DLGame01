
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "Living.h"

class Weapon;
class Blade;
class Platform;

class Player : public Living
{
public:
	static const int PLAYER_WIDTH = 32;
	static const int PLAYER_HEIGHT = 32;
	static const int PLAYER_TERMINAL_VEL = 16;

	Player();
	Player(int x, int y);
	~Player();

	Weapon* weapon;
	Blade* blade;

	void handleEvent(SDL_Event& e);

	void handleCollisions(Platform* P);

	void move();

	bool isFalling();

	void render();

	int GetDirection();

	void takeDamage(float dam);

	void bump(float x, float y, float a, int str);

	void update();

	int takeAim();

	float getPosX();
	float getPosY();

	int getKnock();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	Platform* Support;

	//Coordinates
	int posPrevX;
	int posPrevY;

	float speedX;
	float speedX_Right;
	float speedX_Left;
	float speedY;

	float maxSpeed = 5;

	int aim;

	int pain = 0;

	SDL_RendererFlip orientation;

	bool keyDownLeft;
	bool keyDownRight;
	bool keyDownUp;

	bool crouched;

	bool firing;
	bool slashing;

	bool falling;

	bool freeze;

	int jump;

	float bladeSwing = 0;
	float bladeBack = 0;
};