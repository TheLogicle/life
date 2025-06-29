#include <iostream>

#include "life.hpp"

int main(){
	srand(time(NULL));

	Life life(1000, 1000, "Life");

	life.populate(Particle::PARTICLE_BLUE, 70);
	life.populate(Particle::PARTICLE_RED, 70);
	life.populate(Particle::PARTICLE_GREEN, 70);

	life.run();

	return 0;
}
