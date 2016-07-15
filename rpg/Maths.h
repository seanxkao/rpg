#pragma once

#include "math.h"

const float PI = 3.1415;

//求旋轉後X座漂
float vectorX(float speed, float r)
{
	return cosf(r*PI/180)*speed;
}

//求旋轉後Y座漂
float vectorY(float speed, float r)
{
	return sinf(r*PI/180)*speed;
}


float vectorR(float x1,float y1,float x2, float y2)
{
	//求角度
	if (x1 == x2){
		return y1<=y2?270:90;
	}
	float R=atanf((y2-y1)/(x2-x1))*180/PI;
	return (x2>x1)?R+180:R;
}

template <class T>
T bound(T value, T min, T max){
	if (value > max){
		return max;
	}
	else if(value < min){
		return min;
	}
	else{
		return value;
	}
}


float randomRange(float min, float max)
{
	return min+((float)rand()/(float)RAND_MAX)*(max-min);
}

float random(float exp, float range)
{
	return exp+(0.5-(float)rand()/(float)RAND_MAX)*range;
}

//二維向量

class Vector2D{
public:
	Vector2D();
	Vector2D(float, float);
	Vector2D(Vector2D*);
	virtual ~Vector2D();

	Vector2D operator*(const float& scalar){
		Vector2D temp(this->x*scalar, this->y*scalar);
		return temp;
	}
	
	float getX(){
		return x;
	}
	float getY(){
		return y;
	}

	void normalize(){
		x = x/length();
		y = y/length();
	}

	static Vector2D* polar(float len, float rad){
		float r = rad * PI / 180;
		return new Vector2D(len*cosf(r), len*sinf(r));
	}

	float dot(Vector2D v){
		return x*v.x + y*v.y;
	}
	float cross(Vector2D v){
		return x*v.y - y*v.x; 
	}
	float length(){
		return sqrt(x*x + y*y);
	}

protected:
	float x;
	float y;
};

Vector2D::Vector2D(){
}
Vector2D::Vector2D(float x, float y){
	this->x = x;
	this->y = y;
}
Vector2D::Vector2D(Vector2D *v){
	this->x = v->x;
	this->y = v->y;
}
Vector2D::~Vector2D(){
}