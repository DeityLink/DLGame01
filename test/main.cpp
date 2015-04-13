// Project started on 01/01/2015 by Deity Link
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

#include "PointEntity.h"
#include "LTexture.h"
#include "LTimer.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Blade.h"
#include "Level.h"
#include "Platform.h"
#include "Particle.h"
#include "Healthbar.h"
#include "HUD.h"

using namespace std;

int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 512;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

SDL_Texture* loadTexture(string path);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;

stringstream angleText;

//Scene Textures
LTexture background;
LTexture units;
LTexture weapons;
LTexture blades;
LTexture particles_sheet;

//Scene sprites
SDL_Rect playerClips[1];
SDL_Rect enemyClips[3];
SDL_Rect gunClips[3];
SDL_Rect bladeClips[1];
SDL_Rect bulletClips[1];
SDL_Rect particleClips[5];

//HUD Textures
LTexture HUDshell;
LTexture HUDbars;
LTexture numbers;

//HUD sprites
SDL_Rect HUDshellClips[1];
SDL_Rect HUDbarClips[2];
SDL_Rect numberClips[11];

Level* stage;
Player* player;
Camera* camera;
Weapon* gun;
Blade* knife;
HUD* hud;

vector<Projectile*> projectiles;
vector<Particle*> particles;
vector<Platform*> platforms;
vector<Enemy*> enemies;
vector<Healthbar*> healthbars;

bool init();

bool loadMedia();

void close();

bool compareStrings(string A, string B);

double distanceSquared(int x1, int y1, int x2, int y2);


bool init()
{
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Test Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//create window renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	
	return success;
}

bool loadMedia()
{
	bool success = true;

	gFont = TTF_OpenFont("lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if (!gTextTexture.loadFromRenderedText("bla", textColor))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	if (!background.loadFromFile("bg.png"))
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}

	if (!units.loadFromFile("32x32.png"))
	{
		printf("Failed to load units texture image!\n");
		success = false;
	}
	else
	{
		playerClips[0].x = 0;
		playerClips[0].y = 0;
		playerClips[0].w = 32;
		playerClips[0].h = 32;

		enemyClips[0].x = 0;
		enemyClips[0].y = 32;
		enemyClips[0].w = 32;
		enemyClips[0].h = 32;

		enemyClips[1].x = 32;
		enemyClips[1].y = 32;
		enemyClips[1].w = 32;
		enemyClips[1].h = 32;

		enemyClips[2].x = 64;
		enemyClips[2].y = 32;
		enemyClips[2].w = 32;
		enemyClips[2].h = 32;
	}

	if (!weapons.loadFromFile("weapons.png"))
	{
		printf("Failed to load weapons texture image!\n");
		success = false;
	}
	else
	{
		gunClips[0].x = 0;
		gunClips[0].y = 0;
		gunClips[0].w = 32;
		gunClips[0].h = 32;

		gunClips[1].x = 32;
		gunClips[1].y = 0;
		gunClips[1].w = 32;
		gunClips[1].h = 32;

		gunClips[2].x = 64;
		gunClips[2].y = 0;
		gunClips[2].w = 32;
		gunClips[2].h = 32;

		bulletClips[0].x = 0;
		bulletClips[0].y = 32;
		bulletClips[0].w = 32;
		bulletClips[0].h = 32;
	}

	if (!particles_sheet.loadFromFile("particles.png"))
	{
		printf("Failed to load particles texture image!\n");
		success = false;
	}
	else
	{
		particleClips[0].x = 0;
		particleClips[0].y = 0;
		particleClips[0].w = 32;
		particleClips[0].h = 32;

		particleClips[1].x = 0;
		particleClips[1].y = 32;
		particleClips[1].w = 32;
		particleClips[1].h = 32;

		particleClips[2].x = 0;
		particleClips[2].y = 64;
		particleClips[2].w = 32;
		particleClips[2].h = 32;

		particleClips[3].x = 32;
		particleClips[3].y = 64;
		particleClips[3].w = 32;
		particleClips[3].h = 32;

		particleClips[4].x = 64;
		particleClips[4].y = 64;
		particleClips[4].w = 32;
		particleClips[4].h = 32;
	}

	if (!blades.loadFromFile("blades.png"))
	{
		printf("Failed to load blades texture image!\n");
		success = false;
	}
	else
	{
		bladeClips[0].x = 0;
		bladeClips[0].y = 0;
		bladeClips[0].w = 32;
		bladeClips[0].h = 32;
	}

	if (!HUDshell.loadFromFile("HUD_shell.png"))
	{
		printf("Failed to load HUDshell texture image!\n");
		success = false;
	}
	else
	{
		HUDshellClips[0].x = 0;
		HUDshellClips[0].y = 0;
		HUDshellClips[0].w = 512;
		HUDshellClips[0].h = 512;
	}

	if (!numbers.loadFromFile("numbers.png"))
	{
		printf("Failed to load numbers texture image!\n");
		success = false;
	}
	else
	{
		for (int n = 0; n < 11; n++)
		{
			numberClips[n].x = (n * 18);
			numberClips[n].y = 0;
			numberClips[n].w = 18;
			numberClips[n].h = 28;
		}
	}

	if (!HUDbars.loadFromFile("HUD_bars.png"))
	{
		printf("Failed to load HUDbars texture image!\n");
		success = false;
	}
	else
	{
		for (int n = 0; n < 1; n++)
		{
			HUDbarClips[n].x = 0;
			HUDbarClips[n].y = (24 * n);
			HUDbarClips[n].w = 179;
			HUDbarClips[n].h = 24;
		}
	}

	return success;
}

void close()
{
	//Free loaded image
	background.free();
	units.free();
	weapons.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool compareStrings(string A, string B)
{
	bool same = true;
	int i = A.size();
	int j = B.size();

	if (i != j)
		return false;

	for (int n = 0; n < i; n++)
	{
		if (A.at(n) != B.at(n))
		{
			same = false;
		}
	}
	return same;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

double distPointToLine(float xA, float yA, float xB, float yB, float xP, float yP)
{
	float DelX = xB - xA;
	float DelY = yB - yA;
	double D = sqrt(pow(DelX, 2) + pow(DelY, 2));
	double Ratio = (double)((xP - xA) * DelX +
		(yP - yA) * DelY) / (DelX * DelX + DelY * DelY);
	if (Ratio * (1 - Ratio) < 0)
		return -1;
	return (double)abs(DelX * (yP - yA) - DelY * (xP - xA)) / D;
}

int main(int argc, char** argv)
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			stage = new Level;
			vector<int> Pspawn = stage->getSpawnPoint();
			player = new Player(Pspawn.at(0), Pspawn.at(1));
			camera = new Camera(player);
			gun = new Weapon(player);
			knife = new Blade(player);
			hud = new HUD();

			//The frames per second timer
			LTimer fpsTimer;
			
			//The frames per second cap timer
			LTimer capTimer;
			
			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();

			while (!quit)
			{
				capTimer.start();
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					if ((e.type == SDL_KEYDOWN) && (e.key.keysym.sym == SDLK_ESCAPE))
					{
						quit = true;
					}

					player->handleEvent(e);
				}

				/*
				angleText.str("");
				angleText << "gun angle" << (player->getKnock());
				*/

				if (!gTextTexture.loadFromRenderedText(angleText.str().c_str(), { 0, 0, 0 }))
				{
					printf("Failed to render text texture!\n");
				}

				//Calculate and correct fps
				float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
				if( avgFPS > 2000000 )
				{
					avgFPS = 0;
				}

				//Update particles
				for (int n = 0; n < (int)particles.size(); n++)
				{
					particles[n]->update();
				}

				//Move projectiles
				for (int n = 0; n < (int)projectiles.size(); n++)
				{
					projectiles[n]->move();
				}
				
				//Move Player
				player->move();

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Check for Player-Platform collisions
				for (int n = 0; n < (int)platforms.size(); n++)
				{
					player->handleCollisions(platforms[n]);
				}

				//Update Enemies
				for (int n = 0; n < (int)enemies.size(); n++)
				{
					enemies[n]->update();
				}

				//Update Player
				player->update();

				//Update Healthbars
				for (int n = 0; n < (int)healthbars.size(); n++)
				{
					healthbars[n]->update();
				}

				//Update HUD
				hud->update();

				//Update Camera
				camera->update();

				//============REMOVALS=======================================
				//Remove Projectiles
				for (int n = 0; n < (int)projectiles.size(); n++)
				{
					if (projectiles[n]->isKill())
					{
						delete projectiles[n];
						projectiles.erase(projectiles.begin() + n);
						n--;
					}
				}

				//Remove Particles
				for (int n = 0; n < (int)particles.size(); n++)
				{
					if (particles[n]->isKill())
					{
						delete particles[n];
						particles.erase(particles.begin() + n);
						n--;
					}
				}

				//Remove Enemies
				for (int n = 0; n < (int)enemies.size(); n++)
				{
					if (enemies[n]->isKill())
					{
						delete enemies[n];
						enemies.erase(enemies.begin() + n);
						n--;
					}
				}

				//============RENDERS=======================================
				//Render Background
				background.render(0, 0);

				//Render Platforms
				for (int n = 0; n < (int)platforms.size(); n++)
				{
					if (camera->willDrawPlatform(platforms[n]))
						platforms[n]->render();
				}

				//Render Enemies
				for (int n = 0; n < (int)enemies.size(); n++)
				{
					if (camera->willDrawPoint(enemies[n]))
						enemies[n]->render();
				}

				//Render Player
				player->render();

				gun->render();
				knife->render();

				//Render Particles
				for (int n = 0; n < (int)particles.size(); n++)
				{
					if (camera->willDrawPoint(particles[n]))
						particles[n]->render();
				}

				//Render Projectiles
				for (int n = 0; n < (int)projectiles.size(); n++)
				{
					if (camera->willDrawPoint(projectiles[n]))
						projectiles[n]->render();
				}

				//Render Healthbars
				for (int n = 0; n < (int)healthbars.size(); n++)
				{
					if (camera->willDrawPoint(healthbars[n]->getHolder()))
						healthbars[n]->render();
				}

				//Render HUD
				hud->render();

				gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);

				//Update screen
				SDL_RenderPresent(gRenderer);
				++countedFrames;

				//Capping to 60 FPS
				int frameTicks = capTimer.getTicks();
				if (frameTicks < SCREEN_TICKS_PER_FRAME)
				{
					SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
