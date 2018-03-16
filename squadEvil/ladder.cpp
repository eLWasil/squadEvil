
#include "ladder.h"



ladder::ladder()
{
	sprite.setPosition(0, 0);

	texture.loadFromFile("data/Graphics/Others/Objects/ladder.png");
	sprite.setTexture(texture);
}


ladder::~ladder()
{
}

void ladder::eventP(player &Player)
{
	/*
	cout << "Is in ladder \n";
	if (sprite.getGlobalBounds().intersects(Player->getConstSprite().getGlobalBounds()))
	{
		if (Player->isJumping())
		{
			Vector2f pos(Player->getPosition());
			Player->setPosition(pos.x, pos.y + 10);
		}
		Player->stopGravity();
		cout << "Is in ladder \n";
	}
	else
	{
		Player->startGravity();
	}
	*/

	//cout << "\nLadder in.";
	if (this->sprite.getGlobalBounds().intersects(Player.getSprite().getGlobalBounds()))
	{
		//Player.sigLadderReset();
		//cout << "\nLadder reset.";
	}
	//cout << "\nLadder out.";
}
