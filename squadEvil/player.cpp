#include "player.h"
#include <math.h>
#include <iostream>
#include <string>

#define TILESIZE 64

player::player() : isJump(false), currentDirState(dir::STOP), currentMoveDir(dir::FORWARD), hud(*this)
{
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
	lastControlPoint = Vector2f(20, 0);
	avatar.setPosition(lastControlPoint);
	avatar.setTexture(girlTex[cTextureState][0]);
	avatar.setOrigin(avatar.getGlobalBounds().width / 2, avatar.getGlobalBounds().height / 2 * 0);
	avatar.setScale(Vector2f(0.14, 0.14));
	jumpingCounter = 2;

	

	hitColorCounter = 0;

	skillBar[1] = skillBar[2] = skillBar[3] = 1;
}


player::~player()
{
}

void player::setMap(std::vector <std::vector <int>> &level)
{
	//map = level;
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

	if (isJump && currentDirState-1 == 0)
	{
		cTextureState = states::Jump;
	}
	else if (isJump && (currentDirState - 1) != 0)
	{
		cTextureState = states::Jump_Throw;
	}
	else if ((currentDirState - 1) != 0)
	{
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
		//jumping();
	}

	if (hitColorTimer.getElapsedTime().asMilliseconds() > 300 && hitColorTimer.getElapsedTime().asMilliseconds() < 400)
	{
		avatar.setColor(Color(255, 255, 255));
	}
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

/*
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

*/
