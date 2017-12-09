#include "campFire.h"



campFire::campFire(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
	for (size_t i = 0; i < 5; i++)
	{
		texs[i].loadFromFile("data/Graphics/Others/Object/CampFire.png", IntRect(64 * i, 0, 64, 64));
	}
	fireCounter = 0;
}


campFire::~campFire()
{
}

void campFire::eventP(player*) {}

bool campFire::update()
{
	if (counter.getElapsedTime().asMilliseconds() > 120)
	{
		fireCounter = (fireCounter + 1) % 5;
		sprite.setTexture(texs[fireCounter]);
		counter.restart();
	}
	return false;
}