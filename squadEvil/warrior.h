#pragma once
#include "enemies.h"
class warrior :
	public enemies
{
public:
	warrior(sf::Sprite sprite, string type);
	~warrior();

	virtual bool update();
	virtual void eventP(player &);
	void setMap(std::vector <std::vector < int>> &);

private:
	std::vector <std::vector <int>> map;

	void setLimits();
	Vector2f walkLimits;
	int degree;
	Clock standTimer;
	bool isStanding;
};

