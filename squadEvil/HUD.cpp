#include "HUD.h"
#include "player.h"
#include <string>
#include <iostream>

HUD::HUD(player &Player) : Player(Player)
{
	font.loadFromFile("data/Fonts/Pacha.otf");
	Gold_1.setFont(font);
	Gold_1.setCharacterSize(24);
	Gold_1.setColor(Color(0, 0, 0));

	statistic[0].setFillColor(Color(82, 0, 10));
	statistic[1].setFillColor(Color(22, 39, 86));
	statistic[2].setFillColor(Color(236, 248, 165));

	valueOfStatistic[0] = Player.getHp(); 
	valueOfStatistic[1] = Player.getMana();
	valueOfStatistic[3] = Player.getExp();
}


HUD::~HUD()
{
}


void HUD::update()
{
	Vector2f posP = Player.getConstSprite().getPosition();

	int gold = Player.getGold();
	std::string gold_1 = "G: ";
	gold_1 += std::to_string(gold);
	Gold_1.setString(gold_1);
	//Gold_1.setPosition(screen.getCenter().x - screen.getSize().x / 2, screen.getCenter().y - screen.getSize().y / 2);
	
	Gold_1.setPosition(posP.x, posP.y - 66);
	
	int statisticMax[] = { Player.getMaxHp(), Player.getMaxMana(), 100 };
	float statisticCurrent[] = { Player.getHp(), Player.getMana() - 20, 65 };

	for (int i = 0; i < 3; i++)
	{
		if (valueOfStatistic[i] > statisticCurrent[i])
		{
			valueOfStatistic[i]--;
		}
		else if (valueOfStatistic[i] < statisticCurrent[i])
		{
			valueOfStatistic[i]++;
		}
		float statisticProc = valueOfStatistic[i] / statisticMax[i];
		Vector2f size(Player.getConstSprite().getGlobalBounds().width * statisticProc, 6);
		statistic[i].setPosition(posP.x, posP.y - 40 + (i*(size.y + 2)));
		statistic[i].setSize(size);
	}


}