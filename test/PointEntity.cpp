#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "PointEntity.h"

using namespace std;

PointEntity::PointEntity()
{

}

PointEntity::~PointEntity()
{

}

float PointEntity::getPosX()
{
	return posX;
}

float PointEntity::getPosY()
{
	return posY;
}

int PointEntity::getRadius()
{
	return radius;
}