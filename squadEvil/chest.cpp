#include "chest.h"



chest::chest(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
	isOpen = false;
	layer = Layer::BACK;

	openTex.loadFromFile("data/Graphics/Others/Object/chests.png", sf::IntRect(32, 0, 32, 32));
}


chest::~chest()
{
}

void chest::eventP(player &Player)
{
	if (!isOpen)
	{
		if (sprite.getGlobalBounds().intersects(Player.getConstSprite().getGlobalBounds()))
		{
			int randGold = rand() % 10;
			randGold += 5;
			for (size_t i = 0; i < randGold; i++)
			{
				Player++;
			}
			isOpen = true;
			sprite.setTexture(openTex);
		}
	}
}