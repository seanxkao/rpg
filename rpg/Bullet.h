#pragma once

#include "Body.h"

class Bullet : public Body{
public:

	Bullet();
	~Bullet();

	static const int STATE_CREATE = 0;
	static const int STATE_NORMAL = 1;
	static const int STATE_HIT = 2;
	static const int STATE_DISAPPEAR = 99;

	int kind;
	int atk;
	bool exist;

	int imgIn;
	int imgOn;
	int imgOut;


	virtual void draw(Drawer *drawer){
		if(kind == 0){
			if(state == STATE_CREATE){
			}
			else if (state == STATE_NORMAL){
				setImage(100, 100, 100, 100, rad);
				setAlpha(128);
				setBlendMode(ALPHA_LIGHT);

				if(time==0)	setImgId(2003);
				if(time==1)	setImgId(2001);
				if(time==4)	setImgId(2002);

			}
			else if (state == STATE_HIT){
				setImgId(2000);
				setImage(50,50,50,50,time*3);
				setAlpha(255-time*64);
				setBlendMode(ALPHA_LIGHT);
			}
			else if (state == STATE_DISAPPEAR){
			}
		}
		else if(kind == 100){
			if(state == STATE_CREATE){
				if(time<5){
					setImgId(imgIn);
					setImage(29-time*4,29-time*4,29-time*4,29-time*4,0);
					setAlpha(time*51);
				}
			}
			else if (state == STATE_NORMAL){
				setImgId(imgOn);
				setImage(9,9,9,9,0);
				setAlpha(255);
			}
			else if (state == STATE_HIT){
				if(time<10){
					setImage(9+time*3,9+time*3,9+time*3,9+time*3,time*2.1);
					setAlpha(255-time*25);
					setBlendMode(Image::ALPHA_LIGHT);
				}
			}
			else if (state == STATE_DISAPPEAR){
			}
		}

		Body::draw(drawer);
	}

protected:
	virtual void mainProc(){
		if(kind == 0){
			if(state == STATE_CREATE){
				setState(STATE_NORMAL);
			}
			else if (state == STATE_NORMAL){
				setSpeed(speed, rad, 0);
				setAttack(3, 0, 35, 100, rad+45, rad+135);
				if(time>4) setState(STATE_DISAPPEAR);
				if(isOut()) setState(STATE_HIT);
			}
			else if (state == STATE_HIT){
				if(time <= 4){
					rate = 0;
				}
				else {
					setState(STATE_DISAPPEAR);
				}
			}
			else if (state == STATE_DISAPPEAR){
				exist = false;
				time = 0;
			}
		}
		else if(kind == 100){
			if(state == STATE_CREATE){
				if(time>=5){
					setState(STATE_NORMAL);
				}
			}
			else if (state == STATE_NORMAL){
				//setAttack(6);

				if(isOut()) setState(STATE_HIT);
				
			}
			else if (state == STATE_HIT){
				setImgId(imgOut);
				if( time<=10 ){
					rate=0;
				}
				else {
					setState(STATE_DISAPPEAR);
				}
			}
			else if (state == STATE_DISAPPEAR){
				exist = false;
				time = 0;
			}
		}
		Body::mainProc();
	}
};

Bullet::Bullet():Body(){
	imgIn = 4000;
	imgOn = 4000;
	imgOut = 4000;
}
Bullet::~Bullet(){
}
