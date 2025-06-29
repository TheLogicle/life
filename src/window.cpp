#include "life.hpp"

#include <string>
#include <chrono>
#include <system_error>

#ifdef NULL
#undef NULL
#define NULL 0
#endif

#include <SDL2/SDL.h>

Life::Life(int width, int height, float scale, std::string title){
	this->width = width;
	this->height = height;
	this->scale = scale;

	frameCount = 0;

	const auto p1 = std::chrono::system_clock::now();
	startTime = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

	if(windowCount == 0){
		if(SDL_Init(SDL_INIT_VIDEO) < 0){
			throw error::SDL_init();
		}
	}

	//Create a window
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, SDL_WINDOW_SHOWN);
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
