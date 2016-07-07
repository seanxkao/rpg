#pragma once

#include <list>

#include "Bar.h"
#include "Image.h"
#include "Keyboard.h"
#include "Drawer.h"

using namespace std;

//singleton

class System{
public:
	//static const int COM_SIZE = 500;




	System(int comSize, int ctrlComSize){
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
	}
	~System(){
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

	virtual void onInput(Keyboard *keyboard){
		for(int i=0;i<ctrlComSize;i++){
			if(ctrlCom[i]!=NULL){
				ctrlCom[i]->onInput(keyboard);
			}
		}
	}

	virtual void draw(Drawer *drawer){
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
	}

	virtual void main(){
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
	}

protected:
	list<Object*> runnables;
	list<Object*> inputables;
	list<Object*> drawables;

	Component **com;
	ControlComponent **ctrlCom;
	int comSize;
	int ctrlComSize;

};