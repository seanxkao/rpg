#pragma once

#include "global.h"
#include "stdafx.h"

class Keyboard;
class Drawer;

class Object	//the foundamental game object
{
	public:

	Object(){
		cleanState();
		setSpeed(0, 0, 0);
		setAccel(0, 0);
		parent = NULL;
		finished = false;
	}

	virtual ~Object(){
	}

	void cleanState(){
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
		//設定加速度
		this->ax = ax;
		this->ay = ay;
	}
	
	void setState(int nextState){
		//切換狀態
		this->nextState = nextState;
	}
	void setTarget(Object *parent, float parentX, float parentY){
		this->parent = parent;
		this->parentX = parentX;
		this->parentY = parentY;
	}
	void setTarget(float parentX, float parentY){
		setTarget(parent, parentX, parentY);
	}

	float getX(){
		return x;
	}
	float getY(){
		return y;
	}
	int getState(){
		return state;
	}
	int getTime(){
		return time;
	}

	void finish(){
		finished = true;
	}
	bool isFinished(){
		return finished;
	}
	virtual void main(){
		mainStart();
		mainProc();
		mainEnd();
	}
/*
	virtual void draw(Drawer *drawer){
	};
	virtual void onInput(Keyboard *keyboard){
	};
*/
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

	virtual void stateStart(){
	}

	virtual void stateEnd(){
	}

	virtual void mainStart(){
		//preprocess
		//local coordinate
		if(parent != NULL){
			x = parent->getX() + parentX;
			y = parent->getY() + parentY;
		}
		//init
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
		//最後的處理
		//處理加速度、速度和位移
		mx += ax;
		my += ay;
		x += mx;
		y += my;
		time++;
	}

	void init(){
		cleanState();
		setSpeed(0, 0, 0);
		setAccel(0, 0);
		finished = false;
	}
	
};


