#include <SFML\Graphics.hpp>
using namespace sf;
class accessories;

#pragma once
class skills
{
public:
	skills();
	~skills();

	virtual int update(accessories* frontObject) { return -1; };

	/* Getters */
	virtual const float getDemage() { return basics.demage; }
	virtual const float getCost() { return basics.cost; }
	virtual Sprite getSprite() { return startingSprite; };


	bool toRemove;
protected:
	struct basicStats
	{
		float
			demage,
			duration,
			cost,
			range;
		int	level;
		int dir = 0; // -1 0 1 
	} basics;

	Sprite startingSprite;
	//Sprite startingSprite;
	Texture startingTex;

};

