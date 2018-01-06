#pragma once
#include "accessories.h"
class bush :
	public accessories
{
public:
	bush();
	~bush();

private:
	sf::Texture textures[4];
	const int numberOfTex;
};

