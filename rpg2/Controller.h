#pragma once

#include "Keyboard.h"


class Controller{
public:
	Controller(){
	}
	~Controller(){
	}

	virtual float getDirection(){
		return direction;
	}
	virtual bool getNormalAtk(){
		return normalAtk;
	}
	virtual bool getStat(){
		return status;
	}


protected:
	float direction;
	bool normalAtk;
	bool status;
};

class MyController : public Controller{
public:
	virtual void onInput(Keyboard *keyboard){
		//接收鍵盤指令，轉換為角度
		bool L = keyboard->isPressed(Keyboard::KEY_LEFT);
		bool R = keyboard->isPressed(Keyboard::KEY_RIGHT);
		bool U = keyboard->isPressed(Keyboard::KEY_UP);
		bool D = keyboard->isPressed(Keyboard::KEY_DOWN);

		if(L && !R && !U && !D){
			direction = 180;
		}
		else if(!L && R && !U && !D){
			direction = 0;
		}
		else if(!L && !R && U && !D){
			direction = 90;
		}
		else if(!L && !R && !U && D){
			direction = 270;
		}
		else if(L && !R && U && !D){
			direction = 135;
		}
		else if(L && !R && !U && D){
			direction = 225;
		}
		else if(!L && R && U && !D){
			direction = 45;
		}
		else if(!L && R && !U && D){
			direction = 315;
		}
		else{
			direction = -1;
		}
		normalAtk = keyboard->isDown(Keyboard::KEY_NATK);
		status = keyboard->isDown(Keyboard::KEY_STAT);
	}
};