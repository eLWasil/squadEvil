#include "accessories.h"


accessories::accessories() : remove(false), layer(Layer::BACK)
{
}


accessories::~accessories()
{
}

bool accessories::update()
{
	//cout << "accessories::getName() is working\n";
	//cout << remove << endl;
	if (remove)
	{
		drawable();
		Time t1 = time.getElapsedTime();
		if (t1.asSeconds() >= 1.2f)
		{
			return true;
		}
	}
	return false;
}

void accessories::setPosition(Vector2f newPosition)
{
	sprite.setPosition(newPosition);
}