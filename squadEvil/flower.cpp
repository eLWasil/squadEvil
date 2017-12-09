#include "flower.h"



flower::flower(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
}


flower::~flower()
{
}

void flower::eventP(player*) {}