#include "ladder.h"



ladder::ladder(sf::Sprite obj, string type)
{
	sprite = obj;
	name = type;
	position = obj.getPosition();
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
	if (this->sprite.getGlobalBounds().intersects(Player.getConstSprite().getGlobalBounds()))
	{
		Player.sigLadderReset();
		//cout << "\nLadder reset.";
	}
	//cout << "\nLadder out.";
}
