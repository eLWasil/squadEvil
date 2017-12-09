#pragma once
#include "accessories.h"
class coin :
	public accessories
{
public:
	coin(sf::Sprite sprite, string type);
	~coin();

	virtual void eventP(player &);
	virtual void drawable();

private:
	float increase;
	bool block;
};

