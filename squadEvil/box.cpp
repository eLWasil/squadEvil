#include "box.h"



box::box(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();

}


box::~box()
{
}

void box::eventP(player &Player)
{
	
}