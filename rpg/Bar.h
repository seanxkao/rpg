#pragma once

#include "Avatar.h"
#include "Body.h"
#include "Text.h"

typedef int (Body::*BodyFP)();

class Bar : public Object{
public:
	Bar(Body *refer, Object *target): Object(){
		this->refer = refer;
		frame = new Image();
		bar = new Image();
		frame->setParent(this, 0, 0);
		bar->setParent(this, 0, 0);
		//frame->setZ(0.61);
		//bar->setZ(0.60);
		setParent(target, 0, 0);
		setZ(0.60);
		onFlag(RUNNABLE | DRAWABLE);
	};

	virtual ~Bar(){
	};

	void setBarImg(int barImgId, int frameImgId){
		bar->setImgId(barImgId);
		frame->setImgId(frameImgId);
	}
	
	void setBarRefer(BodyFP getValue, BodyFP getMax){
		this->getValue = getValue;
		this->getMax = getMax;
	}
	
	void setBarValue(float value, float max){
		//系統用Bar，不使用Body的函式指標，需要直接值
		this->value = value;
		this->max = max;
	}

	void setBarSize(float barLeft, float barTop,float barWidth,float barHeight){
		this->barLeft = barLeft;
		this->barTop = barTop;
		this->barWidth = barWidth;
		this->barHeight = barHeight;
	}

	void setFixed(bool fixed){
		bar->setFixed(fixed);
		frame->setFixed(fixed);
	}

	virtual void draw(Drawer *drawer){
		float valWidth = bound((barWidth+barLeft)*(value/max) - barLeft, -barLeft, barWidth);
		bar->setImage(barLeft, barTop, valWidth, barHeight, 0);
		frame->setImage(barLeft, barTop, barWidth, barHeight, 0);
		bar->draw(drawer);
		frame->draw(drawer);
	}


protected:
	Body *refer;
	float barLeft;
	float barTop;
	float barWidth;
	float barHeight;
	float value;
	float max;

	Image *frame;
	Image *bar;
	BodyFP getValue;
	BodyFP getMax;

	virtual void mainProc(){
		if(refer!=NULL){
			value = (float)(refer->*getValue)();
			max = (float)(refer->*getMax)();
			if(refer->isFinished()){
				bar->finish();
				frame->finish();
				finish();
			}
		}
		bar->main();
		frame->main();
	}
};

class HpBar : public Bar{
public:
	HpBar(Body *refer, Object *target): Bar(refer, target){
		setBarImg(1100, 1101);
		setBarRefer(&Body::getHp, &Body::getMaxHp);
	}
	virtual ~HpBar(){
	}
};

class PlayerPanel : public Object{
public:
	PlayerPanel(Drawer *drawer, Body *me){
		ID3DXFont *font;
		D3DXCreateFont(drawer->getDevice(), 46, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &font);   
	
		show = true;
		group = new Image(300);
		group->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/10);
		group->setImage(SCREEN_WIDTH/2, SCREEN_HEIGHT/10, SCREEN_WIDTH/2, SCREEN_HEIGHT/10, 0);
		group->setFixed(true);

		hpBar = new HpBar(me, group);
		hpBar->setBarSize(100, 10, 100, 10);
		hpBar->setParent(group, -200, 0);
		hpBar->setFixed(true);
		
		expBar = new Bar(NULL, group);
		expBar->setBarImg(1104, 1105);
		expBar->setBarSize(100, 10, 100, 10);
		expBar->setParent(group, -200, -30);
		expBar->setFixed(true);

		level = new Text();
		level->setText(50, 20, 50, 20);
		level->setParent(group, 180, 15);
		level->setFixed(true);
		level->setFont(font);
		
		exp = new Text();
		exp->setText(100, 10, 100, 10);
		exp->setParent(group, 200, -15);
		exp->setFixed(true);
		exp->setFont(font);

		onFlag(RUNNABLE | DRAWABLE);
		setZ(0.55);
	}
	virtual ~PlayerPanel(){
	}


	virtual void draw(Drawer *drawer){
		group->draw(drawer);
		level->draw(drawer);
		exp->draw(drawer);
	}
	virtual void setAvatar(Avatar *avatar){
		this->avatar = avatar;
	}


protected:
	bool show;
	Image *group;
	HpBar *hpBar;
	Bar *expBar;
	Text *level;
	Text *exp;
	Avatar *avatar;

	virtual void mainProc(){

		if(state==0){
		}
		else if(state==1){
			//hiding
			if(time<100){
				group->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/10-time*SCREEN_HEIGHT/100);
			}
		}

		group->main();
		
		expBar->setBarValue(avatar->getExp(), avatar->getMaxExp());
		
		stringstream ss;
		ss<<"LV "<<avatar->getLevel()<<endl;
		level->setWord(ss.str());
		level->main();
		ss.str("");
		ss<<"EXP "<<avatar->getExp()<<"/"<<avatar->getMaxExp();
		exp->setWord(ss.str());
		exp->main();
	}
};