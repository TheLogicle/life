#include "life.hpp"

#include <vector>

#include <iostream>

void Life::populate(Particle::Type type, int count){
	for(int i = 0; i < count; ++i){
		int x = rand() % width;
		int y = rand() % height;

		particles.emplace_back(type, SDL_FPoint{(float) x, (float) y}, 7);
	}
}

void Life::initForceMatrix(){
	for(int i = 0; i < PARTICLE_TYPE_COUNT; ++i){
		for(int j = 0; j < PARTICLE_TYPE_COUNT; ++j){
			float multiplier = (float) rand() / (float) RAND_MAX;
			forceMatrix[i][j] = (2.0*multiplier - multiplier) * 0.0002;
		}
	}
}

void Life::run(){
	SDL_Event e;
	bool quit = false;
	while(!quit){

		render();
		move();

		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) quit = true;
			/*
			else if(e.type == SDL_WINDOWEVENT){
				SDL_WindowEvent &winEv = *((SDL_WindowEvent*) &e);
				if(winEv.event == SDL_WINDOWEVENT_RESIZED){
					width = winEv.data1;
					height = winEv.data2;
				}
			}
			*/
		}
	}
}

void Life::render(){

	//draw black background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect bgRect{.x=0, .y=0, .w=width, .h=height};
	SDL_RenderFillRect(renderer, &bgRect);

	for(int i = 0; i < particles.size(); ++i){
		particles[i].render(renderer);
	}

	SDL_RenderPresent(renderer);
}

void Life::move(){
	for(int i = 0; i < particles.size(); ++i){
		Particle &p = particles[i];

		for(int j = 0; j < particles.size(); ++j){
			if(i == j) continue;

			Particle &p1 = particles[j];

			//the relative coordinates of the j particle in relation to the i particle

			//float x = p1.pos.x - p.pos.x + (float) width/2;
			//float y = p1.pos.y - p.pos.y + (float) height/2;

			float x = (p1.pos.x + p1.velX) - (p.pos.x + p.velX) + (float) width/2;
			float y = (p1.pos.y + p1.velY) - (p.pos.y + p.velY) + (float) height/2;

			if(x < 0) x += width;
			else if(x > width) x -= width;
			if(y < 0) y += height;
			else if(y > height) y -= height;

			x -= (float) width/2;
			y -= (float) height/2;

			#define PI (3.14159265358979)

			float dist = sqrt(x*x + y*y);
			float angle = 0;
			if(x == 0 && y > 0) angle = PI/2;
			else if(x == 0 && y < 0) angle = PI*3/2;
			else if(x > 0) angle = atan(y/x);
			else if(x < 0) angle = atan(y/x) + PI;

			/*
			#define REPEL_THRESHOLD 500
			if(dist < REPEL_THRESHOLD){
				//float force = (float) 0.015*(dist/THRESHOLD) - 0.015;
				float force = -2.0/dist;

				p.velX += force * cos(angle);
				p.velY += force * sin(angle);
			}
			*/

			#define ATTRACT_LOW_THRESHOLD 25.0
			#define ATTRACT_PEAK 112.5
			#define ATTRACT_UP_THRESHOLD 200.0
			{
				float force = 0;
				float multiplier = forceMatrix[p.type][p1.type];
				if(dist < ATTRACT_LOW_THRESHOLD){
					force = 0.0077 * (-1.0 + dist/ATTRACT_LOW_THRESHOLD);
				}
				else if(dist < ATTRACT_PEAK){
					force = multiplier * (-1.0 + dist/ATTRACT_LOW_THRESHOLD);
				}
				else if(dist < ATTRACT_UP_THRESHOLD){
					force = multiplier * ((ATTRACT_UP_THRESHOLD-dist)/(ATTRACT_UP_THRESHOLD-ATTRACT_PEAK));
				}

				#define FRICTION (0.000001)
				float speed = sqrt(p.velX*p.velX + p.velY*p.velY);
				if(speed <= FRICTION){
					p.velX = 0;
					p.velY = 0;
				}
				else{
					float motionAngle = 0;
					if(p.velX == 0 && p.velY > 0) motionAngle = PI/2;
					else if(p.velX == 0 && p.velY < 0) motionAngle = PI*3/2;
					else if(p.velX > 0) motionAngle = atan(p.velY/p.velX);
					else if(p.velX < 0) motionAngle = atan(p.velY/p.velX) + PI;

					p.velX -= FRICTION * cos(motionAngle);
					p.velY -= FRICTION * sin(motionAngle);

				}

				p.velX += force * cos(angle);
				p.velY += force * sin(angle);
			}
		}

		p.pos.x += p.velX;
		p.pos.y += p.velY;

		for(int j = 0; j < p.points.size(); ++j){
			p.points[j].position.x += p.velX;
			p.points[j].position.y += p.velY;

			if(p.pos.x < 0) p.points[j].position.x += width;
			else if(p.pos.x > width) p.points[j].position.x -= width;
			if(p.pos.y < 0) p.points[j].position.y += height;
			else if(p.pos.y > height) p.points[j].position.y -= height;
		}


		if(p.pos.x < 0) p.pos.x += width;
		else if(p.pos.x > width) p.pos.x -= width;
		if(p.pos.y < 0) p.pos.y += height;
		else if(p.pos.y > height) p.pos.y -= height;

		//p.recomputeVerticies();

	}

	//prevent the system from slowly gaining momentum for some reason
	float avgVelX = 0;
	float avgVelY = 0;
	for(int i = 0; i < particles.size(); ++i){
		avgVelX += particles[i].velX;
		avgVelY += particles[i].velY;
	}

	avgVelX /= particles.size();
	avgVelY /= particles.size();

	for(int i = 0; i < particles.size(); ++i){
		particles[i].velX -= avgVelX;
		particles[i].velY -= avgVelY;
	}
}
