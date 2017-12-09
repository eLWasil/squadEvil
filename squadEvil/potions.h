#pragma once
#include "accessories.h"

class potions :
	public accessories
{
public:
	potions(sf::Sprite sprite, string type);
	~potions();

	virtual void eventP(player *);
};

