#pragma once

#include "global.h"

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
	

void addBullet(float x, float y, int z, float spd, float r, int wnum, int lnum, float turn, float space, int atk, int kind)
{
	int a = 0;
	int b = 0;
	for(int i = 0; i < 1000; i++){
		if(!bullet[i].exist){
			bullet[i].exist = true;
			bullet[i].clean();
			bullet[i].setPosition(x, y);
			bullet[i].setSpeed(spd, r+turn*(wnum-a*2-1)/2, 1);
			bullet[i].setImgId(2000);
			bullet[i].setImage(9,9,9,9,0,0);
			bullet[i].setAlpha(255);
			bullet[i].setBlendMode(ALPHA_NORMAL);
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

void main(Drawer *drawer){
	for(int i=0;i<1000;i++){
		if(bullet[i].exist){
			bullet[i].main(drawer);
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

