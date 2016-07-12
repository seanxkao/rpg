#pragma once

#include"Particle.h"

class Pool{
public:

	Pool(int);
	virtual ~Pool();

	int	size;
	Particle *particle;

	void addParticle(int life, int kind, float x, float y, float mx, float my, float ax, float ay){
		for(int i = 0; i < size; i++){
			if(!particle[i].alife()){
				particle[i].setExist(true);
				particle[i].init();
				particle[i].setParticle(kind, life);
				particle[i].setPosition(x, y);
				particle[i].setSpeed(mx, my);
				particle[i].setAccel(ax, ay);
				break;
			}
		}
	}
	void draw(Drawer *drawer){
		for(int i = 0;i < size;i++){
			if(particle[i].alife()){
				particle[i].draw(drawer);
			}
		}
	}

	void main(){
		for(int i = 0;i < size;i++){
			if(particle[i].alife()){
				particle[i].main();
			}
		}
	}

};

Pool::Pool(int size){
	this->size = size;
	particle = new Particle[size];
	for(int i = 0; i < size; i++){
		particle[i].setExist(false);
		particle[i].init();
	}
}

Pool::~Pool(){
	delete []particle;
}
