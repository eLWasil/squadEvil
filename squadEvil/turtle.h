#pragma once
#include "enemies.h"
class turtle :
	public enemies
{
public:
	turtle(sf::Sprite sprite, string type);
	~turtle();

	virtual void eventP(player *);
};

