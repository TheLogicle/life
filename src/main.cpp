#include <iostream>

#include "life.hpp"

int main(){
	srand(time(NULL));

	Life life(3000, 2000, 0.5, "Life");

	#define COUNT 110
	life.populate(Particle::PARTICLE_BLUE, COUNT);
	life.populate(Particle::PARTICLE_RED, COUNT);
	life.populate(Particle::PARTICLE_GREEN, COUNT);
	life.populate(Particle::PARTICLE_YELLOW, COUNT);
	life.populate(Particle::PARTICLE_VIOLET, COUNT);
	life.populate(Particle::PARTICLE_CYAN, COUNT);
	life.populate(Particle::PARTICLE_ORANGE, COUNT);

	life.run();

	return 0;
}
