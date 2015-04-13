#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Player.h"
#include "Platform.h"

#include "Camera.h"

using namespace std;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

Camera::Camera()
{
}

Camera::Camera(Player* P)
{
	focus = P;
}

Camera::~Camera()
{

}

bool Camera::willDrawPoint(PointEntity* P)
{
	return(!(((P->getPosX() + P->getRadius()) < (getCameraX() - (SCREEN_WIDTH / 2))) || ((P->getPosX() - P->getRadius()) > (getCameraX() + (SCREEN_WIDTH / 2))) || ((P->getPosY() + P->getRadius()) < (getCameraY() - (SCREEN_HEIGHT / 2))) || ((P->getPosY() - P->getRadius()) > (getCameraY() + (SCREEN_WIDTH / 2)))));
}

bool Camera::willDrawPlatform(Platform* P)
{
	return(!((P->getRekt()->x > (getCameraX() + (SCREEN_WIDTH / 2))) || ((P->getRekt()->x + P->getRekt()->w) < (getCameraX() - (SCREEN_WIDTH / 2))) || (P->getRekt()->y > (getCameraY() + (SCREEN_HEIGHT / 2))) || ((P->getRekt()->y + P->getRekt()->h) < (getCameraY() - (SCREEN_HEIGHT / 2)))));
}

int Camera::getCameraX()
{
	return (posX + offsetX);
}

int Camera::getCameraY()
{
	return (posY + offsetY);
}

Player* Camera::getFocus()
{
	return focus;
}

void Camera::update()
{
	posX = focus->getPosX();
	posY = focus->getPosY();
}