#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

#include "Platform.h"

#include "Level.h"

using namespace std;

extern vector<Platform*> platforms;
extern bool compareStrings(string A, string B);

Level::Level()
{
	initLevel();
}

Level::~Level()
{
	//Deallocate
}

void Level::initLevel()
{
	SDL_RWops* file = SDL_RWFromFile("Level1.txt", "r");

	if (file == NULL)
	{
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		int lineNum = 0;
		vector<string> levelData;
		ifstream levelFile ("Level1.txt");
		string line;
		while (getline(levelFile, line))
		{
			++lineNum;
			levelData.push_back(line);
		}

		if (levelData.size() == 0)
		{
			printf("Warning: File is empty! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			string section = "test";

			string partA = "*spawn";
			string partB = "*platforms";

			for (int n = 0; n < (int)levelData.size(); n++)
			{
				if (levelData[n].at(0) == '*')
				{
					section = levelData[n].c_str();
				}
				else
				{
					if (compareStrings(section, partA))
					{
						string spawnX = "";
						string spawnY = "";
						int i = 0;
						while (levelData[n].at(i) != ',')
						{
							spawnX += levelData[n].at(i);
							i++;
						}
						i++;
						while (levelData[n].at(i) != ';')
						{
							spawnY += levelData[n].at(i);
							i++;
						}
						pSpawn[0] = stoi(spawnX);
						pSpawn[1] = stoi(spawnY);
					}
					else if (compareStrings(section, partB))
					{
						string platX = "";
						string platY = "";
						string platW = "";
						string platH = "";
						int i = 0;
						while (levelData[n].at(i) != ',')
						{
							platX += levelData[n].at(i);
							i++;
						}
						i++;
						while (levelData[n].at(i) != ',')
						{
							platY += levelData[n].at(i);
							i++;
						}
						i++;
						while (levelData[n].at(i) != ',')
						{
							platW += levelData[n].at(i);
							i++;
						}
						i++;
						while (levelData[n].at(i) != ';')
						{
							platH += levelData[n].at(i);
							i++;
						}
						platforms.push_back(new Platform(stoi(platX), stoi(platY), stoi(platW), stoi(platH)));
					}
				}
			}
		}
	}

}

vector< int > Level::getSpawnPoint()
{
	vector<int> vector;
	vector.push_back(pSpawn[0]);
	vector.push_back(pSpawn[1]);
	return vector;
}