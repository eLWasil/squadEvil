#include "Mage.h"



Mage::Mage(std::string name) //: player("Lukasz")
{
	currentStats.regMana += 5;
}


Mage::~Mage()
{
}

skills* Mage::getSkill(const int number = 0)
{
	Vector2f pos = this->getCenter();
	skills* temp = (new fireball(pos, this->currentDir - 1));
	if (number < 3)
	{
		temp = (new fireball(pos, this->currentDir - 1));
		int cost = temp->getCost();
		if (cost > getMANA())
		{
			return temp;
		}
		else
		{
			this->currentStats.MANA -= cost;
		}
	}
	return temp;
}

void Mage::regen()
{
	if (regTimer.getElapsedTime().asSeconds() > 5)
	{
		if (getHP() < getMaxHp())
		{
			currentStats.HP += currentStats.regHp;
			if (getHP() > getMaxHp())
			{
				currentStats.HP = getMaxHp();
			}
		}
		if (getMana() < getMaxMana())
		{
			currentStats.MANA += currentStats.regMana;
			if (getMana() > getMaxMana())
			{
				currentStats.HP = getMaxMana();
			}
		}
		regTimer.restart();
	}
}