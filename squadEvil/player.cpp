#include "player.h"
#include <math.h>
#include <iostream>

player::player() : isJump(false), currentState(dir::STOP), currentDir(dir::FORWARD), texWidth(84), texHeight(64), hud(*this), defaultSpeed(0.6)
{
	avatarTex.loadFromFile("data/Graphics/Avatars/tileset_mage.png", IntRect(0, 0, texWidth, texHeight));
	avatarBackTex.loadFromFile("data/Graphics/Avatars/tileset_mage.png", IntRect(0, 64, texWidth, texHeight));
	//avatarTex.loadFromFile("data/Avatars/new mage.png"); 
	avatar.setTexture(avatarTex);
	avatar.setPosition(0, 0);

	jumpingCounter = 2;

	

	hitColorCounter = 0;
	gravitation = 4;
	currentSpeed = defaultSpeed;
	gravityState = true;

	ladderTime.restart();
	skillBar[1] = skillBar[2] = skillBar[3] = 1;
}


player::~player()
{
}

void player::setMap(std::vector <std::vector <int>> &level)
{
	map = level;
}


Vector2f player::getPosition()
{
	return avatar.getPosition();
}

Vector2f player::getCenter()
{
	Vector2f center = avatar.getPosition();
	center.x += avatar.getGlobalBounds().width / 2;
	center.y += avatar.getGlobalBounds().height / 2;
	return center;
}

const Sprite &player::getConstSprite()
{
	return this->avatar;
}

void player::hitEffect()
{
	avatar.setColor(Color(0, 0, 0));
	hitColorTimer.restart();
	avatar.setPosition(avatar.getPosition().x - 5, avatar.getPosition().y - 10);
}

void player::hudEffect(RenderWindow &window)
{
	hud.update();
	window.draw(hud.statistic[0]);
	window.draw(hud.statistic[1]);
	window.draw(hud.statistic[2]);

}

void player::drawCorners(RenderWindow &window)
{
	for (size_t i = 0; i < 4; i++)
	{
		CircleShape point;
		point.setFillColor(Color(0, 0, 0));
		point.setPosition(corners[i]);
		point.setRadius(2);
		window.draw(point);

	}
}

void player::setCorners()
{
	Vector2f oldPosition = avatar.getPosition();
	/*CORNERS*/ //84x 64y
	corners[0].x = oldPosition.x + 4;
	corners[0].y = oldPosition.y + 4;
	corners[1].x = (oldPosition.x + texWidth) - 20;
	corners[1].y = oldPosition.y + 4;
	corners[2].x = (oldPosition.x + texWidth) - 20;
	corners[2].y = (oldPosition.y + texHeight) - 6;
	corners[3].x = oldPosition.x + 4;
	corners[3].y = (oldPosition.y + texHeight) - 6;
}

void player::update()
{
	regen();

	Vector2f oldPosition = avatar.getPosition();
	setCorners();

	if (oldPosition.y + gravitation >= (map[0].size() - 1) * 64)
	{
		avatar.setPosition(0, 0);
		gravitation = 4;
	}

	if (isJump && jumpingCounter > 0)
	{
		jumping();
	}
	else
	{
		if (gravityState)
		{
			gravity();
		}
		/*else
		{
			if (getPosition().x + (avatar.getGlobalBounds().width / 2) < ladderParamsX.x)
			{
				startGravity();
				ladderParamsX = Vector2f(0, 0);
			}
			else if (getPosition().x > ladderParamsX.y)
			{
				startGravity();
				ladderParamsX = Vector2f(0, 0);
			}
		}*/
	}
	moving();

	if (hitColorTimer.getElapsedTime().asMilliseconds() > 300 && hitColorTimer.getElapsedTime().asMilliseconds() < 400)
	{
		avatar.setColor(Color(255, 255, 255));
	}

	//std::cout << int(avatar.getPosition().y / 64) + 1 << std::endl;
}

Sprite &player::getSprite()
{
	return avatar;
}

void player::gravity()
{
	static int k = 0;
	Vector2i tileOnPlayer((avatar.getPosition().x + (corners[2].x - corners[3].x) / 2) / 64, (corners[2].y + gravitation + 6) / 64);
	float dist = ((tileOnPlayer.y + 1) * 64) - corners[2].y;
	//if (map[tileOnPlayer.x][tileOnPlayer.y + 1] == 0)
	//printf("{player::gravity()} TileMap: %d(%d, %d), [%d]\n", map[tileOnPlayer.x][tileOnPlayer.y], tileOnPlayer.x, tileOnPlayer.y, (++k));
		//std::cout << "" <<  << std::endl;
	if(map[tileOnPlayer.x][tileOnPlayer.y] == 0)
	{
		//std::cout << dist << std::endl;

		//std::cout << "gravity = " << gravity << " / 2 * " << sec << "*" << sec << " = " << drop << std::endl;

		avatar.setPosition(Vector2f(avatar.getPosition().x, avatar.getPosition().y + gravitation)); 
		if (gravitation > 0)
		{
			gravitation *= (gravitation > 32 ? 1 : 1.1);

		}
		else
		{
			gravitation = 4;
		}

		/*
		setCorners();
		Vector2i tileOnPlayer(((corners[2].x - corners[3].x) / 2) / 64,  ((corners[2].y - corners[1].y) / 2) / 64);
		if (map[tileOnPlayer.x][tileOnPlayer.y + 1] != 0)
		{
			if (avatar.getPosition().y > tileOnPlayer.y * 64)
			{
				avatar.setPosition(avatar.getPosition().x, tileOnPlayer.y * 64);
			}
			gravitation = 4;
		}
		*/
		//std::cout << gravity << std::endl;
	}
	else
	{
		avatar.setPosition(Vector2f(avatar.getPosition().x, (tileOnPlayer.y - 1) * 64));
		static int reflecCounter = 0;
		gravitation = 4;


			/*
			if (reflecCounter == 0)
			{
				//reflecCounter = 0;
			}
			else
			{
				timeZero.restart();
				avatar.setPosition(avatar.getPosition().x, avatar.getPosition().y - ((2 - reflecCounter) * 16));
				reflecCounter++;
			}
			*/
		jumpingCounter = 2;
	}
}


void player::moving()
{
	Vector2f position = avatar.getPosition();
	/*
	static float currentMove = 0;
	int DIR = currentState - 1;

	int expSpeed = DIR * 8;
	const float changing = 0.6;
	if (expSpeed < 0 && currentMove > expSpeed)
	{
		currentMove -= 2*changing;
	}
	else if (expSpeed > 0 && currentMove < expSpeed)
	{
		currentMove += 2*changing;
	}
	else if (expSpeed == 0)
	{
		if (currentMove < changing && currentMove > -changing)
		{
			currentMove = 0;
		}
		else
		{
			currentMove *= 0.8;
		}
	}
	*/
	static float currentMove = 0;
	static float max_speed = 8;
	int DIR = currentState - 1;
	if (DIR < 0)
	{
		currentSpeed *= (currentMove < (max_speed * -1) ? 0 : 1.015);
		currentMove -= currentSpeed;

		avatar.setTexture(avatarBackTex);
		//std::cout << currentMove << std::endl;
	}
	else if (DIR > 0)
	{
		currentSpeed *= (currentMove > max_speed ? 0 : 1.015);
		currentMove += currentSpeed;
		
		avatar.setTexture(avatarTex);
		//std::cout << currentMove << std::endl;
	}
	else
	{
		currentMove = 0;
		currentSpeed = defaultSpeed;
	}
	if (currentSpeed > 8)
	{
		currentSpeed = 8;
	}

	//std::cout << currentMove << std::endl;
	prevSpeed = currentMove;
	position.x += currentMove;

	/*
	//COLLISIONS
	if (currentMove > 0)
	{
		Vector2i tileParam(((position.x + texWidth - 16) / 64), (position.y + texHeight - 2) / 64);
		//std::cout << tileParam.x << " " << tileParam.y << " ";
		int tileRight = map[tileParam.x][tileParam.y];
		//std::cout << tileRight << std::endl;
		if (tileRight > 0 && tileRight < 10)
		{
			position.x = (tileParam.x * 64) - texWidth;
		}
	}
	else if(currentMove < 0)
	{
		Vector2i tileParam(position.x / 64, (position.y + texHeight - 2) / 64);
		int tileLeft = map[tileParam.x][tileParam.y];
		if (tileLeft > 0 && tileLeft < 10)
		{
			position.x = (tileParam.x + 1) * 64;
		}
	}
	*/

	Vector2f oldPos(avatar.getPosition());
	avatar.setPosition(position);
	//COLLISIONS 2
	if (currentMove > 0)
	{
		Vector2i tileOnPlayer(corners[2].x / 64, corners[2].y / 64);
		if (map[tileOnPlayer.x][tileOnPlayer.y] != 0)
		{
			avatar.setPosition(oldPos);
		}
	}
	else if (currentMove < 0)
	{
		Vector2i tileOnPlayer(corners[3].x / 64, corners[3].y / 64);
		if (avatar.getPosition().x < 0)
		{
			avatar.setPosition(0, avatar.getPosition().y);
		}
		else if (map[tileOnPlayer.x][tileOnPlayer.y] != 0)
		{
			avatar.setPosition(oldPos);
		}
	}
}

void player::jumping()
{
	static int	jumpHightCounter = 0;

	if (jumpHightCounter < 5)
	{
		Vector2i tileOnPlayer((avatar.getPosition().x + (corners[2].x - corners[3].x) / 2) / 64, (corners[0].y - 24) / 64);
		if (map[tileOnPlayer.x][tileOnPlayer.y] == 0)
		{
			avatar.setPosition(avatar.getPosition().x, avatar.getPosition().y - 24);
		}
		else
		{
			avatar.setPosition(avatar.getPosition().x, (((corners[0].y - 24) / 64) * 64) + 64);
		}
		

		jumpHightCounter++;
		gravitation = 4;
	}
	else
	{
		jumpHightCounter = 0;
		isJump = false;
		jumpingCounter--;
	}
}

void player::jump()
{
	if (jumpingCounter > 0)
	{
		isJump = true;
	}
}

void player::move(dir mode)
{
	if (mode != currentState)
	{
		currentState = mode;
	}
	if (mode != dir::STOP)
	{
		currentDir = mode;
	}
}

int player::sigLadder()
{
	//std::cout << "\nsig ladder time: " << ladderTime.getElapsedTime().asMilliseconds() << " ms";

	if (ladderTime.getElapsedTime().asMilliseconds() < 100)
	{
		stopGravity();
		return 1;
	}
	else
	{
		startGravity();
		return 0;
	}
}

