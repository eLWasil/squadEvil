#include "warrior.h"



warrior::warrior()
{
	sprite.setPosition(0, 0);
	texture.loadFromFile("data/Graphics/Others/enemies/warrior.png");
	sprite.setTexture(texture);

	enemie.range = 256;

	enemie.speed = 2.5;
	enemie.range = 320;
	enemie.dir = Vector2f(1, 0);
	degree = 0;
}


warrior::~warrior()
{
}


void warrior::setMap(std::vector <std::vector <int>> &level)
{
	map = level;
	setLimits();
}

void warrior::setLimits()
{
	Vector2f tileUnder((sprite.getPosition().x + (sprite.getGlobalBounds().width / 2)) / 64, (sprite.getPosition().y + sprite.getGlobalBounds().height + 2) / 64);
	//cout << map[tileUnder.x][tileUnder.y] << endl;
	int rangeInTile = enemie.range / 64;

	bool leftSide = true, rightSide = true;
	for (size_t i = 1; i < rangeInTile; i++)
	{
		if (leftSide)
		{
			if (map[tileUnder.x - i][tileUnder.y] == 0)
			{
				walkLimits.x = (tileUnder.x - i) * 64;
				leftSide = false;
			}
			else if (map[tileUnder.x - i][tileUnder.y - 1] != 0)
			{
				walkLimits.x = (tileUnder.x - i) * 64;
				leftSide = false;
			}
			else
			{
				walkLimits.x = (tileUnder.x - i) * 64;
			}
		}
		if (rightSide)
		{
			if (map[tileUnder.x + i][tileUnder.y] == 0)
			{
				rightSide = false;
			}
			else if (map[tileUnder.x + i][tileUnder.y - 1] != 0)
			{
				rightSide = false;
			}
			else
			{
				walkLimits.y = ((tileUnder.x + i) * 64) - 64;
			}
		}
	}
}

void warrior::eventP(player&) {}

bool warrior::update()
{
	
	Vector2f newPosition;
	newPosition.x = sprite.getPosition().x + (enemie.speed * enemie.dir.x);
	newPosition.y = sprite.getPosition().y + (enemie.speed * enemie.dir.y);
	if (newPosition.x >= walkLimits.y)
	{
		if (!isStanding)
		{
			standTimer.restart();
		}
		isStanding = true;
		if (standTimer.getElapsedTime().asMilliseconds() > 1000)
		{
			enemie.dir = Vector2f(-1, 0);
			isStanding = false;
		}
		else
		{
			enemie.dir = Vector2f(0, 0);
		}
	}
	else if (newPosition.x <= walkLimits.x)
	{
		if (!isStanding)
		{
			standTimer.restart();
		}
		isStanding = true;
		if (standTimer.getElapsedTime().asMilliseconds() > 1000)
		{
			enemie.dir = Vector2f(1, 0);
			isStanding = false;
		}
		else
		{
			enemie.dir = Vector2f(0, 0);
		}
	}
	sprite.setPosition(newPosition);
	//cout << (enemie.speed * enemie.dir.x) << "  lim.x. -> " << walkLimits.y << endl;

	return false;
}