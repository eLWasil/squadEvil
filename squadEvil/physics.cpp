#include "physics.h"
#include <math.h>
#define TILESIZE 64


physics::physics()
{
	this->map = map;
	gravitationPower = 3;
	controlPoint = Vector2f(0, 0);
	maxMoveSpeed = 6;
	acceleration = maxMoveSpeed / 500; // maxMoveSpeed / seconds of speeding
}

physics::~physics()
{
}

void physics::setLevel(map_level *map)
{
	this->map = map;
}

bool physics::groudChecker(player *p)
{
	//tileonplayer
	Vector2f groudTilePos(p->getPosition().x, p->getCorner(player::physicsPoints::LEFT_BOT).y + 6);
	int tileType = map->getTileType(groudTilePos);

	if (tileType != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void physics::gravity(player *p)
{
	if (p->getJumpingVariable())
	{
		// -(x^2) +16 // -4 ... 4
		const int timeOfJumping = 600; // as Milisecons
		
		if (jumpTimer.getElapsedTime().asMilliseconds() < timeOfJumping)
		{
			const int stepCount = 8;
			float step = timeOfJumping / stepCount;
			float x = (jumpTimer.getElapsedTime().asMilliseconds() / step) - (stepCount / 2);
			float y = (-(pow(x, 2.)) + 16)*8;

			Vector2f newPos = Vector2f(p->getPosition().x, p->startingPositionY - y);

			if (x > 0) // it mean player is going down
			{
				p->endJump();
			}
			else
			{
				p->setPosition(newPos);
			}
		}
		else
		{
			p->endJump();
		}
	}
	else
	{
		jumpTimer.restart();

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
			p->currentMoveDir = player::dir::BACK;
			currentMoveSpeed *= -1;
			if (p->getSprite().getScale().x > 0)
			{
				p->getSprite().setScale(p->getSprite().getScale().x * -1, p->getSprite().getScale().y);
			}
		}
		else if (cDir > 0)
		{
			p->currentMoveDir = player::dir::FORWARD;
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

void physics::jump(player *p)
{
	//Jumping moved to gravity


}