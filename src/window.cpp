#include "life.hpp"

#include <string>
#include <ctime>

#ifdef NULL
#undef NULL
#define NULL 0
#endif

#include <SDL2/SDL.h>

Life::Life(int width, int height, std::string title){
	this->width = width;
	this->height = height;

	if(windowCount == 0){
		if(SDL_Init(SDL_INIT_VIDEO) < 0){
			throw error::SDL_init();
		}
	}

	//Create a window
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if(window == NULL){
		throw error::SDL_create_window();
	}

	++windowCount;

	//Create a renderer for the window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){
		throw error::SDL_create_renderer();
	}

	initForceMatrix();
}

Life::~Life(){
	SDL_DestroyWindow(window);
	--windowCount;

	if(windowCount == 0) SDL_Quit();
}
