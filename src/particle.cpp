#include "life.hpp"

#include <cmath>
#include <vector>
#include <SDL2/SDL.h>

#define PI (3.14159265358979)

Particle::Particle(Particle::Type type, SDL_FPoint pos, float radius){
	this->pos = pos;
	this->radius = radius;
	this->type = type;
	color = to_color(type);

	//TEMP
	//velX = ((float) rand()-((float) RAND_MAX/2)) / RAND_MAX / 5;
	//velY = ((float) rand()-((float) RAND_MAX/2)) / RAND_MAX / 5;
	velX = 0;
	velY = 0;


	recomputeVerticies();
}

#define ITER 40

int Particle::indices[ITER*3];
bool Particle::indicesSet = false;

void Particle::recomputeVerticies(){

	if(!indicesSet){
		indicesSet = true;

		for(int i = 0; i < ITER; ++i){
			indices[i*3] = 0;
			indices[i*3+1] = i+1;
			indices[i*3+2] = i+2;
		}

		indices[ITER*3-1] = 1;
	}

	points = std::vector<SDL_Vertex>();

	points.push_back(SDL_Vertex{pos, color});

	//compute verticies to draw the circle
	for(int i = 0; i < ITER; ++i){
		float x = pos.x + radius * cos(2*PI/ITER * i);
		float y = pos.y + radius * sin(2*PI/ITER * i);

		points.push_back(SDL_Vertex{SDL_FPoint{x, y}, color});
	}
}

void Particle::render(SDL_Renderer* renderer){
	SDL_RenderGeometry(renderer, NULL, &(points[0]), points.size(), indices, ITER*3);
}
