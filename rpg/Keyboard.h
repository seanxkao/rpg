#pragma once

#include "Drawer.h"


class Keyboard{
public:
	Keyboard(int keyLength, int recordLength){
		this->keyLength = keyLength;
		keyRecord = new int[recordLength];
		key = new Key*[keyLength];
		for(int i=0; i<keyLength; i++){
			key[i] = new Key();
			key[i]->now = false;
			key[i]->last = false;
			key[i]->down = false;
			key[i]->up = false;
		}
		for(int i=0; i<recordLength; i++){
			keyRecord[i] = KEY_NO;
		}
		keyTime = 0;
	}
	~Keyboard(){
	}

	enum KEYNAME{
		KEY_NO,
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_NATK,
		KEY_SATK,
		KEY_SLOW,
		KEY_STAT
	};

	bool isPressed(int keyName){
		return key[keyName]->now;
	}
	bool isDown(int keyName){
		return key[keyName]->down;
	}
	void press(int keyName){
		key[keyName]->buffer = true;
	}
	void release(int keyName){
		key[keyName]->buffer = false;
	}

	void main(){
		for(int i=0;i<keyLength;i++){
			Key *keyNow = key[i];
			keyNow->last = keyNow->now;
			keyNow->now = keyNow->buffer;
			keyNow->down = false;
			keyNow->up = false;

			if(keyNow->now && !keyNow->last){
				keyNow->down = true;
			}
			else if(!keyNow->now && keyNow->last){
				keyNow->up = true;
			}
		}
	}


protected:

	struct Key{
		bool buffer;
		bool now;
		bool last;
		bool down;
		bool up;
	};


	int keyLength;
	int recordLength;
	Key **key;
	int *keyRecord;
	int keyTime;

};
