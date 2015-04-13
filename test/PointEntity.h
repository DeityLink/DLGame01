#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using namespace std;

#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 8
#define NORTHEAST 5
#define NORTHWEST 9
#define SOUTHEAST 6
#define SOUTHWEST 10

#ifndef POINTENTITY_H
#define POINTENTITY_H
class PointEntity
{
public:

	PointEntity();

	~PointEntity();

	void update();

	void render();

	float getPosX();
	float getPosY();
	int getRadius();

protected:
	float posX;
	float posY;
	int radius = 0;
};
#endif /* POINTENTITY_H */