#pragma once
#include "accessories.h"
class coin :
	public accessories
{
public:
	//coin(sf::Sprite sprite, string type);
	coin();
	~coin();

	virtual void eventP(player &);
	virtual void drawable();

private:
	float hightOfFly;
	bool blockMutex;
};

