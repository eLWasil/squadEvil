#include "accessories.h"


accessories::accessories() : remove(false), layer(Layer::BACK)
{
}


accessories::~accessories()
{
}

string accessories::getName() { return name; }

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

