#pragma once
#include "accessories.h"
class ladder :
	public accessories
{
public:
	ladder(sf::Sprite sprite, string type);
	~ladder();
	virtual void eventP(player &);
};

