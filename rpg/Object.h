#pragma once

#include "global.h"
#include "Maths.h"

class Keyboard;
class Drawer;

using namespace std;

class Object{	//the foundamental game object
public:
	static queue<Object*> *createQueue;
	static const unsigned int RUNNABLE = 1;
	static const unsigned int DRAWABLE = 2;
	static const unsigned int INPUTABLE = 4;
	static const unsigned int ZOMBIE = 8;
	
	Object(){
		init();
		setSpeed(0, 0, 0);
		setAccel(0, 0);
		setParent(NULL, 0, 0);
		finished = false;
		flag = 0;
		onFlag(RUNNABLE);
		Object::createQueue->push(this);
	}
	
	virtual ~Object(){
	}
	
	void init(){
		time = 0;
		state = 0;
		nextState = 0;
	}

	void setPosition(float x, float y){
		this->x = x;
		this->y = y;
	}

	void setSpeed(float mx, float my){
		this->mx = mx;
		this->my = my;
	}
	
	void setSpeed(float speed, float rad, float rate){
		this->speed = speed;
		this->rad = rad;
		this->rate = rate;
		float mx = cosf(rad/180*3.14)*speed*rate;
		float my = sinf(rad/180*3.14)*speed*rate;
		setSpeed(mx, my);
	}

	void setAccel(float ax, float ay){
		this->ax = ax;
		this->ay = ay;
	}
	
	void setState(int nextState){
		this->nextState = nextState;
	}
	void setParent(Object *parent, float parentX, float parentY){
		this->parent = parent;
		this->parentX = parentX;
		this->parentY = parentY;
	}
	void setParent(float parentX, float parentY){
		setParent(parent, parentX, parentY);
	}
	
	Object* getParent(){
		return parent;
	}
	

	void setZ(float imgZ){
		this->imgZ = imgZ;
	}
	
	float getX(){
		return x;
	}
	
	float getY(){
		return y;
	}
	
	float getZ(){
		return imgZ;
	}
	
	int getState(){
		return state;
	}
	
	int getTime(){
		return time;
	}

	void finish(){
		finished = true;
		onFinish();
	}
	
	bool isFinished(){
		return finished;
	}
	
	int isRunnable(){
		return flag&RUNNABLE;
	}
	
	int isDrawable(){
		return flag&DRAWABLE;
	}
	
	int isInputable(){
		return flag&INPUTABLE;
	}
	
	int isZombie(){
		return flag&ZOMBIE;
	}
	
	void onFlag(unsigned int on){
		flag |= on;
	}
	
	void offFlag(unsigned int off){
		flag &= ~off;
	}
	
	virtual void main(){
		mainStart();
		mainProc();
		mainEnd();
	}
	virtual void draw(Drawer *drawer){};
	virtual void onInput(Keyboard *keyboard){};

protected:
	float x;
	float y;
	float mx;
	float my;
	float ax;
	float ay;
	float speed;
	float rad;
	float rate;
	int	state;
	int	time;
	int nextState;
	Object *parent;
	float parentX;
	float parentY;
	bool finished;
	float imgZ;
	int flag;

	virtual void stateStart(){
	}

	virtual void stateEnd(){
	}

	virtual void mainStart(){
		//use parent coordinate
		if(parent!=NULL){
			x = parent->getX() + parentX;
			y = parent->getY() + parentY;
		}
		//change state
		if(state != nextState){
			stateEnd();
			time = 0;
			state = nextState;
		}
		if(time == 0){
			stateStart();
		}
	}
	virtual void mainProc(){
	}

	virtual void mainEnd(){
		mx += ax;
		my += ay;
		x += mx;
		y += my;
		time++;
	}
	
	virtual void onFinish(){};
	
};

class ZSorter{
public:
	bool operator()(Object *a, Object *b){
		if(a->getZ()!=b->getZ())return a->getZ()<b->getZ();
		if(a->getY()!=b->getY())return a->getY()>b->getY();
		return a<b;
	}
};

