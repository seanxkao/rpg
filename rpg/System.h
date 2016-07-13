#pragma once

#include <list>

#include "Bar.h"
#include "Image.h"
#include "Keyboard.h"
#include "Drawer.h"

#include <stdio.h>

using namespace std;

//singleton

class System{
public:
	System(){
		drawer = new Drawer();
		keyboard= new Keyboard(20, 10);
		allObject = Object::allObject;
	}
	~System(){
		delete drawer;
		delete keyboard;
	}
	
	Drawer* getDrawer(){
		return drawer;
	}
	Keyboard* getKeyboard(){
		return keyboard;
	}
	
	Bar* createBar(int type, Body *refer, Object *target){
		Bar *bar;
		if(type==0){
			bar = (Bar*)(new HpBar(refer, target)); 
		}
		return bar;
	}
	
	Menu* createMenu(int type){
		Menu *menu;
		if(type==0){
			menu = (Menu*)(new MainMenu()); 
		}
		else if(type==1){
			menu = (Menu*)(new StartMenu()); 
		}
		else if(type==2){
			menu = (Menu*)(new TrainingMenu()); 
		}
		return menu;
	}

	virtual void onInput(){
		keyboard->main();
		for(auto inputable: *allObject){
			if(inputable->isInputable()){
				inputable->onInput(keyboard);
			}
		}
	}

	virtual void draw(){
		for(auto drawable: *allObject){
			if(drawable->isDrawable()){
				drawable->draw(drawer);
			}
		}
	}

	virtual void main(){
		for(auto runnable: *allObject){
			if(runnable->isRunnable()){
				runnable->main();
			}
		}
	}

	void garbageCollect(){
		for(auto it = allObject->begin();it!=allObject->end();){
			if((*it)->isFinished()){
				if(!(*it)->isZombie()){
					delete (*it);
				}
				it = allObject->erase(it);
			}
			else{
				++it;
			}
		}
	}

protected:
	list<Object*> *allObject;
	list<Object*> runnables;
	list<Object*> inputables;
	list<Object*> drawables;
	Drawer *drawer;
	Keyboard *keyboard;
};