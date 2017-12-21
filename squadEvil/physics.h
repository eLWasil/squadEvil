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
	void jump(player *p);
private:
	map_level* map;
	Vector2f controlPoint;

	/* Gravity */
	Clock gravitySpeedTimer;
	float gravitationPower;

	/* Move */
	float maxMoveSpeed, acceleration;
	Clock moveAccTimer;

	/* Jump */
	Clock jumpTimer;
	bool isJumping;
};

