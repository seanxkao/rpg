#pragma once

#include "Body.h"

class Bullet : public Body{
public:

	Bullet(BodyManager*);
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
				setImage(100,100,100,100,0,rad);
				setAlpha(128);
				setBlend(ALPHA_LIGHT);

				if(time==0)	setImgId(2003);
				if(time==1)	setImgId(2001);
				if(time==4)	setImgId(2002);

			}
			else if (state == STATE_HIT){
				setImgId(2000);
				setImage(50,50,50,50,0,time*3);
				setAlpha(255-time*64);
				setBlend(ALPHA_LIGHT);
			}
			else if (state == STATE_DISAPPEAR){
			}
		}
		else if(kind == 100){
			if(state == STATE_CREATE){
				if(time<5){
					setImgId(imgIn);
					setImage(29-time*4,29-time*4,29-time*4,29-time*4,0,0);
					setAlpha(time*51);
				}
			}
			else if (state == STATE_NORMAL){
				setImgId(imgOn);
				setImage(9,9,9,9,0,0);
				setAlpha(255);
			}
			else if (state == STATE_HIT){
				if(time<10){
					setImage(9+time*3,9+time*3,9+time*3,9+time*3,0,time*2.1);
					setAlpha(255-time*25);
					setBlend(Image::ALPHA_LIGHT);
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

Bullet::Bullet(BodyManager *manager) :
	Body(manager){
	imgIn = 4000;
	imgOn = 4000;
	imgOut = 4000;
}
Bullet::~Bullet(){
}


/*



class BulletManager
{
public:
	Bullet bullet[1000];

	BulletManager();
	~BulletManager();

	void clear(){
		for(int i=0;i<1000;i++){
			if(bullet[i].exist){
				bullet[i].setState(Bullet::STATE_HIT);
			}
		}
	}
	

void addBullet(float x,float y,int z,float spd,float r,int wnum, int lnum, float turn,float space,int atk ,int kind)
{
	int a = 0;
	int b = 0;
	for(int i = 0; i < 1000; i++){
		if(!bullet[i].exist ){
			bullet[i].exist = true;
			bullet[i].cleanState();
			bullet[i].setPosition(x, y);
			bullet[i].setSpeed( spd, r+ turn*(wnum-a*2-1)/2, 1);
			bullet[i].setImgId(2000);
			bullet[i].setImage(9,9,9,9,0,0);
			bullet[i].setAlpha(255);
			bullet[i].setBlend(0,1);
			bullet[i].atk=atk;
			bullet[i].kind=kind;
			a++;
			if (a >= wnum){
				a = 0;
				b++;
			}
			if (b >= lnum){
				break;
			}
		}
	}
}




void addBullet(float x,float y,int z,float spd,Object *MS,int wnum, int lnum, float turn,float space,int atk ,int kind)
{	
	float r = vectorR(MS->getX(), MS->getY(), x, y);
	addBullet(x, y, z, spd, r, wnum,  lnum, turn, space, atk , kind);
}

virtual void draw(Drawer *drawer){
	for(int i=0;i<1000;i++){
		if(bullet[i].exist){
			bullet[i].draw(drawer);
		}
	}
}


virtual void main(){
	for(int i=0;i<1000;i++){
		if(bullet[i].exist){
			bullet[i].main();
		}
	}

}

};

BulletManager::BulletManager(){
	for(int i = 0; i < 1000; i++){
		bullet[i].exist = false;
	}
}

BulletManager::~BulletManager(){
}

*/