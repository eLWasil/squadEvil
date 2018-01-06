#include "bush.h"

bush::bush() : numberOfTex((rand() % 4))
{
	sprite.setPosition(0, 0);
	
	textures[0].loadFromFile("data/Graphics/Others/Object/Bush (1).png");
	textures[1].loadFromFile("data/Graphics/Others/Object/Bush (2).png");
	textures[2].loadFromFile("data/Graphics/Others/Object/Bush (3).png");
	textures[3].loadFromFile("data/Graphics/Others/Object/Bush (4).png");

	texture = textures[numberOfTex];
	sprite.setTexture(texture);

	layer = Layer::FRONT;
}


bush::~bush()
{
}
