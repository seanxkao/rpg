#pragma once

#include "Avatar.h"
#include "Body.h"
#include "Text.h"

typedef int (Body::*BodyFP)();

class Bar : public Component{
public:

	Bar(Body*, Object*);
	virtual ~Bar();

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
		bar->setImage(barLeft, barTop, valWidth, barHeight, 0, 0);
		frame->setImage(barLeft, barTop, barWidth, barHeight, 0, 0);

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
		}
		if(refer!=NULL){
			if(refer->isFinished()){
				finish();
			}
		}
		bar->main();
		frame->main();
		
	}
};


Bar::Bar(Body *refer, Object *target) :
	Component(){
	this->refer = refer;

	frame = new Image();
	bar = new Image();
	frame->setTarget(this, 0, 0);
	bar->setTarget(this, 0, 0);

	setTarget(target, 0, 0);
};

Bar::~Bar(){
};

class HpBar : public Bar{
public:
	HpBar(Body*, Object*);
	~HpBar();
};
HpBar::HpBar(Body *refer, Object *target) :
	Bar(refer, target){
	setBarImg(1100, 1101);
	setBarRefer(&Body::getHp, &Body::getMaxHp);
}
HpBar::~HpBar(){
}




class PlayerPanel : public Component{
public:
	PlayerPanel(Body *me){
		show = true;
		group = new Image(300);
		group->setPosition(SCREEN_WIDTH/2, SCREEN_HEIGHT/10);
		group->setImage(SCREEN_WIDTH/2, SCREEN_HEIGHT/10, SCREEN_WIDTH/2, SCREEN_HEIGHT/10, 0, 0);
		group->setFixed(true);

		hpBar = new HpBar(me, group);
		hpBar->setBarSize(100, 10, 100, 10);
		hpBar->setTarget(group, -200, 0);
		hpBar->setFixed(true);
		
		expBar = new Bar(NULL, group);
		expBar->setBarImg(1104, 1105);
		expBar->setBarSize(100, 10, 100, 10);
		expBar->setTarget(group, -200, -30);
		expBar->setFixed(true);

		level = new Text();
		level->setText(50, 20, 50, 20);
		level->setTarget(group, 180, 15);
		level->setFixed(true);

		
		exp = new Text();
		exp->setText(100, 10, 100, 10);
		exp->setTarget(group, 200, -15);
		exp->setFixed(true);

		patk = new Text();
		patk->setText(50, 10, 50, 10);
		patk->setTarget(group, -200, 10);
		patk->setFixed(true);
	}
	virtual ~PlayerPanel(){
	}


	virtual void draw(Drawer *drawer){
		group->draw(drawer);
		hpBar->draw(drawer);
		expBar->draw(drawer);
		level->draw(drawer);
		exp->draw(drawer);
		patk->draw(drawer);
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
	Text *patk;
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
		hpBar->main();
		expBar->setBarValue(avatar->getExp(), avatar->getMaxExp());
		expBar->main();

		stringstream ss;

		ss<<"LV "<<avatar->getLevel()<<endl;
		level->setWord(ss.str());
		level->main();

		ss.str("");
		ss<<"EXP "<<avatar->getExp()<<"/"<<avatar->getMaxExp();
		exp->setWord(ss.str());
		exp->main();

		//ss.clear();
		//ss<<"PATK "<<avatar->get <<"/"<<avatar->getMaxExp();
		//ss>>test;
		//exp->setWord(test);
		//patk->main();
	}
};