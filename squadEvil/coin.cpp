#include "coin.h"


coin::coin(sf::Sprite obj, string type) : block(false)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
	increase = 2;
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
		if (!block)
		{
			Player++;
			block = true;
		}
		//std::cout << name << std::endl;
	}
}
void coin::drawable()
{
	float posY = sprite.getPosition().y - increase;
	increase *= 1.15;

	if (posY > position.y - 252)
	{
		sprite.setPosition(sprite.getPosition().x, posY);
		int k = (255 - (increase * 9.f));
		sprite.setColor(sf::Color(255, 255, 255, k < 0 ? 0 : k));
	}
}