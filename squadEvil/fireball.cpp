#include "fireball.h"
#include "accessories.h"
#include <iostream>
using namespace std;

fireball::fireball(float x, float y, int dir) : dirent(dir)
{
	startingSprite.setPosition(x, y);
	build();
}

fireball::fireball(Vector2f pos, int dir) : dirent(dir)
{
	startingSprite.setPosition(pos.x, pos.y);
	build();
	//printf("fireball(vec2f)\n");
}	

void fireball::build()
{
	speed = 9;
	dist = 0;
	basics.cost = 5;
	basics.demage = 20;
	basics.duration = 0.5;
	basics.level = 1; // Do poprawienia, przy wczytywaniu postaci.
	basics.range = 64 * 10;

	startingTex.loadFromFile("data/Graphics/Skills/Mage/Fireball/rfireball.png");
	startingSprite.setTexture(startingTex);
	
	//startingSprite.setFillColor(Color(255, 0, 0));
	//startingSprite.setRadius(12.f);

	this->currentSprite = startingSprite;

	//currentSprite.setFillColor(startingSprite.getFillColor());
	//currentSprite.setPosition(startingSprite.getPosition());
	//currentSprite.setRadius(startingSprite.getRadius());
	//printf("Fireball Position (x, y): %d, %d \n", startingSprite.getPosition().x, startingSprite.getPosition().y);
}

fireball::~fireball()
{
}

Sprite fireball::getSprite()
{
	//cout << "fireball FPS: ";
	if (fpsTimer.getElapsedTime().asMilliseconds() > (1000/60))
	{
		//cout << fpsTimer.getElapsedTime().asMilliseconds() << endl;
		Vector2f pos = currentSprite.getPosition();
		pos.x += (speed * dirent); 
		pos.y = pos.y + (rand() % 3) - 1;
		currentSprite.setPosition(pos);
		//cout << pos.x << ", " << pos.y << endl;

		if (abs(dist += speed) > basics.range)
		{
			toRemove = true;
		}
		fpsTimer.restart();
	}
	return currentSprite;
}

int fireball::update(accessories *temp)
{
	return 0;
}