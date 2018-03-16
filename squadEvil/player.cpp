#include "player.h"
#include <math.h>
#include <iostream>
#include <string>

#define TILESIZE 64

player::player() : isJumping(false), isAttack(false), currentDirState(dir::STOP), currentMoveDir(dir::FORWARD), hud(*this)
{
	fEndOfLevel = false;
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
	pSprite.setPosition(lastControlPoint);
	pSprite.setTexture(girlTex[cTextureState][0]);
	pSprite.setOrigin(pSprite.getGlobalBounds().width / 2, pSprite.getGlobalBounds().height / 2 * 0);
	pSprite.setScale(Vector2f(0.14, 0.14));
	
	startingPositionY = 0; // for jumping
	hitColorCounter = 0;

	skillBar[1] = skillBar[2] = skillBar[3] = 1;
}


player::~player()
{
}

Vector2f player::getPosition()
{
	return pSprite.getPosition();
}

Vector2f player::getCenter()
{
	Vector2f center = pSprite.getPosition();
	center.x += pSprite.getGlobalBounds().width / 2;
	center.y += pSprite.getGlobalBounds().height / 2;
	return center;
}

void player::hitEffect()
{
	pSprite.setColor(Color(0, 0, 0));
	hitColorTimer.restart();
	pSprite.setPosition(pSprite.getPosition().x - 5, pSprite.getPosition().y - 10);
}


//ENUM HERE 
void player::checkTexState()
{
	static int texCounter = 0;
	if (avatarFramesTimer.getElapsedTime().asMilliseconds() > (1000 / 60))
	{
		texCounter = (texCounter + 1) % 9;
		pSprite.setTexture(girlTex[cTextureState][texCounter]);
		avatarFramesTimer.restart();
	}

	if (isJumping && currentDirState-1 == 0)
	{
		cTextureState = states::Jump;
	}
	else if (isJumping && (currentDirState - 1) != 0)
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
	Vector2f oldPosition = pSprite.getPosition();
	if (oldPosition.x < 0)
	{
		pSprite.setPosition(0, oldPosition.y);
	}

	checkTexState();
	setCorners();
	regen();

	//std::cout << pSprite.getOrigin().x << ", " << pSprite.getOrigin().y << std::endl;
	//std::cout << pSprite.getScale().x << std::endl;
	//pSprite.setOrigin(pSprite.getGlobalBounds().width , pSprite.getGlobalBounds().height );
	//pSprite.rotate(10);


	if (hitColorTimer.getElapsedTime().asMilliseconds() > 300 && hitColorTimer.getElapsedTime().asMilliseconds() < 400)
	{
		pSprite.setColor(Color(255, 255, 255));
	}
}

Sprite &player::getSprite()
{
	return pSprite;
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
	Vector2f pos = pSprite.getPosition();
	/* CORNERS FOR COLLISIONS */
	/* Have to be a little bit 'in' texture */
	int padding = 6;
	float avatarHalfWidth = pSprite.getGlobalBounds().width / 2;
	corners[0].x = pos.x - avatarHalfWidth + padding; // LEFT TOP
	corners[0].y = pos.y + padding;
	corners[1].x = pos.x + avatarHalfWidth  - padding; // RIGHT TOP
	corners[1].y = pos.y + padding;
	corners[2].x = pos.x + avatarHalfWidth  - padding; // RIGHT BOT
	corners[2].y = pos.y + pSprite.getGlobalBounds().height - padding - 4;
	corners[3].x = pos.x - avatarHalfWidth + padding;
	corners[3].y = pos.y + pSprite.getGlobalBounds().height - padding - 4; // LEFT BOT
	
	// Center
	corners[4].x = pos.x;
	corners[4].y = pos.y + pSprite.getGlobalBounds().height / 2;
}