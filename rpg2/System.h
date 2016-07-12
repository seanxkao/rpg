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
	System(int comSize, int ctrlComSize){
		drawer = new Drawer();
		keyboard= new Keyboard(20, 10);
		this->comSize = comSize;
		com = new Component*[comSize];
		for(int i=0; i<comSize; i++){
			com[i] = NULL;
		}
		this->ctrlComSize = ctrlComSize;
		ctrlCom = new ControlComponent*[ctrlComSize];
		for(int i=0; i<ctrlComSize; i++){
			ctrlCom[i] = NULL;
		}
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

	void addCtrlCom(ControlComponent *newCtrlCom){
		for(int i=0;i<ctrlComSize;i++){
			if(ctrlCom[i]==NULL){
				ctrlCom[i] = newCtrlCom;
				break;
			}
		}
	}

	void addCom(Component *newCom){
		for(int i=0;i<comSize;i++){
			if(com[i]==NULL){
				com[i] = newCom;
				break;
			}
		}
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
		auto it = inputables.begin();
		while(it!=inputables.end()){
			Object *inputable = *it;
			if(!inputable->isFinished()){
				inputable->onInput(keyboard);
				it++;
			}
			else{
				inputables.erase(it++);
			}
		}
		
		for(int i=0;i<ctrlComSize;i++){
			if(ctrlCom[i]!=NULL){
				ctrlCom[i]->onInput(keyboard);
			}
		}
		
		for(auto inputable: *allObject){
			if(inputable->isInputable()){
				inputable->onInput(keyboard);
			}
		}
		
	}

	virtual void draw(){
		auto it = drawables.begin();
		while(it!=drawables.end()){
			Object *drawable = *it;
			if(!drawable->isFinished()){
				drawable->draw(drawer);
				it++;
			}
			else{
				drawables.erase(it++);
			}
		}
		
		for(int i=0;i<comSize;i++){
			if(com[i]!=NULL){
				com[i]->draw(drawer);
			}
		}
		for(int i=0;i<ctrlComSize;i++){
			if(ctrlCom[i]!=NULL){
				ctrlCom[i]->draw(drawer);
			}
		}
		
		for(auto drawable: *allObject){
			if(drawable->isDrawable()){
				drawable->draw(drawer);
			}
		}
	}

	virtual void main(){
		auto it = runnables.begin();
		while(it!=runnables.end()){
			Object *runnable = *it;
			if(!runnable->isFinished()){
				runnable->main();
				it++;
			}
			else{
				delete runnable;
				runnables.erase(it++);
			}
		}
		
		for(int i=0;i<comSize;i++){
			if(com[i]!=NULL){
				com[i]->main();
			}
		}
		for(int i=0;i<ctrlComSize;i++){
			if(ctrlCom[i]!=NULL){
				ctrlCom[i]->main();
			}
		}
		
		for(auto runnable: *allObject){
			if(runnable->isRunnable()){
				runnable->main();
			}
		}
	}

	void garbageCollect(){
		for(int i=0;i<comSize;i++){
			if(com[i]!=NULL){
				if(com[i]->isFinished()){
					delete com[i];
					com[i] = NULL;
				}
			}
		}
		for(int i=0;i<ctrlComSize;i++){
			if(ctrlCom[i]!=NULL){
				if(ctrlCom[i]->isFinished()){
					delete ctrlCom[i];
					ctrlCom[i] = NULL;
				}
			}
		}
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
	
	Component **com;
	ControlComponent **ctrlCom;
	int comSize;
	int ctrlComSize;

};