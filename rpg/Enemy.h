#pragma once

#include "Bar.h"
#include "Bullet.h"
#include "Image.h"
#include "Maths.h"

class Enemy : public Body{
public:
	Enemy(): Body(){
		setImage(30, 30, 30, 30, 0);
		setImgId(1000);
		setHp(100, 100);
		status = new Status(2);
		setZ(0.5);
	}
	virtual ~Enemy(){
	}

	int getExp(){
		return exp;
	}
	virtual void mainStart(){
		bdyEnable = false;
		atkEnable = false;
		Body::mainStart();
	}

	virtual void mainProc(){
		if(state==0){
			normal();
		}
		else if(state==10){
			damaged();
		}
		else if(state==99){
			disappear();
		}
		if(hp<0)	setState(99);
		if(isOut())	setState(99);
		Body::mainProc();
	}
	
protected:
	int exp;
	int kind;
	bool exist;

	virtual void normal(){};
	virtual void damaged(){};
	virtual void disappear(){};
};


class EnemySoldier : public Enemy{
public:
	EnemySoldier(): Enemy(){
		setImgId(3001);
		setHp(30, 30);
		exp = 10;
	}
	~EnemySoldier(){
	}

	virtual void draw(Drawer *drawer){
		switch(state){
			case STATE_NORMAL:
				setImage(40, 40, 40, 40, 0);
				break;
			case STATE_DAMAGED:
				setImage(40, 40, 40, 40, 0);
				break;
			case STATE_DISAPPEAR:
				if(time<25){
					imgRad+=17;
					setAlpha(255-time*10);
					setImage(30+time*7, 30+time*7, 30+time*7, 30+time*7, imgRad);
				}
				break;
		}
		Body::draw(drawer);
	}

protected:
	virtual void normal(){
		setBody(BDY_NORMAL, 20);
		if(time%50==0){
			direction = (int)random(3.5,7)*45;
			setSpeed(4, direction, 1);
		}
		else if(time%50 > 30){
			setSpeed(4, 0, 0);

			if(time%50==31){
				SwordShadow *swordShadow = new SwordShadow(200, 20, 80, 255);
				swordShadow->setParent(this, 0, 0);
				swordShadow->wield(4, direction - 100, direction+100);

			}
			else if(time%50==32){
				int atk = (int)randomRange(status->getMinPAtk(), status->getMaxPAtk());
				setAttack(atk, 0, 4, 100, direction-45, direction+45);
			}
		}
	}
	virtual void damaged(){
		if(time>4){
			setState(0);
		}
	}
	virtual void disappear(){
		if(time>=25){
			finish();
		}
	}
};

class BodySystem: public BodyManager{
public:
	BodySystem(int size) :
		BodyManager(size){
	}
	virtual ~BodySystem(){
	}

	Enemy* addEnemy(float x,float y,int z,int kind, int state){
		for(int i=0;i<size;i++){
			if(body[i]==NULL){
				Enemy *enemy = new Enemy();
				body[i] = enemy;
				body[i]->onCreate();
				body[i]->setPosition(x, y);
				return enemy;
			}
		}
	}
		
	EnemySoldier* addEnemySoldier(float x,float y,int z,int state){
		for(int i=0;i<size;i++){
			if(body[i]==NULL){
				EnemySoldier *enemy = new EnemySoldier();
				body[i] = enemy;
				body[i]->onCreate();
				body[i]->setPosition(x, y);
				return enemy;
			}
		}
	}

protected:
};
