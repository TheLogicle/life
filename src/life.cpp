#include "life.hpp"

#include <chrono>
#include <vector>

void Life::populate(int count){
	for(int i = 0; i < count; ++i){
		float x = (float) rand() / RAND_MAX * width;
		float y = (float) rand() / RAND_MAX * height;

		int type = (int) ((float) rand() / RAND_MAX * PARTICLE_TYPE_COUNT);

		particles.emplace_back(type, SDL_FPoint{(float) x, (float) y}, 5, scale);
	}
}

#define FORCE_MULTIPLIER 0.0007
void Life::initForceMatrix(){
	for(int i = 0; i < PARTICLE_TYPE_COUNT; ++i){
		for(int j = 0; j < PARTICLE_TYPE_COUNT; ++j){
			float forceVal = (float) rand() / (float) RAND_MAX;
			forceMatrix[i][j] = (2.0*forceVal - 1.0) * FORCE_MULTIPLIER;
		}
	}
}

void Life::run(){
	SDL_Event e;
	bool quit = false;
	while(!quit){

		render();
		move();

		++frameCount;

		if(frameCount >= 50){
			frameCount = 0;

			const auto p1 = std::chrono::system_clock::now();
			uint32_t seconds = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();

			#define RESET_INTERVAL_SECONDS 600

			if(seconds - startTime >= RESET_INTERVAL_SECONDS){
				startTime = seconds;

				Particle::resetTypeColors();

				for(int i = 0; i < particles.size(); ++i){
					Particle &p = particles[i];

					float x = (float) rand() / RAND_MAX * width;
					float y = (float) rand() / RAND_MAX * height;
					p.pos = SDL_FPoint{x, y};

					p.velX = 0;
					p.velY = 0;

					p.prevVelX = 0;
					p.prevVelY = 0;

					p.type = (int) ((float) rand() / RAND_MAX * PARTICLE_TYPE_COUNT);

					p.recomputeVerticies(scale);
				}

				initForceMatrix(); //reset force fields
			}
		}

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
	SDL_Rect bgRect{.x=0, .y=0, .w=(int) (width*scale)+5, .h=(int) (height*scale)+5};
	SDL_RenderFillRect(renderer, &bgRect);

	for(int i = 0; i < particles.size(); ++i){
		particles[i].render(renderer);
	}

	SDL_RenderPresent(renderer);
}
