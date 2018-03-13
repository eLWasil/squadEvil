#include "chest.h"




chest::chest()
{
	isOpen = false;

	sprite.setPosition(0, 0);
	texture.loadFromFile("data/Graphics/Others/Objects/chests.png", sf::IntRect(0, 0, 32, 32));
	openTex.loadFromFile("data/Graphics/Others/Objects/chests.png", sf::IntRect(32, 0, 32, 32));

	sprite.setTexture(texture);
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