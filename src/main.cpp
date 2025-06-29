#include <iostream>

#include "life.hpp"

int main(){
	srand(time(NULL));

	try{
		Life life(3000, 2000, 0.5, "Life");

		#define COUNT 1200
		life.populate(COUNT);

		life.run();
	}
	catch(error::life_error e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
