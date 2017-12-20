#include "physics.h"
#define TILESIZE 64


physics::physics()
{
	this->map = map;
	gravitationPower = 3;
	controlPoint = Vector2f(0, 0);
	maxMoveSpeed = 12;
	acceleration = maxMoveSpeed / 1000; // maxMoveSpeed / seconds of speeding
}

physics::~physics()
{
}

void physics::setLevel(map_level *map)
{
	this->map = map;
}

void physics::gravity(player *p)
{
	static int k = 0;
	int timer = gravitySpeedTimer.getElapsedTime().asMilliseconds();
	int mod = gravitationPower + ((timer > 1000 ? 1000 : timer) / 50);
	Vector2f newPosition = Vector2f(p->getPosition().x, p->getPosition().y + mod);
	Vector2i tileOnNewPosition(p->getCorner(player::physicsPoints::CENTER).x / TILESIZE, (p->getCorner(player::physicsPoints::RIGHT_BOT).y + mod) / TILESIZE);

	if (map->tileMap[tileOnNewPosition.x][tileOnNewPosition.y] == 0)
	{
		if (newPosition.y > (map->tileMap.size() - 5) * TILESIZE)
		{
			p->setPosition(controlPoint);
		}
		else
		{
			p->setPosition(newPosition);
		}
	}
	else
	{
		gravitySpeedTimer.restart();
		//isJump = false;


		//jumpingCounter = 2;
	}
}

void physics::tileCollisions(player *p)
{
	
	int cDir = p->currentDirState - 1; //Look player::dir 
	Vector2f position = p->getPosition();
	static float currentMoveSpeed = 1;

	currentMoveSpeed = currentMoveSpeed > maxMoveSpeed ? maxMoveSpeed : (acceleration * moveAccTimer.getElapsedTime().asMilliseconds());
	//cout << acceleration << " * " << moveAccTimer.getElapsedTime().asMilliseconds() << " = " << currentMoveSpeed << endl;

	if (cDir == 0)
	{
		currentMoveSpeed = 0;
		moveAccTimer.restart();
	}
	else
	{
		if (cDir < 0)
		{
			currentMoveSpeed *= -1;
			if (p->getSprite().getScale().x > 0)
			{
				p->getSprite().setScale(p->getSprite().getScale().x * -1, p->getSprite().getScale().y);
			}
		}
		else if (cDir > 0)
		{
			if (p->getSprite().getScale().x < 0)
			{
				p->getSprite().setScale(p->getSprite().getScale().x * -1, p->getSprite().getScale().y);
			}
		}

		Vector2f newPosition = Vector2f(p->getPosition().x + currentMoveSpeed, p->getPosition().y);
		if (newPosition.x < 0)
		{
			newPosition.x = 0;
		}
		else if (newPosition.x > map->getMapSizeX())
		{
			currentMoveSpeed = 0;
		}

		int tileTypeAfterMove = map->getTileType(newPosition);

		if (tileTypeAfterMove == 0)
		{
			p->setPosition(newPosition);
		}
	}
}