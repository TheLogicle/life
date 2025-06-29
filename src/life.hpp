#ifndef LIFE_HPP
#define LIFE_HPP

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#define PARTICLE_TYPE_COUNT 15

class Particle{
	friend class Life;

	public:
		Particle(int type, SDL_FPoint pos, float radius, float scale);

		void render(SDL_Renderer* renderer);

		static void resetTypeColors();
		static SDL_Color getTypeColor(int type);

	private:
		static std::vector<SDL_Color> typeColors;

		static int indices[];
		static bool indicesSet;

		std::vector<SDL_Vertex> points;

		void recomputeVerticies(float scale);

		SDL_FPoint pos;
		float radius;

		int type;

		float velX;
		float velY;

		float prevVelX;
		float prevVelY;

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

		uint32_t frameCount;
		uint32_t startTime; //in seconds

		std::vector<Particle> particles;

		//first index is self-type, second index is other-type
		//this stores the forces imposed on a particular type of particle by another type
		float forceMatrix[PARTICLE_TYPE_COUNT][PARTICLE_TYPE_COUNT];
		void initForceMatrix();

	public:
		Life(int width, int height, float scale, std::string title);
		~Life();

		void run();
		void populate(int count);
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
