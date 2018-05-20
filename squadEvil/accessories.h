#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <math.h>
#include "player.h"
using namespace std;

class map_level;

class accessories
{
public:
	accessories();
	virtual ~accessories();

	virtual void eventP(player &) { };
	virtual void drawable() {};
	virtual bool update(); //return true if u want remove 
	virtual void setPosition(Vector2f);
	virtual Sprite getSprite() { return sprite; };

	bool update(map_level &) { return false; };

	enum Layer
	{
		BACK, FRONT
	} layer;

	//string getName();


	operator Sprite() const { return sprite; }
protected:
	//string name;
	sf::Sprite sprite;
	sf::Texture texture;

	Clock time;
	bool remove;

	int typeOfObjectByNr;
private:
};

