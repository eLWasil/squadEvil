#include "bush.h"



bush::bush(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
	layer = Layer::FRONT;
}


bush::~bush()
{
}
