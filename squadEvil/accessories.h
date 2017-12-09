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

	sf::Sprite sprite;

	virtual void eventP(player &) { };
	virtual void drawable() {};
	virtual bool update(); //return if delete object
	bool update(map_level &) { return false; };

	enum Layer
	{
		BACK, FRONT
	} layer;

	string getName();
protected:
	string name;
	sf::Vector2f position;

	

	Clock time;
	bool remove;

private:
};

