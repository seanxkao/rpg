#pragma once

#include "math.h"
#include "Maths.h"
#include <vector>

using namespace std;

//A~Z = 41~5A

//定義

#define PLAYER1_LEFT	VK_LEFT	//左
#define PLAYER1_RIGHT	VK_RIGHT//右
#define PLAYER1_UP		VK_UP	//上
#define PLAYER1_DOWN	VK_DOWN	//下
#define PLAYER1_NATK	0x5A	//Z
#define PLAYER1_SATK	0x58	//X
#define PLAYER1_STAT	0x53	//S

const float W = GetSystemMetrics(SM_CXSCREEN);
const float H = GetSystemMetrics(SM_CYSCREEN);

#define SCREEN_WIDTH  W
#define SCREEN_HEIGHT H

//#define FRAME_LEFT   L
//#define FRAME_RIGHT  R
//#define FRAME_UP     U
//#define FRAME_DOWN   D


#define FRAME_LEFT   20
#define FRAME_RIGHT  W - 20
#define FRAME_UP     H + 50
#define FRAME_DOWN   -50

//const int L = 200;
//const int R = W - 200;
const int L = 20;
const int R = W - 20;
const int U = H - 50;
const int D = 50;


enum CONTROLER{
	AUTO,	
	PLAYER1,
	PLAYER2 
};


struct Attack{
	float rad;
	float start;
	float end;
	int damage;
	int kind;
	float knock;
};


////////////////////////////////////////函式定義/////////////////////////////////////////////////////


void* new2d(int w, int h, int size){
	void** p = (void**)(new char[w*h*size + w*sizeof(void*)]);
	for(int i = 0; i < w; i++){
		p[i] = (char*)(p+w) + i*h*size;
	}
	return p;
} 
#define new2D(W, H, TYPE) (TYPE**)(new2d(W, H, sizeof(TYPE)))
