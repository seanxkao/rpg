#pragma once

//C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <memory.h>
#include <tchar.h>
//C++
#include <vector>
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
//winapi
#include <windows.h>
//local
#include "Maths.h"

using namespace std;

//A~Z = 41~5A

//©w¸q

#define PLAYER1_LEFT	VK_LEFT	
#define PLAYER1_RIGHT	VK_RIGHT
#define PLAYER1_UP		VK_UP	
#define PLAYER1_DOWN	VK_DOWN
#define PLAYER1_NATK	0x5A	//Z
#define PLAYER1_SATK	0x58	//X
#define PLAYER1_STAT	0x53	//S

const float W = GetSystemMetrics(SM_CXSCREEN);
const float H = GetSystemMetrics(SM_CYSCREEN);

#define SCREEN_WIDTH  W
#define SCREEN_HEIGHT H

#define FRAME_LEFT   0
#define FRAME_RIGHT  W
#define FRAME_UP     H
#define FRAME_DOWN   50


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

void* new2d(int w, int h, int size){
	void** p = (void**)(new char[w*h*size + w*sizeof(void*)]);
	for(int i = 0; i < w; i++){
		p[i] = (char*)(p+w) + i*h*size;
	}
	return p;
} 

#define new2D(W, H, TYPE) (TYPE**)(new2d(W, H, sizeof(TYPE)))
