#include "chicken.h"



chicken::chicken()
{
	allTex[0].loadFromFile("data/Graphics/Others/enemies/angryChickenTileset.png", IntRect(0, 0, 64, 46));
	allTex[1].loadFromFile("data/Graphics/Others/enemies/angryChickenTileset.png", IntRect(0, 46, 64, 46));
	allTex[2].loadFromFile("data/Graphics/Others/enemies/h_angryChickenTileset.png", IntRect(0, 0, 64, 46));
	allTex[3].loadFromFile("data/Graphics/Others/enemies/h_angryChickenTileset.png", IntRect(0, 46, 64, 46));

	sprite.setPosition(0, 0);
	texture = allTex[0];
	sprite.setTexture(texture);

	position = sprite.getPosition();

	angleIncrease = 0;
	changeDir = false;
	runAway = false;
	targetChanging = false;
	stats.demage = 15;
	enemie.range = 124;
	degree = 0;

	attackSpeed = 4.4;
	flySpeed = 3.7 + (float(rand() % 100) / 100);
	angleSpeed = 3.5;

	enemie.speed = flySpeed;
	reverse = false;

	layer = Layer::FRONT;

	maxHP = stats.HP;
	currentHpOnBar = stats.HP;

	isHFlagOpen = true;
}


chicken::~chicken()
{
}

void chicken::eventP(player &Player)
{
	if (runAway)
	{
		Vector2f pPos = Player.getConstSprite().getPosition();

		double a = abs(pPos.x - sprite.getPosition().x);
		double b = abs(pPos.y - sprite.getPosition().y);

		double c = sqrt((a*a) + (b*b));

		if (c > enemie.range * 4)
		{
			runAway = false;
		}
	}
	else if (!attack)
	{
		Vector2f pPos = Player.getConstSprite().getPosition();
		pPos.x += Player.getConstSprite().getGlobalBounds().width / 2;
		pPos.y += Player.getConstSprite().getGlobalBounds().height / 2;

		double a = abs(pPos.x - sprite.getPosition().x);
		double b = abs(pPos.y - sprite.getPosition().y);

		double c = sqrt((a*a) + (b*b));

		if (c <= (enemie.range))
		{
			enemie.speed = attackSpeed;
			attack = true;
			sprite.setTexture(allTex[0]);
			timer.restart();
			target.x = Player.getPosition().x + (Player.getConstSprite().getGlobalBounds().width / 2);
			target.y = Player.getPosition().y + (Player.getConstSprite().getGlobalBounds().height / 2);
			targetChanging = true;
		}
	}
	else
	{
		enemie.speed += enemie.speed > 4.4 ? 0.005 : 0;

		if (timer.getElapsedTime().asSeconds() > 1)
		{
			timer.restart();
			target.x = Player.getPosition().x + (Player.getConstSprite().getGlobalBounds().width / 2);
			target.y = Player.getPosition().y + (Player.getConstSprite().getGlobalBounds().height / 2);
			targetChanging = true;
		}
	}

	if (targetChanging)
	{
		Vector2f enemyPos(sprite.getPosition());
		enemyPos.x -= sprite.getGlobalBounds().width / 2;
		enemyPos.y -= sprite.getGlobalBounds().height / 2;


		double a = abs(target.y - enemyPos.y);
		double b = abs(target.x - enemyPos.x);


		double c = sqrt(a*a + b*b);

		double inRadian = asin(a / c);
		degree = inRadian * 57.29577951308; // 180/PI
		if (target.y >= sprite.getPosition().y)
		{
			if (target.x >= sprite.getPosition().x)
			{
			}
			else
			{
				degree = 180 - degree;
			}
		}
		else
		{
			if (target.x >= sprite.getPosition().x)
			{
				degree = 360 - degree;
			}
			else
			{
				degree = 180 + degree;
			}
		}

		// Ta 2 oznacza o ile siê zmienia max k¹t ptaków w czasie namierzaniu celu
		if (abs(degree - enemie.angle) >= 2)
		{
			angleIncrease = 2;

			int leftRev = abs(degree - enemie.angle);
			int rightRev = abs(enemie.angle + (360 - degree));

			if (rightRev < leftRev)
			{
				angleIncrease *= -1;
			}

			if (abs(enemie.angle - degree) < angleIncrease)
			{
				angleIncrease = abs(enemie.angle - degree);
			}
		}
		else
		{
			angleIncrease = 0;
		}
		//cout << degree << " " << target.x << " " << target.y << endl;
		//cout << enemie.angle << ", " << degree << " inc: " << angleIncrease << endl;

		targetChanging = false;
	}


	/* PLAYER COLLISION */
	if (sprite.getGlobalBounds().intersects(Player.getConstSprite().getGlobalBounds()))
	{
		if (isHFlagOpen)
		{
			isHFlagOpen = false;
			hittimer.restart();

			Player.hitEffect();
			Player -= stats.demage;


			enemie.speed = attackSpeed;
			hittimer.restart();
			runAway = true;
			target = position;
			targetChanging = true;
		}
		else if (hittimer.getElapsedTime().asMilliseconds() > 1000)
		{
			isHFlagOpen = true;
		}

		
	}
}

bool chicken::update()
{
	if (!isDead)
	{
		enemie.dir = newDir(angleIncrease);
		Vector2f newPosition;
		newPosition.x = sprite.getPosition().x + (enemie.speed * enemie.dir.x);
		newPosition.y = sprite.getPosition().y + (enemie.speed * enemie.dir.y);

		if (!attack)
		{

			if (enemie.dir.x < 0)
			{
				newPosition.x -= 1.5;
			}
			else if (enemie.dir.x >= 0)
			{
				newPosition.x += 1.5;
			}
			//cout << newPosition.x << " = " << sprite.getPosition().x << " + " << (enemie.speed * enemie.dir.x) << ", angle: " << enemie.angle << endl;
			sprite.setPosition(newPosition);

			//REVERSE
			int startPos = position.x;
			Vector2f limits(startPos - enemie.range, startPos + enemie.range);
			if (newPosition.x > position.x + enemie.range)
			{
				if (!reverse)
				{
					angle = 180;
				}
				reverse = true;
			}
			else if (newPosition.x < 0 || newPosition.x < position.x - enemie.range)
			{
				if (!reverse)
				{
					angle = 0;
				}
				reverse = true;
				// cout << "Chicken new position: " << newPosition.x << endl;
			}

			if (angle >= 360)
			{
				angle -= 360;
			}

			if (reverse)
			{
				//enemie.angle != angle
				if (abs(angle - enemie.angle) >= 2)
				{
					angleIncrease = 2;
					enemie.speed = angleSpeed;
					if (enemie.angle > angle - 90)
					{
						if (enemie.dir.y > 0)
						{
							sprite.setTexture(allTex[1]);
						}
						else if (enemie.dir.y < 0)
						{
							sprite.setTexture(allTex[0]);
						}
					}
				}
				else
				{
					angleIncrease = 0;
					reverse = false;
					enemie.speed = flySpeed;
				}
				//cout << enemie.angle << " != " << angle << endl;
			}
		}
		else
		{
			if (abs(degree - enemie.angle) <= 5)
			{
				angleIncrease = 0;
			}

			if (hitTimer.getElapsedTime().asSeconds() < 3)
			{
				if ((hitTimer.getElapsedTime().asMilliseconds() % 500) < 250)
				{
					if (enemie.dir.x < 0)
					{
						sprite.setTexture(allTex[2]);
					}
					else if (enemie.dir.x >= 0)
					{
						sprite.setTexture(allTex[3]);
					}
				}
				else
				{
					sprite.setTexture(allTex[0]);
				}
			}
			else
			{
				sprite.setTexture(allTex[0]);
			}

			sprite.setPosition(newPosition);
		}

		/* HP BAR */
		if (stats.HP < currentHpOnBar)
		{
			currentHpOnBar--;
			hpBar.setSize(Vector2f((currentHpOnBar / maxHP) * sprite.getGlobalBounds().width, 6));
		}
		hpBar.setPosition(this->position.x, this->position.y + 10);

		return false;
	}
	else
	{
		return true;
	}
}