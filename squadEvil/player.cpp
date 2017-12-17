#include "player.h"
#include <math.h>
#include <iostream>
#include <string>

#define TILESIZE 64

player::player() : isJump(false), currentState(dir::STOP), currentMoveDir(dir::FORWARD), texWidth(84), texHeight(64), hud(*this), defaultSpeed(0.6), gravitation(2)
{
	avatarTex.loadFromFile("data/Graphics/Avatars/tileset_mage.png", IntRect(0, 0, texWidth, texHeight));
	avatarBackTex.loadFromFile("data/Graphics/Avatars/tileset_mage.png", IntRect(0, 64, texWidth, texHeight));

	String states[] = { "Attack", "Climb", "Dead", "Glide", "Idle", "Jump", "Jump_Attack", "Jump_Throw", "Run", "Slide", "Throw" };
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			String n = "data/Graphics/Avatars/girl/" + states[i];
			n += "__00";
			n += std::to_string(j);
			n += ".png";


			
			girlTex[i][j].loadFromFile(n);
			girlTex[i][j].setSmooth(true);
		}
	}
	cTextureState = states::Run;

	// AVATAR SPRITE 
	controlPoint = Vector2f(20, 0);
	avatar.setPosition(controlPoint);
	avatar.setTexture(girlTex[cTextureState][0]);
	avatar.setOrigin(avatar.getGlobalBounds().width / 2, avatar.getGlobalBounds().height / 2 * 0);
	avatar.setScale(Vector2f(0.14, 0.14));
	jumpingCounter = 2;

	

	hitColorCounter = 0;
	//gravitation = 4;
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
	window.draw(hud.getGold());
	window.draw(hud.statistic[0]);
	window.draw(hud.statistic[1]);
	window.draw(hud.statistic[2]);

}

void player::checkTexState()
{
	static int texCounter = 0;
	if (avatarFramesTimer.getElapsedTime().asMilliseconds() > (1000 / 60))
	{
		avatar.setTexture(girlTex[cTextureState][++texCounter % 9]);
		avatarFramesTimer.restart();
	}

	if (isJump && currentState-1 == 0)
	{
		cTextureState = states::Jump;
	}
	else if (isJump && (currentState - 1) != 0)
	{
		cTextureState = states::Jump_Throw;
	}
	else if ((currentState - 1) != 0)
	{
		//std::cout << currentState - 1 << std::endl;
		cTextureState = states::Run;
	}
	else
	{
		cTextureState = states::Idle;
	}
}

void player::update()
{
	Vector2f oldPosition = avatar.getPosition();

	checkTexState();
	setCorners();
	regen();

	//std::cout << avatar.getOrigin().x << ", " << avatar.getOrigin().y << std::endl;
	//std::cout << avatar.getScale().x << std::endl;
	//avatar.setOrigin(avatar.getGlobalBounds().width , avatar.getGlobalBounds().height );
	//avatar.rotate(10);

	
	

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

void player::drawCorners(RenderWindow &window)
{
	for (size_t i = 0; i < 5; i++)
	{
		CircleShape point;
		point.setFillColor(Color(255, 255, 255));
		point.setPosition(corners[i]);
		point.setRadius(2);
		window.draw(point);

	}
}

void player::setCorners()
{
	Vector2f pos = avatar.getPosition();
	/* CORNERS FOR COLLISIONS */
	/* Have to be a little bit 'in' texture */
	int padding = 6;
	float avatarHalfWidth = avatar.getGlobalBounds().width / 2;
	corners[0].x = pos.x - avatarHalfWidth + padding; // LEFT TOP
	corners[0].y = pos.y + padding;
	corners[1].x = pos.x + avatarHalfWidth  - padding; // RIGHT TOP
	corners[1].y = pos.y + padding;
	corners[2].x = pos.x + avatarHalfWidth  - padding; // RIGHT BOT
	corners[2].y = pos.y + avatar.getGlobalBounds().height - padding - 4;
	corners[3].x = pos.x - avatarHalfWidth + padding;
	corners[3].y = pos.y + avatar.getGlobalBounds().height - padding - 4; // LEFT BOT
	
	// Center
	corners[4].x = pos.x;
	corners[4].y = pos.y + avatar.getGlobalBounds().height / 2;
}

void player::gravity()
{
	static int k = 0;
	int timer = gravitySpeedTimer.getElapsedTime().asMilliseconds();
	int mod = gravitation + ((timer > 1000 ? 1000 : timer) / 50);
	Vector2f newPosition = Vector2f(avatar.getPosition().x, avatar.getPosition().y + mod);
	Vector2i tileOnNewPosition(corners[4].x / TILESIZE, (corners[2].y + mod) / TILESIZE);

	if(map[tileOnNewPosition.x][tileOnNewPosition.y] == 0)
	{
		if (newPosition.y > (map.size() - 5) * TILESIZE)
		{
			avatar.setPosition(controlPoint);
		}
		else
		{
			avatar.setPosition(newPosition);
		}
	}
	else
	{
		gravitySpeedTimer.restart(); 
		isJump = false;
		

		jumpingCounter = 2;
	}
}


void player::moving()
{
	Vector2f position = avatar.getPosition();
	static float currentMoveSpeed = 0;
	static float max_speed = 8;
	int DIR = currentState - 1;
	

	if (DIR < 0)
	{
		currentSpeed *= (currentMoveSpeed < (max_speed * -1) ? 0 : 1.015);
		currentMoveSpeed -= currentSpeed;

		if (avatar.getScale().x > 0)
		{
			avatar.setScale(avatar.getScale().x * -1, avatar.getScale().y);
		}
	}
	else if (DIR > 0)
	{
		currentSpeed *= (currentMoveSpeed > max_speed ? 0 : 1.015);
		currentMoveSpeed += currentSpeed;
		
		if (avatar.getScale().x < 0)
		{
			avatar.setScale(avatar.getScale().x * -1, avatar.getScale().y);
		}
	}
	else
	{
		currentMoveSpeed = 0;
		currentSpeed = defaultSpeed;
	}
	if (currentSpeed > 8)
	{
		currentSpeed = 8;
	}

	prevSpeed = currentMoveSpeed;
	position.x += currentMoveSpeed;

	Vector2f oldPos(avatar.getPosition());
	avatar.setPosition(position.x, position.y);

	/*
	//COLLISIONS
	if (currentMoveSpeed > 0)
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
	else if(currentMoveSpeed < 0)
	{
		Vector2i tileParam(position.x / 64, (position.y + texHeight - 2) / 64);
		int tileLeft = map[tileParam.x][tileParam.y];
		if (tileLeft > 0 && tileLeft < 10)
		{
			position.x = (tileParam.x + 1) * 64;
		}
	}
	*/



	//COLLISIONS 2
	if (currentMoveSpeed > 0)
	{
		Vector2i tileOnPlayer(corners[2].x / 64, corners[2].y / 64);
		if (map[tileOnPlayer.x][tileOnPlayer.y] != 0)
		{
			avatar.setPosition(oldPos);
		}
	}
	else if (currentMoveSpeed < 0)
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
	int hightJump = 32;

	if (jumpHightCounter < 3)
	{
		Vector2i tileOnPlayer(avatar.getPosition().x / 64, corners[3].y / 64);
		//std::cout << "player.cpp(344): " << tileOnPlayer.x << " " << tileOnPlayer.y << std::endl;
		
		if (map[tileOnPlayer.x][tileOnPlayer.y] == 0)
		{
			avatar.setPosition(avatar.getPosition().x, avatar.getPosition().y - hightJump);
		}
		else
		{
			avatar.setPosition(avatar.getPosition().x, (((corners[0].y - 24) / 64) * 64) + 64);
		}
		

		jumpHightCounter++;
		//gravitation = 3;
	}
	else
	{
		jumpHightCounter = 0;
		//isJump = false;
		jumpingCounter--;
	}
}

void player::jump()
{
	if (jumpingCounter > 0)
	{
		isJump = true;
		cTextureState = states::Jump;
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
		currentMoveDir = mode;
	}
}

int player::sigLadder()
{
	//std::cout << "\nsig ladder time: " << ladderTime.getElapsedTime().asMilliseconds() << " ms";

	if (ladderTime.getElapsedTime().asMilliseconds() < 100)
	{
		stopGravity();
		return 1;
		//std::cout
	}
	else
	{
		startGravity();
		return 0;
	}
}

