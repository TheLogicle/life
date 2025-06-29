#ifndef LIFE_HPP
#define LIFE_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#define PARTICLE_TYPE_COUNT 7

class Particle{
	friend class Life;

	public:
		enum Type{
			PARTICLE_BLUE = 0,
			PARTICLE_RED = 1,
			PARTICLE_GREEN = 2,
			PARTICLE_YELLOW = 3,
			PARTICLE_VIOLET = 4,
			PARTICLE_CYAN = 5,
			PARTICLE_ORANGE = 6,
		};
		SDL_Color to_color(Particle::Type type){
			switch(type){
				case PARTICLE_BLUE: return SDL_Color{50, 50, 200, 255};
				case PARTICLE_RED: return SDL_Color{200, 50, 50, 255};
				case PARTICLE_GREEN: return SDL_Color{50, 200, 50, 255};
				case PARTICLE_YELLOW: return SDL_Color{220, 220, 50, 255};
				case PARTICLE_VIOLET: return SDL_Color{109, 18, 214, 255};
				case PARTICLE_CYAN: return SDL_Color{33, 163, 170, 255};
				case PARTICLE_ORANGE: return SDL_Color{208, 102, 27, 255};

				default: return SDL_Color{0, 0, 0, 0};
			}
		}

		Particle(Particle::Type type, SDL_FPoint pos, float radius, float scale);

		void render(SDL_Renderer* renderer);

	private:
		std::vector<SDL_Vertex> points;

		static int indices[];
		static bool indicesSet;

		void recomputeVerticies(float scale);

		SDL_FPoint pos;
		float radius;

		Particle::Type type;
		SDL_Color color;

		float velX;
		float velY;

};

class Life{
	private:
		uint64_t windowCount = 0;

		void render();
		void move();

		int width;
		int height;
		float scale;

		SDL_Window* window;
		SDL_Renderer* renderer;

		std::vector<Particle> particles;

		//first index is self-type, second index is other-type
		//this stores the forces imposed on a particular type of particle by another type
		float forceMatrix[PARTICLE_TYPE_COUNT][PARTICLE_TYPE_COUNT];
		void initForceMatrix();

	public:
		Life(int width, int height, float scale, std::string title);
		~Life();

		void run();
		void populate(Particle::Type type, int count);
};


namespace error{
	struct life_error{
		virtual std::string what(){
			return "Unknown error";
		}
	};

	struct SDL_init: life_error{
		std::string what(){
			return std::string{"SDL could not initialize! SDL_Error: "} + SDL_GetError();
		}
	};
	struct SDL_create_window: life_error{
		std::string what(){
			return std::string{"Window could not be created! SDL_Error: "} + SDL_GetError();
		}
	};
	struct SDL_create_renderer: life_error{
		std::string what(){
			return std::string{"Renderer could not be created! SDL_Error: "} + SDL_GetError();
		}
	};
}

#endif
