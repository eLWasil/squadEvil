#include "potions.h"



potions::potions(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
}


potions::~potions()
{
}
void potions::eventP(player*) {}