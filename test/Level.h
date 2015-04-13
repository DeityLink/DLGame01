#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

class Platform;

using namespace std;

class Level
{
public:
	Level();

	~Level();

	void initLevel();

	vector< int > getSpawnPoint();

private:
	int pSpawn[2];

};