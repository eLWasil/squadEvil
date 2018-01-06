#include "ghost.h"



ghost::ghost(sf::Sprite obj, string type)
{
	sprite = obj;
}


ghost::~ghost()
{
}

void ghost::eventP(player*) {}