#pragma once

#include "Image.h"


class Particle : public Image{
public:
	Particle();
	virtual ~Particle();

	void setExist(bool exist){
		this->exist = exist;
	}
	
	bool isExist(){
		return exist;
	}
	void setParticle(int kind, int life){
		this->kind = kind;
		this->life = life;
	}

	virtual void draw(Drawer *drawer){
		if(kind==0){
			if(state==0){	
				imgId = 800;
				float rate = (float)time/life;

				if(time<life){
					setARGB(255* (1-rate),172,200,255);
					float si = 2+10*rate;
					setImage(si,si,si,si,0);
					setBlendMode(ALPHA_LIGHT);
				}
				//fire
				/*
				
				if(time<life){
					setARGB(255* (1-rate),255,255*(1-rate) ,25 + 25*(1-rate) );
					float si = 60-40*rate;

					setImage(si,si,si,si,0);
					setBlendMode(ALPHA_LIGHT);
				}
				*/

			}
			else if (state==99){
			}
		}
		else if(kind == 1){
			if(state==0){
				imgId = 810;
				float rate = (float)time/life;

				if(time<life){
					setARGB(255* (1-rate),255,255,255);
					float si = 50 + 100*rate;
					setImage(si,si,si,si,0);
					setBlendMode(ALPHA_LIGHT);
				}
			}
			else if (state==99){
			}
		}


		Image::draw(drawer);
	}

	virtual void mainProc(){
		if(kind==0){
			if(state==0){
				if(time>=life){
					setState(99);
				}

				/*
					img = 800;
				if(time< life/2){
					setARGB(255*(float)time/life*2,180,180,255);
					float si = 10+40*(float)time/life;

					setImage(si,si,si,si,0);
					setBlendMode(ALPHA_LIGHT);
				}
				else if(time<life){
					setARGB(255 - 255*((float)time-life/2)/(life/2),180,180,255);
					float si = 10+40*(float)time/life;

					setImage(si,si,si,si,0);
					setBlendMode(ALPHA_LIGHT);
				}
				else{
					setState(99);
				}
				*/
				
			}
			else if (state==99){
				offFlag(RUNNABLE | DRAWABLE);
				exist = false;
			}
		}
		else if(kind == 1){
			if(state==0){
				if(time == 0){
					rad = random(180, 360);
				}
				if(time>=life){
					setState(99);
				}
			}
			else if (state==99){
				exist=false;
				offFlag(RUNNABLE | DRAWABLE);
			}
		}
	}


protected:
	int	life;
	int kind;
	bool exist;
};

Particle::Particle(): Image(){
	setZ(0.7);
}

Particle::~Particle(){
}
