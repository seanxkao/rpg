#pragma once

#include "Status.h"

class Avatar{
public:
	Avatar(){
		level = 1;
		maxExp = 10 + 2 * (level * level);
		exp = 0;

		status = new Status(10);

	}

	int getLevel(){
		return level;
	}

	int getExp(){
		return exp;
	}

	Status* getStatus(){
		return status;
	}
	
	void addExp(int add){
		exp+=add;
		if(exp>=maxExp){
			level++;
			exp = exp-maxExp;
			maxExp = 10 + 2 * (level * level);
			status->setSwordSkill(10 + level*5);
		}
	}

	int getMaxExp(){
		return maxExp;
	}

protected:
	int	level;
	int	exp;
	int	maxExp;
	int	maxHp;
	int	maxMp;

	Status *status;
};
