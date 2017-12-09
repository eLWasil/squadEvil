#pragma once
#include "player.h"
#include "skillsBook.h"

class Mage :
	public player
{
public:
	Mage(std::string name);
	~Mage();

	virtual const int getGold() { return currentStats.gold; };
	virtual const float getHP() { return currentStats.HP; };
	virtual const float getMANA() { return currentStats.MANA; };
	virtual const float getEXP() { return currentStats.EXP; };
	virtual const int getMaxHp() { return currentStats.max_hp; };
	virtual const int getMaxMana() { return currentStats.max_mana; };

	virtual skills* getSkill(int nr);
	virtual void regen();
	//virtual skills* getSkill(int nr) { return (new fireball(getConstSprite().getPosition())); };
};

