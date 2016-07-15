#include "Body.h"
#include "Controller.h"


class PlayerCharacter: public Body{
public:
	int revive;
	PlayerCharacter(Controller*);
	PlayerCharacter();
	~PlayerCharacter();

	void setController(Controller *controller){
		this->controller = controller;
	}
	void setAvatar(Avatar *avatar){
		this->avatar = avatar;
	}
	void moveBody(){
		float direction = controller->getDirection();
		if(direction>=0){
			this->direction = direction;
			Vector2D *velocity = Vector2D::polar(8, direction);
			x += velocity->getX();
			y += velocity->getY();
		}

		if(x < FRAME_LEFT + 50){
			x = FRAME_LEFT + 50;
		}
		if(x > FRAME_RIGHT - 50){
			x = FRAME_RIGHT	- 50;
		}
		if(y > FRAME_UP	- 60){
			y = FRAME_UP - 60;
		}
		if(y < FRAME_DOWN + 60){
			y = FRAME_DOWN	+ 60;
		}
	}

	virtual void draw(Drawer *drawer){
		switch(state){
			case STATE_NORMAL:
				setImage(50,50,50,50,0);
				if(time%16 == 0){
					setImgId(1000);
				}
				else if(time%16 == 4){
					setImgId(1001);
				}
				else if(time%16 == 8){
					setImgId(1000);
				}
				else if(time%16 == 12){
					setImgId(1002);
				}
				break;
			case STATE_DAMAGED:
				setImage(50,50,50,50,0);
				break;
			case STATE_DISAPPEAR:
				if(time<25){
					imgRad+=17;
					setAlpha(255-time*10);
					setImage(30+time*7, 30+time*7, 30+time*7, 30+time*7, imgRad);
				}
				else{
					setAlpha(0);
				}
				break;
		}
		Body::draw(drawer);
	}
	virtual void mainStart(){
		bdyEnable = false;
		atkEnable = false;
		Body::mainStart();
	}

	virtual void stateStart(){
		if(state==0){
			setSpeed(3, 0, 0);
		}
		else if(state==1){
			setSpeed(3, direction, 1);
		}
	}
	
	virtual void mainProc(){
		if(state==0){
			setBody(BDY_NORMAL, 20);
			moveBody();
			if(controller->getNormalAtk()){
				setState(1);
			}
			if(hp<0)setState(99);
		}

		else if(state==1){
			setBody(BDY_NORMAL, 25);
			if(time==0){
				SwordShadow *swordShadow = new SwordShadow(200, 20, 80, 255);
				swordShadow->setParent(this, 0, 0);
				if(rand()<RAND_MAX/2){
					swordShadow->wield(5, direction - 100, direction + 100);
				}
				else{
					swordShadow->wield(5, direction + 100, direction - 100);
				}
			}
			else if(time==1){
				status = avatar->getStatus();
				int atk = randomRange(status->getMinPAtk(), status->getMaxPAtk());
				setAttack(atk, 0, 4, 100, direction-60, direction+60);
			}
			else if(time > 4){
				setState(0);
			}
		}
		else if(state==10){
			if(time>4){
				setState(0);
			}
		}

		else if(state==99){
		}
		
		revive++;
		if(revive>=60){
			if(hp<maxHp)hp++;
			revive = 0;
		}

		Body::mainProc();
	}

protected:
	Controller *controller;
	Avatar *avatar;
};

PlayerCharacter::PlayerCharacter(Controller *controller): Body(){
	setController(controller);
	setImage(30, 30, 30, 30, 0);
	imgId = 1000;
	maxHp = 100;
	hp = 100;
	revive = 0;
	status = new Status(10);
	setZ(0.5);
}


PlayerCharacter::~PlayerCharacter(){
}
