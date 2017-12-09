#pragma once
#include "skills.h"
using namespace sf;
class accessories;

class fireball : public skills
{
public:
	fireball(float x, float y, int dir = 1);
	fireball(Vector2f position, int dir = 1);
	~fireball();

	virtual Sprite getSprite();
	virtual int update(accessories* frontObject);

private:
	void build();

	Sprite currentSprite, current;
	//basicStats basics;
	Clock fpsTimer;


	float dist;
	float speed;
	int dirent;
};

