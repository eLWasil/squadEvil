#pragma once
#include "enemies.h"
class flower :
	public enemies
{
public:
	flower(sf::Sprite sprite, string type);
	~flower();

	virtual void eventP(player *);
};

