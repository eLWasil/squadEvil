#pragma once
#include "accessories.h"
class chest :
	public accessories
{
public:
	chest(sf::Sprite sprite, string type);
	~chest();

	virtual void eventP(player &);

private:
	bool isOpen;
	Texture openTex;
};

