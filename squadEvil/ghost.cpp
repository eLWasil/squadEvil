#include "ghost.h"



ghost::ghost(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
}


ghost::~ghost()
{
}

void ghost::eventP(player*) {}