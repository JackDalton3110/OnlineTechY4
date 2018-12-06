#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <SDL_image.h>
#include <stdio.h>
#include "Client.h"
#include "Dot.h"
#include "LTexture.h"
#include "TCPListener.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

bool init();
bool loadMedia();
void close();

SDL_Window* m_window = NULL;
SDL_Surface* m_ScreenSurface = NULL;
SDL_Surface* m_ImageSurface = NULL;
SDL_Surface* loadSurface(std::string location);
SDL_Renderer* gRenderer;
Dot* dot;
Dot* chaseDot;
Client* client;



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		m_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	/*if (!gDotTexture.loadFromFile("dot.bmp", gRenderer))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}*/

	return success;
}

void close()
{
	//Free loaded images
	//gDotTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(m_window);
	m_window = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{

	dot = new Dot(false);
	chaseDot = new Dot(true);
	client = new Client();
	client->run();

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

			//The dot that will be moving around on the screen
			dot->SetPosition(100, 300);
			dot->Init(gRenderer);
			chaseDot->Init(gRenderer);
			

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle input for the dot
					dot->handleEvent(e);
					chaseDot->handleEvent(e);
					
				}

				//Move the dot
				dot->move(SCREEN_WIDTH, SCREEN_HEIGHT);
				chaseDot->move(SCREEN_WIDTH, SCREEN_HEIGHT);
				client->recieve();

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render objects
				dot->render(gRenderer);
				chaseDot->render(gRenderer);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}