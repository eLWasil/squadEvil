#pragma once
#include "enemies.h"
class chicken :
	public enemies
{
public:
	chicken(sf::Sprite sprite, string type);
	~chicken();
	virtual void eventP(player &);
	virtual bool update(); 

private:
	int maxHP, currentHpOnBar;
	float range;
	int speed;
	bool changeDir;
	float angleIncrease;

	bool reverse;
	bool runAway;
	bool targetChanging;
	int angle;
	float degree;
	bool DMG;

	Clock timer, hittimer;
	Vector2f target;
	float attackSpeed;
	float angleSpeed;
	float flySpeed;
};

