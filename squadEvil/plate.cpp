#include "plate.h"



plate::plate()
{
	sprite.setPosition(0, 0);
	texture.loadFromFile("data/Graphics/Others/Object/Plate.png");
	sprite.setTexture(texture);
}


plate::~plate()
{
}

void plate::eventP(player &p)
{
	if (sprite.getGlobalBounds().intersects(p.getSprite().getGlobalBounds()))
	{
		p.fEndOfLevel = true;
	}
}