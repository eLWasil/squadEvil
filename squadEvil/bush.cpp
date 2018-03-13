#include "bush.h"

bush::bush() 
{
	sprite.setPosition(0, 0);
	
	texture.loadFromFile("data/Graphics/Others/Objects/bush.png");
	sprite.setTexture(texture);

	layer = Layer::FRONT;
}


bush::~bush()
{
}
