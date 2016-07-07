#pragma once


#include "Bar.h"
#include "Bullet.h"
#include "Image.h"
#include "Maths.h"




class Enemy : public Body{
public:
	Enemy(BodyManager *manager) : 
		Body(manager){
		setImage(15, 15, 30, 30, 0, 0);
		setImgId(1000);
		setHp(100, 100);
		status = new Status(2);
	}
	virtual ~Enemy(){
	}

	int kind;
	bool exist;

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
/*
	virtual void mainProc(Drawer *drawer){
		if(kind == 0){
			if(state==0){
				if(time==0){
					hp=30;
					setBody(20);
					hpBar = new HpBar(this);
				}
				if(time%30==0){
					bulletManager->addBullet(x,y,0,30,270,1,1,0,0,1,100);
				}
				hpBar->main(drawer);
				if(hp<0)setState(99);
				if(isOut())setState(99);
			}

			else if(state==10){
				if(time > 4){
					setState(0);
				}
			}
			else if(state==99){
				if(time<25){
					hpBar->main(drawer);
					imgRad+=17;
					setARGB(255 - time*10);
					setImage(30 + time*7,30 + time*7,30 + time*7,30 + time*7, 0, imgRad);
				}
				else{
					delete hpBar;
					hpBar = NULL;
					setARGB(0);
					setExist(false);
				}
			}
		}
		else if(kind == 1){
			//Âø§L
			if(state==0){
				if(time==0 && hpBar==NULL){
					imgId = 3001;
					hp = 30;
					max_hp = 30;
					hpBar = new HpBar(this);
				}
					setImage(40, 40, 40, 40, 0, 0);
					setBody(20);
				if(time%50==0){
					changeRad((int)random(3.5,7)*45);
					changeRate(1);
				}
				else if(time%50 > 30){
					changeRate(0);
				}
				hpBar->main(drawer);
				if(hp<0)setState(99);
				if(isOut())setState(99);
			}

			else if(state==1){
				hpBar->main(drawer);
				if(hp<0)setState(99);
				if(isOut())setState(99);
			}


			else if(state==10){
				if(time > 4){
					setState(0);
				}
				hpBar->main(drawer);
				if(hp<0)setState(99);
				if(isOut())setState(99);
			}
			else if(state==99){
				if(time<25){
					imgRad+=17;
					setARGB(255 - time*10);
					setImage(30 + time*7,30 + time*7,30 + time*7,30 + time*7, 0, imgRad);
					hpBar->main(drawer);
				}
				else{
					delete hpBar;
					hpBar = NULL;
					setARGB(0);
					setExist(false);
				}
			}
		}

		draw(drawer);
		Body::mainProc();
	}
*/
protected:
	int exp;


	virtual void normal(){};
	virtual void damaged(){
	};

	virtual void disappear(){

	}

};


class EnemySoldier : public Enemy{
public:
	EnemySoldier(BodyManager *manager) :
		Enemy(manager){
		setImgId(3001);
		setHp(30, 30);
		exp = 10;
	}
	~EnemySoldier(){
	}

	virtual void draw(Drawer *drawer){
		switch(state){
			case STATE_NORMAL:
				setImage(40, 40, 40, 40, 0, 0);
				break;
			case STATE_DAMAGED:
				if(time>4){
					setState(0);
				}

				break;
			case STATE_DISAPPEAR:
				if(time<25){
					imgRad+=17;
					setARGB(255 - time*10);
					setImage(30 + time*7,30 + time*7,30 + time*7,30 + time*7, 0, imgRad);
				}
				break;
		}
		Body::draw(drawer);
	}

protected:

	void stateStart(){

	}

	virtual void normal(){
		setBody(BDY_NORMAL, 20);

		if(time%50==0){
			setSpeed(4, (int)random(3.5,7)*45, 1);
		}
		else if(time%50 > 30){
			setSpeed(4, 0, 0);

			if(time%50==31){
				swordShadow->wield(4, direction - 100, direction+100);

			}
			else if(time%50==32){
				int atk = (int)randomRange(status->getMinPAtk(), status->getMaxPAtk());
				setAttack(atk, 0, 4, 100, direction-45, direction+45);
			}
		}
		Enemy::normal();
	}
	virtual void damaged(){
		Enemy::damaged();
	}
	virtual void disappear(){
		if(time>=25){
			finish();
		}
		Enemy::disappear();
	}


protected:

};


class BodySystem : public BodyManager{
public:
	BodySystem(int size) :
		BodyManager(size){
	}
	virtual ~BodySystem(){
	}

	void release(int i){
		if(body[i]->isFinished()){
			delete body[i];
			body[i] = NULL;
		}
	}

	Enemy* addEnemy(float x,float y,int z,int kind, int state){
		for(int i=0;i<size;i++){
			if(body[i]==NULL){
				Enemy *enemy = new Enemy(this);
				body[i] = enemy;
				body[i]->cleanState();
				body[i]->setPosition(x, y);

				return enemy;
			}
		}
	}
		
	EnemySoldier* addEnemySoldier(float x,float y,int z,int state){
		for(int i=0;i<size;i++){
			if(body[i]==NULL){
				EnemySoldier *enemy = new EnemySoldier(this);
				body[i] = enemy;
				body[i]->cleanState();
				body[i]->setPosition(x, y);
					
				return enemy;
			}
		}
		return NULL;
	}


	virtual void draw(Drawer *drawer){
		for(int i=0;i<size;i++){
			if(body[i]!=NULL){
				body[i]->draw(drawer);
			}
		}
	}
	virtual void main(){
		for(int i=0;i<size;i++){
			if(body[i]!=NULL){
				body[i]->main();
			}
		}
	}



protected:
};
