#include "coin.h"

/*
coin::coin(sf::Sprite obj, string type) : block(false)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
	hightOfFly = 2;
}
*/

coin::coin()
{
	sprite.setPosition(0, 0);
	hightOfFly = 2;
	texture.loadFromFile("data/Graphics/Others/Object/coin.png");
	sprite.setTexture(texture);
}

coin::~coin()
{
}

void coin::eventP(player &Player)
{
	//cout << name << " posX: " << sprite.getPosition().x << endl;
	if (Player.getConstSprite().getGlobalBounds().intersects(sprite.getGlobalBounds()))
	{
		this->remove = true; time.restart();
		if (!blockMutex)
		{
			Player++;
			blockMutex = true;
		}
		//std::cout << name << std::endl;
	}
}
void coin::drawable()
{
	float posY = sprite.getPosition().y - hightOfFly;
	hightOfFly *= 1.15;

	if (posY > sprite.getPosition().y - 252)
	{
		sprite.setPosition(sprite.getPosition().x, posY);
		int k = (255 - (hightOfFly * 9.f));
		sprite.setColor(sf::Color(255, 255, 255, k < 0 ? 0 : k));
	}
}