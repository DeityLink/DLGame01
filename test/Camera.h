
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>

#include "PointEntity.h"

class Player;
class Platform;

class Camera : public PointEntity
{
public:
	Camera();
	Camera(Player* P);

	~Camera();

	void update();

	int getCameraX();
	int getCameraY();

	bool willDrawPoint(PointEntity* P);
	bool willDrawPlatform(Platform* P);

	Player* getFocus();

private:

	Player* focus;

	int offsetX = 0;
	int offsetY = 0;

};