#include "box.h"



box::box()
{
	sprite.setPosition(0, 0);

	texture.loadFromFile("data/Graphics/Others/Objects/box.png");
	sprite.setTexture(texture);

}


box::~box()
{
}

void box::eventP(player &Player)
{
	
}