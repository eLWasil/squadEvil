#pragma once
#include "map_level.h"
#include "player.h"

class physics
{
public:
	physics();
	~physics();

	void setLevel(map_level *);
	void gravity(player* p);
	void tileCollisions(player* p);
private:
	map_level* map;
	Vector2f controlPoint;

	Clock gravitySpeedTimer;
	float gravitationPower;

	float maxMoveSpeed, acceleration;
	Clock moveAccTimer;
};

