#pragma once

#include "Image.h"

class Body : public Image{
public:
	Body() : 
		Image(){
		bdyEnable = false;
		atkEnable = false;
		direction = 90;
		swordShadow = new SwordShadow(200, 20, 80, 255);
		swordShadow->setParent(this, 0, 0);
		onFlag(RUNNABLE | DRAWABLE);
	}

	virtual ~Body(){
		swordShadow->finish();
	}

	enum STATE {
		STATE_NORMAL,			//0
		STATE_DAMAGED = 10,		//10
		STATE_DISAPPEAR = 99	//99
	};

	enum BDYKIND{
		BDY_NORMAL		//0
	};

	enum ATKKIND{
		ATK_NORMAL		//0
	};

	void setBody(float bdyKind, float bdyRad){
		this->bdyKind = bdyKind;
		this->bdyRad = bdyRad;
		this->bdyEnable = true;
	}
	
	void setAttack(int atkDamage, int atkKind, float atkKnock, float atkRad, float atkStart, float atkEnd){
		this->attack.rad = atkRad;
		this->attack.start = atkStart;
		this->attack.end = atkEnd;
		this->attack.knock = atkKnock;
		this->attack.damage = atkDamage;
		this->attack.kind = atkKind;
		this->atkEnable = true;
	}
	void setAttack(int atkDamage, int atkKind, float atkKnock, float atkRad){
		setAttack(atkDamage, atkKind, atkKnock, atkRad, 0, 0);
	}

	Attack getAttack(){
		return attack;
	}

	static bool crash(Body *A, Body *B){
		//不可碰撞單位
		if (!(A->atkEnable && B->bdyEnable)) return false;
		//判斷是圓形還是扇形
		Vector2D w(B->getX() - A->getX(), B->getY() - A->getY());
		float wDis = w.length();
		float dis = A->attack.rad + B->bdyRad;
		if(A->attack.start == A->attack.end) return wDis < dis;
		//扇形向量公式
		Vector2D *u;
		Vector2D *v;
		u = Vector2D::polar(A->attack.rad, A->attack.start);
		v = Vector2D::polar(A->attack.rad, A->attack.end);
		float uDis = u->length();
		float vDis = v->length();
		float alpha = (w.cross(v) + B->bdyRad*vDis)/u->cross(v);
		float beta = (u->cross(w) + B->bdyRad/vDis*u->dot(v))/u->cross(v);
		return (wDis<dis && alpha>0 && beta>0);
	}

	virtual void onAttack(Body *enemy){
		
	}

	virtual int onDamaged(Body *enemy){
		float rad = vectorR(x, y, enemy->getX(), enemy->getY());
		float len = enemy->getAttack().knock;
		setSpeed(len, rad, 2);
		setState(10);
		int damage = enemy->getAttack().damage;
		this->hp -= damage;
		return damage;
	}

	void setHp(int hp, int maxHp){
		this->hp = hp;
		this->maxHp = maxHp;
	}

	int getHp(){
		return hp;
	}
	int getMaxHp(){
		return maxHp;
	}

	virtual void onCreate(){
		swordShadow->init();		
	}

protected:
	float direction;
	int bdyKind;
	float bdyRad;
	bool bdyEnable;
	Attack attack;
	bool atkEnable;
	int hp;
	int maxHp;
	Status *status;	
	SwordShadow *swordShadow;
};


class BodyManager{
public:
	BodyManager(int size){
		this->size = size;
		body = new Body*[size];
		for(int i=0;i<size;i++){
			body[i] = NULL;
		}
	}

	~BodyManager(){
		for(int i=0;i<size;i++){
			if(body[i]!=NULL){
				delete body[i];
				body[i] = NULL;
			}
		}
	}


	Body* addBody(){
		for(int i=0;i<size;i++){
			if(body[i]==NULL){
				return body[i];
			}
		}
		return NULL;
	}
	void removeBody(int i){
		body[i]=NULL;
	}

	Body* getBody(int i){
		return body[i];
	}
protected:
	int size;
	Body **body;
};

