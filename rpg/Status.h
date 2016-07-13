#pragma once

class Status{

public:
	Status(int swordSkill){
		setSwordSkill(swordSkill);
	}
	virtual ~Status(){

	}
	void setSwordSkill(int swordSkill){
		this->swordSkill = swordSkill;
		maxPAtk = 10 + (int)(swordSkill*0.8);
		minPAtk = (int)(swordSkill*0.4);
	}
	
	int getMaxPAtk(){
		return maxPAtk;
	}
	int getMinPAtk(){
		return minPAtk;
	}

protected:
	int swordSkill;

	int maxPAtk;
	int minPAtk;
	

};