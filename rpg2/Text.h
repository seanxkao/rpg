#pragma once

#include "Object.h"
#include "Drawer.h"

class Text : public Object{

public:
	Text(){
		exist = false;
		setFixed(false);
		setWord("");
		setARGB(255, 255, 255, 255);
	}
	Text(string word, int colorA, int colorR, int colorG, int colorB){
		exist = false;
		setFixed(false);
		setWord(word);
		setARGB(colorA, colorR, colorG, colorB);
	}
	virtual ~Text(){
	}

	enum STATE{
		STATE_IN,	//出現 0
		STATE_ON,	//顯示 1
		STATE_OUT	//消失 2
	};
	void setFont(ID3DXFont *font){
		this->font = font;
	}
	void setFixed(bool fixed){
		this->fixed = fixed;
	}
	void setText(float textLeft,float textTop,float textWidth,float textHeight){
		this->textLeft		= textLeft;
		this->textTop		= textTop;
		this->textWidth		= textWidth;
		this->textHeight	= textHeight;
	}

	void setARGB(int colorA, int colorR, int colorG, int colorB){
		if(colorA < 0)		this->colorA = 0;
		else if(colorA>255)	this->colorA = 255;
		else				this->colorA = colorA;		
		if(colorR < 0)		this->colorR = 0;
		else if(colorR>255)	this->colorR = 255;
		else				this->colorR = colorR;
		if(colorG < 0)		this->colorG = 0;
		else if(colorG>255)	this->colorG = 255;
		else				this->colorG = colorG;
		if(colorB < 0)		this->colorB = 0;
		else if(colorB>255)	this->colorB = 255;
		else				this->colorB = colorB;
	}

	void setAlpha(int colorA){
		setARGB(colorA, this->colorR, this->colorG, this->colorB);
	}

	void setWord(string word){
		wordStr = word;
		this->word = wordStr.c_str();
	}
	void setExist(bool exist){
		this->exist = exist;
	}

	bool isExist(){
		return exist;
	}

	virtual void draw(Drawer *drawer){
		float cameraX = drawer->getCameraX();
		float cameraY = drawer->getCameraY();
		
		int rectLeft;
		int rectTop;
		int rectRight;
		int rectBottom;
		if(fixed){
			rectLeft = (int)((x - textLeft));
			rectTop = (int)(SCREEN_HEIGHT-(y - textTop));
			rectRight = (int)((x - textLeft + textWidth));
			rectBottom = (int)(SCREEN_HEIGHT-(y - textTop + textHeight));
		}
		else{
			rectLeft = (int)((x - textLeft)-cameraX);
			rectTop = (int)(SCREEN_HEIGHT-(y - textTop)+cameraY);
			rectRight = (int)((x - textLeft + textWidth)-cameraX);
			rectBottom = (int)(SCREEN_HEIGHT-(y - textTop + textHeight )+cameraY);
		}
		SetRect(&rect, rectLeft, rectTop, rectRight, rectBottom);
		font->DrawText(NULL, word, -1, &rect, DT_LEFT|DT_NOCLIP, D3DCOLOR_ARGB(colorA, colorR, colorG, colorB));
	}

	virtual void mainProc(){
	}

protected:
	bool exist;
	bool fixed;

	float textLeft;
	float textTop;
	float textWidth;
	float textHeight;

	RECT rect;

	int colorA;
	int colorR;
	int colorG;
	int colorB;
	
	ID3DXFont *font;
	LPCTSTR word;
	string wordStr;
};


class AnimeText : public Text{
public:
	AnimeText() : 
		Text(){
	}
	AnimeText(LPCTSTR word, int colorA, int colorR, int colorG, int colorB) :
		Text(word, colorA, colorR, colorG, colorB){
	}
	virtual ~AnimeText(){
	}
	
	void setIn(int inTime,int inMode,float inX,float inY){
		this->inTime = inTime;
		this->inMode = inMode;
		this->inX = inX;
		this->inY = inY;
	}
	void setOn(int onTime,int onMode,float onX,float onY){
		this->onTime = onTime;
		this->onMode = onMode;
		this->onX = onX;
		this->onY = onY;
	}
	void setOut(int outTime,int outMode,float outX,float outY){
		this->outTime = outTime;
		this->outMode = outMode;
		this->outX = outX;
		this->outY = outY;
	}

	virtual void main(){
		switch(state){
		case STATE_IN:
			if(inMode == 0){
				if(time < inTime){
					float ratio = (float)time / (float)inTime;
					setPosition(inX + (onX-inX) * ratio, inY + (onY-inY) * ratio);
					setAlpha(255*ratio);
				}
				else{
					setState(1);
					setPosition(onX, onY);
				}
			}
			break;
		case STATE_ON:
			setPosition(onX, onY);
			setAlpha(255);

			if(onMode == 0){
				if(time >= onTime){
					setState(2);
				}
			}
			else if(onMode == 1){
			}
			break;
		case STATE_OUT:
			if(outMode == 0){
				if(time < outTime){
					float ratio = (float)time / (float)outTime;
					setPosition (onX + (outX-onX) * ratio, onY + (outY-onY) * ratio);
					setAlpha(255*(1 - ratio));
				}
				else{
					setAlpha(0);
					setExist(false);
				}
			}
			break;
		}
		Text::main();

	}
protected:
	int inTime, inMode, inX, inY;
	int onTime, onMode, onX, onY;
	int outTime, outMode , outX, outY;
};


class TextManager{
public:	
	TextManager(int size, ID3DXFont *font){
		this->font = font;
		this->size = size;
		text = new AnimeText[size];
		for(int i = 0; i < size; i++){
			text[i].setExist(false);
			text[i].init();
		}
	}

	virtual ~TextManager(){		
		delete []text;
	}

	void addText(string word, float left, float top, float width, float height, int colorA, int colorR, int colorG, int colorB, int inTime,int inMode,float inX,float inY, int onTime,int onMode,float onX,float onY, int outTime,int outMode,float outX,float outY){
		for(int i = 0; i < size; i++){
			if(!text[i].isExist()){
				text[i].setFont(font);
				text[i].setExist(true);
				text[i].init();
				text[i].setText(left, top, width, height);
				text[i].setWord(word);
				text[i].setARGB(colorA, colorR, colorG, colorB);
				text[i].setIn(inTime, inMode, inX, inY);
				text[i].setOn(onTime, onMode, onX, onY);
				text[i].setOut(outTime, outMode, outX, outY);
				break;
			}
		}
	}


	virtual void draw(Drawer *drawer){
		for(int i = 0;i < size;i++){
			if(text[i].isExist()){
				text[i].draw(drawer);
			}
		}
	}
	virtual void main(){
		for(int i = 0;i < size;i++){
			if(text[i].isExist()){
				text[i].main();
			}
		}
	}
protected:
	int	size;
	AnimeText *text;
	ID3DXFont *font;
};
