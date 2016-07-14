#pragma once

#include "global.h"
#include "Button.h"
#include "Keyboard.h"

class Menu: public Object{
public:
	static const int COOL_DOWN = 10;
	enum STATE{
		STATE_CREATE,	
		STATE_NORMAL,	
		STATE_PRESSED,	
		STATE_DISAPPEAR	
	};
	
	Menu(float x,float y, int size){
		btnPressed = -1;
		this->x = x;
		this->y = y;
		this->size = size;
		onFlag(RUNNABLE | DRAWABLE | INPUTABLE);
	};

	virtual ~Menu(){
	};
	
	int getPressed(){
		return btnPressed;
	}

	void addImage(Image *image){
		this->image.push_back(image);
	}

	void onInput(Keyboard *keyboard){
		if(btnNow==NULL)return;
		if(state==STATE_NORMAL){
			if(cooldown == 0){
				if(keyboard->isPressed(Keyboard::KEY_LEFT)){
					if(btnNow != btnNow->getBtnLeft()){
						cooldown  = COOL_DOWN;
						btnNow = btnNow->getBtnLeft();
					}
				}
				else if(keyboard->isPressed(Keyboard::KEY_RIGHT)){
					if(btnNow != btnNow->getBtnRight()){
						cooldown  = COOL_DOWN;
						btnNow = btnNow->getBtnRight();
					}
				}
				else if(keyboard->isPressed(Keyboard::KEY_UP)){
					if(btnNow != btnNow->getBtnUp()){
						cooldown  = COOL_DOWN;
						btnNow = btnNow->getBtnUp();
					}
				}
				else if(keyboard->isPressed(Keyboard::KEY_DOWN)){
					if(btnNow != btnNow->getBtnDown()){
						cooldown  = COOL_DOWN;
						btnNow = btnNow->getBtnDown();
					}
				}
			}
			if(keyboard->isPressed(Keyboard::KEY_NATK)){
				if(btnPressed!=NULL){
					btnPressed = btnNow->getBtnId();
					setState(2);
				}
			}
		}
	}

	virtual void draw(Drawer *drawer){
	}
	
	virtual void stateStart(){
		if(state == STATE_DISAPPEAR){
			for(vector<Image*>::iterator it = image.begin();it!=image.end();++it){
				Image *imgNow = *it;
				imgNow->setState(Anime::STATE_OUT);
			}
		}
	}
	virtual void mainProc(){
		if(state == STATE_CREATE){
			create();
		}
		else if(state == STATE_NORMAL){
			normal();
		}
		else if(state == STATE_PRESSED){
			pressed();
		}
		else if(state == STATE_DISAPPEAR){
			disappear();
		}
		if(cooldown > 0)cooldown--;
	}
protected:
	int		size;
	int 	cooldown;
	Button	*button;
	Button	*btnNow;
	int		btnPressed;
	bool	finished;
	virtual void create(){};
	virtual void normal(){};
	virtual void pressed(){};
	virtual void disappear(){};
	
	vector<Image*>  image;
};

class ListMenu: public Menu{
public:
	static const bool HORIZONTAL = true;
	static const bool VERTICLE = false;
	
	ListMenu(float x, float y, float width, float height, int size, float space, float distance, bool orientation)
	: Menu(x, y,size){
		this->orientation = orientation;
		this->distance = distance;
		button = new MainButton[size];
		btnNow = NULL;

		for(int i=0;i<size;i++){
			button[i].setBtnId(i);
			if(orientation == HORIZONTAL){
				button[i].setBtnLink(button + (size+i-1)%size, button + (size+i+1)%size,NULL,NULL);
				button[i].setPosition(x + (i - (size-1)/2)*(width+space), y);
				button[i].setImage(width/2, height/2, width/2, height/2, 0);
			}
			else{
				button[i].setBtnLink(NULL, NULL, button + (size+i-1)%size, button+(size+i+1)%size);
				button[i].setPosition(x, y - (i-(size-1)/2)*(height+space));
				button[i].setImage(width/2, height/2, width/2, height/2, 0);
			}

		}
		cooldown = 0;
	};

	virtual ~ListMenu(){
	};

	virtual void create(){
		if(time%3 == 0 && time/3 < size) {
			button[time/3].setState(Button::STATE_ACTIVE);
		}
		for(int i=0;i<size;i++){
			if(time >= i*3 && time < i*3+10){
				if(orientation == HORIZONTAL){
					button[i].setPosition(button[i].getX(), y - distance + (time-i*3) * distance / 10 ); 
				}else{
					button[i].setPosition(x - distance + (time-i*3)*distance/10, button[i].getY()); 
				}
			}
		}

		if(time > size*3+10){
			setState(1);
		}
	}
	virtual void normal(){
		if(time==0){
			btnNow	= button;
		}
		for(int i=0;i<size;i++){
			Button *buttonI = button+i;
			buttonI->setChoosed(buttonI==btnNow);
		}
	}
	virtual void pressed(){
		if(time<20){
			btnNow->setState(Button::STATE_PRESSED);
		}
		else{
			setState(3);
			btnNow = NULL;
		}
	}
	virtual void disappear(){
		if(time <= size*3+10){
			if(time%3==0 && time/3 < size){
				button[time/3].setState(Button::STATE_DISAPPEAR);
			}
			for(int i=0;i<size;i++){
				if(time >= i * 3  && time < i*3 +10 ){
					if(orientation == HORIZONTAL){
						button[i].setPosition( button[i].getX() ,y - (time-i*3) * distance / 10 ); 
					}
					else {
						button[i].setPosition( x + (time-i*3) * distance / 10,button[i].getY() ); 
					}
				}
			}
		}
		else{
			finish();
		}
	}

protected:
	float distance;
	bool orientation;	//0 = horizontal, 1 = verticle
};

class MainMenu: public ListMenu{
public:
	MainMenu(): ListMenu(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 65 , 300, 100, 4, 10, 40, VERTICLE){
		button[0].setBtnImg(0, 1, 1);
		button[1].setBtnImg(2, 3, 3);
		button[2].setBtnImg(4, 5, 5);
		button[3].setBtnImg(6, 7, 7);
		addImage(new ThemeAnime());
	}
	virtual ~MainMenu(){
	}
};

class StartMenu: public ListMenu{
public:
	StartMenu(): ListMenu(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 -10 , 300, 100, 5,10,40, VERTICLE){
		button[0].setBtnImg(20,21,21);
		button[1].setBtnImg(22,23,23);
		button[2].setBtnImg(24,25,25);
		button[3].setBtnImg(26,27,27);
		button[4].setBtnImg(28,29,29);
	}
	virtual ~StartMenu(){
	}
};

class TrainingMenu: public ListMenu{
public:
	TrainingMenu(): ListMenu(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 -10 , 1000, 100, 5,10,0, VERTICLE){
		button[0].setBtnImg(30,31,31);
		button[1].setBtnImg(32,33,33);
		button[2].setBtnImg(34,35,35);
		button[3].setBtnImg(30,31,31);
		button[4].setBtnImg(30,31,31);
	}
	virtual ~TrainingMenu(){
	}
};