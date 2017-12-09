#pragma once
#include "accessories.h"
class campFire :
	public accessories
{
public:
	campFire(sf::Sprite sprite, string type);
	~campFire();

	virtual void eventP(player *);
	virtual bool update();

private:
	Texture texs[5];
	int fireCounter;
	Clock counter;
};

