#pragma once
#include "enemies.h"
class ghost :
	public enemies
{
public:
	ghost(sf::Sprite sprite, string type);
	~ghost();

	virtual void eventP(player *);
};

