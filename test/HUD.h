#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

class HUD
{
public:
	HUD();

	~HUD();

	void update();

	void render();

private:

	int lives = 0;
	int ammo = 0;
	std::string gun = "";
};