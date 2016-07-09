#pragma once

#include "Image.h"
#include "Keyboard.h"


class Button : public Image{
public:

	Button();
	Button(int, int, int);
	virtual ~Button();
	
	//按鈕狀態
	static const int STATE_CREATE = 0;
	static const int STATE_ACTIVE = 1;
	static const int STATE_NORMAL = 2;
	static const int STATE_PRESSED = 3;
	static const int STATE_DISAPPEAR = 4;

	//設定上下左右連結到的的按鈕

	void setChoosed(bool choosed){
		this->choosed = choosed;
	}

	void setBtnLink(Button* button_left, Button* button_right, Button* button_up, Button* button_down){
		this->button_left  = button_left ;
		this->button_right = button_right;
		this->button_up    = button_up   ;
		this->button_down  = button_down ;
	}
	
	void setBtnImg(int imgNormal, int imgOn, int imgPress){
		this->imgNormal = imgNormal;
		this->imgOn     = imgOn    ;
		this->imgPress  = imgPress ;
	}

	void setBtnId(int btnId){
		this->btnId = btnId;
	}

	Button* getBtnLeft(){
		if(button_left != NULL){
			return button_left;
		}
		else{
			return this;
		}
	}
	Button* getBtnRight(){
		if(button_right != NULL){
			return button_right;
		}
		else{
			return this;
		}
	}

	Button* getBtnUp(){
		if(button_up != NULL){
			return button_up;
		}
		else{
			return this;
		}
	}
	Button* getBtnDown(){
		if(button_down != NULL){
			return button_down;
		}
		else{
			return this;
		}
	}

	int getBtnId(){
		return btnId;
	}

	virtual void onInput(Keyboard *keyboard){};

	virtual void create() = 0;
	virtual void active() = 0;
	virtual void normal() = 0;
	virtual void pressed() = 0;
	virtual void disappear() = 0;

	virtual void mainProc(){
		switch(state){
		case STATE_CREATE:
			create();
			break;
		case STATE_ACTIVE:
			active();
			break;
		case STATE_NORMAL:
			normal();
			break;
		case STATE_PRESSED:
			pressed();
			break;
		case STATE_DISAPPEAR:
			disappear();
			break;
		}
		Image::mainProc();
	}
protected:
	int imgNormal;
	int imgOn;
	int imgPress;
	int btnId;
	Button *button_left;
	Button *button_right;
	Button *button_up;
	Button *button_down;
	bool choosed;
private:
	void init(){
		//初始化
		//按鈕還未被選擇
		//未連結任何按鈕
		choosed = false;
		button_left = NULL;
		button_right = NULL;
		button_up = NULL;
		button_down = NULL;
	}
};


Button::Button() :
	Image(){
	init();
};

Button::Button(int imgNormal,int imgOn,int imgPress) :
	Image(){
	this->imgNormal = imgNormal;
	this->imgOn     = imgOn    ;
	this->imgPress  = imgPress ;
	init();
};

Button::~Button(){
};

class MainButton : public Button{

public:	
	MainButton();
	MainButton(int, int, int);
	virtual ~MainButton();


	virtual void draw(Drawer *drawer){
		switch(state){
		case STATE_CREATE:
			imgId = imgNormal;
			setAlpha(0);
			break;
		case STATE_ACTIVE:
			imgId = imgNormal;
			if(time < 10){
				setAlpha(time*25);
			}
			else{
				setAlpha(255);
			}
			break;
		case STATE_NORMAL:
			setAlpha(255);
			if(choosed){
				imgId = imgOn;
			}
			else{
				imgId = imgNormal;
			}
			break;
		case STATE_PRESSED:
			imgId = imgPress;
			break;
		case STATE_DISAPPEAR:
			imgId = imgNormal;
			if(time < 10){
				setAlpha(255-time * 25);
			}
			else{
				setAlpha(0);
			}
			break;
		}
		Image::draw(drawer);
	}

	void create(){
	}
	void active(){
		if(time >= 10){
			setState(STATE_NORMAL);
		}
	}
	void normal(){
	}
	void pressed(){
	}
	void disappear(){
	}

};

MainButton::MainButton() :
	Button(){
};

MainButton::MainButton(int imgNormal,int imgOn,int imgPress) :
	Button(imgNormal, imgOn, imgPress){
	imgId = imgNormal;
};

MainButton::~MainButton(){
};