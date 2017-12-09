#include "turtle.h"



turtle::turtle(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
}


turtle::~turtle()
{
}

void turtle::eventP(player*) {}