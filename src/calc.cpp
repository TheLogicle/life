#include "life.hpp"

#include <vector>

void Life::move(){
	for(int i = 0; i < particles.size(); ++i){
		Particle &p = particles[i];
		p.prevVelX = p.velX;
		p.prevVelY = p.velY;
	}

	for(int i = 0; i < particles.size(); ++i){
		Particle &p = particles[i];

		for(int j = 0; j < particles.size(); ++j){
			if(i == j) continue;

			Particle &p1 = particles[j];

			//the relative coordinates of the j particle in relation to the i particle

			//float x = p1.pos.x - p.pos.x + (float) width/2;
			//float y = p1.pos.y - p.pos.y + (float) height/2;

			float x = (p1.pos.x + p1.prevVelX) - (p.pos.x + p.prevVelX) + (float) width/2;
			float y = (p1.pos.y + p1.prevVelY) - (p.pos.y + p.prevVelY) + (float) height/2;

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

			#define ATTRACT_LOW_THRESHOLD (24.0)
			#define ATTRACT_UP_THRESHOLD (500.0)
			#define ATTRACT_PEAK (ATTRACT_LOW_THRESHOLD+ATTRACT_UP_THRESHOLD)/2.0

			#define REPEL_FORCE 0.1
			{
				float force = 0;
				float multiplier = forceMatrix[p.type][p1.type];
				if(dist < ATTRACT_LOW_THRESHOLD){
					force = REPEL_FORCE * (-1.0 + dist/ATTRACT_LOW_THRESHOLD);
				}
				else if(dist < ATTRACT_PEAK){
					force = multiplier * (-1.0 + dist/ATTRACT_LOW_THRESHOLD);
				}
				else if(dist < ATTRACT_UP_THRESHOLD){
					force = multiplier * ((ATTRACT_UP_THRESHOLD-dist)/(ATTRACT_UP_THRESHOLD-ATTRACT_PEAK));
				}

				p.velX += force * cos(angle);
				p.velY += force * sin(angle);
			}
		}


		#define FRICTION (0.002)
		#define SUPER_FRICTION (0.1)
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

			if(speed < 3.0){
				p.velX -= FRICTION * cos(motionAngle);
				p.velY -= FRICTION * sin(motionAngle);
			}
			else{
				p.velX -= SUPER_FRICTION * cos(motionAngle);
				p.velY -= SUPER_FRICTION * sin(motionAngle);
			}

		}

		//accelerate
		p.pos.x += p.velX;
		p.pos.y += p.velY;

		//move each vertex of the circle
		for(int j = 0; j < p.points.size(); ++j){
			p.points[j].position.x += p.velX*scale;
			p.points[j].position.y += p.velY*scale;

			//wrap around sides
			if(p.pos.x < 0) p.points[j].position.x += width*scale;
			else if(p.pos.x > width) p.points[j].position.x -= width*scale;
			if(p.pos.y < 0) p.points[j].position.y += height*scale;
			else if(p.pos.y > height) p.points[j].position.y -= height*scale;
		}


		//wrap around sides
		if(p.pos.x < 0) p.pos.x += width;
		else if(p.pos.x > width) p.pos.x -= width;
		if(p.pos.y < 0) p.pos.y += height;
		else if(p.pos.y > height) p.pos.y -= height;

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
