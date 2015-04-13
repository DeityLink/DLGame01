
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

class Platform
{
public:

	Platform();
	Platform(int X, int Y, int W, int H);

	~Platform();

	SDL_Rect* getRekt();

	void render();

private:

	SDL_Rect* fillRect;

	int x;
	int y;
	int w;
	int h;

};