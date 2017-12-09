#pragma once
#include "accessories.h"
class box :
	public accessories
{
public:
	box(sf::Sprite sprite, string type);
	~box();

	virtual void eventP(player &);

};

