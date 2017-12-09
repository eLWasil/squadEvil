#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class player;
using namespace sf;

class HUD
{
public:
	HUD(player &);
	~HUD();

	void update();
	Text getGold() { return Gold_1; };
	RectangleShape statistic[3];
private:
	Font font;
	Text Gold_1;
	
	player &Player;
	float valueOfStatistic[3];
};

