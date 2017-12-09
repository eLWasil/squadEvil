#include "enemies.h"
#include "skills.h"
#define PI 3.14159265



enemies::enemies() : isDead(false)
{
	attack = false;
	hpBar.setFillColor(Color((82, 0, 10)));
	hpBar.setPosition(this->position.x, this->position.y + 10);
	Vector2f size(this->sprite.getGlobalBounds().width * stats.HP, 6);
	hpBar.setSize(size);
}


enemies::~enemies()
{
}

Vector2f enemies::newDir(int rotAngle)
{
	int r = 1;
	double angle = enemie.angle + rotAngle;
	if (angle >= 360)
	{
		angle = angle - 360;
	}
	else if (angle < 0)
	{
		angle = 360 - angle;
	}
	enemie.angle = angle;
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setRotation(enemie.angle);

	angle = angle * PI / 180.0;
	return Vector2f(cos(angle), sin(angle));
}

int enemies::operator-=(int skillDmg)
{
	stats.HP -= skillDmg;
	if (stats.HP <= 0)
	{
		stats.HP = 0;
		isDead = true;
	}
	if (!attack)
	{
		attack = true;
	}
	hitTimer.restart();
	return stats.HP;
}

void enemies::hit(skills* skill)
{
	stats.HP -= skill->getDemage();
	if (stats.HP <= 0)
	{
		stats.HP = 0;
		isDead = true;
	}
	if (!attack)
	{
		attack = true;
	}
}

bool enemies::update()
{
	return isDead;
}