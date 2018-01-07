#include "game.h"
#include "warrior.h"
#include "ladder.h"

game::game(RenderWindow &window, options *sett) : window(window), settings(*sett)
{
	p_1 = new Mage("Lukasz");
	currentMapName = "Normal_1";
	level = new map_of_level(currentMapName);
	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;	
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);

	Physics.setLevel(level);
	
	mainLoop();
}


game::~game()
{
	delete level;
	delete p_1;
}

void game::mainLoop()
{
	while (true)
	{
		Physics.gravity(p_1);
		Physics.tileCollisions(p_1);

		Event handler;
		if (window.pollEvent(handler))
		{

			if (handler.type == Event::Closed)
			{
				window.close();
				break;
			}

			if (handler.type == Event::LostFocus)
			{
				window.setFramerateLimit(0);
			}
			else if (handler.type == Event::GainedFocus)
			{
				window.setFramerateLimit(60);
			}

			if (handler.type == Event::KeyPressed)
			{
				if (handler.key.code == Keyboard::Escape)
				{
					break;
				}
				else if (handler.key.code == Keyboard::Up)
				{
					
				}
				else if (handler.key.code == Keyboard::Right)
				{
					p_1->currentDirState = player::dir::FORWARD; 
				}
				else if (handler.key.code == Keyboard::Left)
				{
					p_1->currentDirState = player::dir::BACK;
				}
				else if (handler.key.code == Keyboard::Space)
				{
					if (!p_1->getJumpingVariable() && Physics.groudChecker(p_1))
					{
						p_1->startJump();
					}
				}
				else if (handler.key.code == Keyboard::Z)
				{
					skillsArray.push_back(p_1->getSkill(1));
				}

			}
			else if (handler.type == Event::KeyReleased)
			{
				if (handler.key.code == Keyboard::Right)
				{
					p_1->currentDirState = player::dir::STOP;
				}
				else if (handler.key.code == Keyboard::Left)
				{
					p_1->currentDirState = player::dir::STOP;
				}
			}

		}

		window.clear();
		camera();
		window.setView(screen);

		draw();
		//p_1->drawCorners(window);

		window.display();
		endOfLevel();
	}

}

void game::draw()
{

	Vector2f screenPosition(screen.getCenter().x - (SCRN_WIDTH / 2),
	screen.getCenter().y - (SCRN_HEIGHT / 2));
	level->backgroundSprite.setPosition(screenPosition);
	window.draw(level->backgroundSprite);

	/* MAP /
	for (int i = 0; i < level->tileSprites.size(); i++)
	{
		if (level->tileSprites[i].getPosition().x > screenPosition.x - 128)
		{
			if (level->tileSprites[i].getPosition().x < screenPosition.x + SCRN_WIDTH + 128)
			{
				window.draw(level->tileSprites[i]);
			}
		}
	}*/
	for (int i = 0; i < level->getAreaSpritesCount(); i++)
	{
		window.draw(level->getAreaSprite(i));
	}

	/* OBJECTS */
	vector<Sprite *> frontObjects;
	vector<enemies* > enemieObjects;
	for (int i = 0; i < level->getObjectsCount(); i++)
	{
		if (level->getObjectAt(i)->getSprite().getPosition().x > (screen.getCenter().x - (SCRN_WIDTH / 2)) - 128)
		{
			if (level->getObjectAt(i)->getSprite().getPosition().x < (screen.getCenter().x + (SCRN_WIDTH / 2)) + 256)
			{
				if (level->getObjectAt(i)->layer == accessories::Layer::FRONT)
				{
					/* ENEMIE CHECKER */
					enemies *temp = dynamic_cast<enemies*>(level->getObjectAt(i));
					if (temp)
					{
						if (!temp->isDead)
						{
							enemieObjects.push_back(temp);
						}
					}
					else
					{
						frontObjects.push_back(&level->getObjectAt(i)->getSprite());
					}
				}
				else
				{
					window.draw(level->getObjectAt(i)->getSprite());
				}
				level->getObjectAt(i)->eventP(*p_1);

				if (level->getObjectAt(i)->update())
				{

					//cout << "Usunalem nr. " << i << endl;
				}
			}
			else
			{
				break;
			}
		}
	}

	/* SKILLS 
	for (size_t i = 0; i < skillsArray.size(); i++)
	{
		if (skillsArray[i]->toRemove)
		{
			delete skillsArray[i];
			skillsArray.erase(skillsArray.begin() + i);
		}
		else
		{
			window.draw(skillsArray[i]->getSprite());
			for (size_t j = 0; j < enemieObjects.size(); j++)
			{
				window.draw(enemieObjects[i]->sprite);
				if (enemieObjects[j]->getSprite().getGlobalBounds().intersects(skillsArray[i]->getSprite().getGlobalBounds()))
				{
					enemieObjects[j]->hit(skillsArray[i]);
					skillsArray[i]->toRemove = true;
				}
			}
		}
	}
	*/
	/* ENEMIES AND SKILLS */
		//SKILLS
	for (size_t i = 0; i < skillsArray.size(); i++)
	{
		if (skillsArray[i]->toRemove)
		{
			delete skillsArray[i];
			skillsArray.erase(skillsArray.begin() + i);
		}
		else
		{
			window.draw(skillsArray[i]->getSprite());
		}
	}
		//ENEMIES
	for (size_t j = 0; j < enemieObjects.size(); j++)
	{
		window.draw(enemieObjects[j]->getSprite());
		for (size_t i = 0; i < skillsArray.size(); i++)
		{
			if (enemieObjects[j]->getSprite().getGlobalBounds().intersects(skillsArray[i]->getSprite().getGlobalBounds()))
			{
				*enemieObjects[j] -= (int)skillsArray[i]->getDemage();
				skillsArray[i]->toRemove = true;
			}
			if (enemieObjects[j]->getAttackState())
			{
				window.draw(enemieObjects[j]->hpBar);
			}
		}
	}

	/* PLAYER */
	if (p_1->getHp() > 0)
	{
		p_1->update();
		window.draw(p_1->getSprite());
	}
	else
	{
		p_1->death(Vector2f(0, 0));
	}

	/* OTHERS */
	for (size_t i = 0; i < frontObjects.size(); i++)
	{
		window.draw(*frontObjects[i]);
	}
	p_1->hudEffect(window);
}

void game::camera()
{
	const float maxCamSpeed = 14.6;
	float camSpeed = 2 + ((double)cameraTmer.getElapsedTime().asMilliseconds() / 600);
	camSpeed = camSpeed > maxCamSpeed ? maxCamSpeed : camSpeed;
	camSpeed = camSpeed < 2 ? 2 : camSpeed;

	float moveLine = screen.getCenter().x;
	float minLine = screen.getCenter().x - SCRN_WIDTH / 2;

	Vector2f moveCam(0, 0);
	if (p_1->currentMoveDir - 1 > 0)
	{
		moveLine = (screen.getCenter().x - (SCRN_WIDTH / 2)) + SCRN_WIDTH / 3;
		if (minLine < p_1->getPosition().x && p_1->getPosition().x > moveLine)
		{
			moveCam.x += camSpeed;
		}
		else if (minLine > p_1->getPosition().x)
		{
			moveCam.x -= camSpeed;
		}
	}
	else if(p_1->currentMoveDir - 1 < 0)
	{
		moveLine = (screen.getCenter().x + (SCRN_WIDTH / 2)) - SCRN_WIDTH / 3;
		if (p_1->getPosition().x < moveLine && minLine > 0)
		{
			moveCam.x -= camSpeed;
		}
	}

	float v = p_1->getPosition().y - screen.getCenter().y; // przesuniêcie osi Y
	if (v > 8)
	{
		moveCam.y += camSpeed / 2;
	}
	else if (v < -8)
	{
		moveCam.y -= camSpeed / 2;
	}
	else
	{
		moveCam.y = v;
	}

	if (moveCam.x + moveCam.y == 0)
	{
		cameraTmer.restart();
	}

	screen.move(moveCam);
}

void game::camera2()
{
	//const float increase = 0.05;
	const float maxCamSpeed = 14.6;
	float camSpeed = 0.8 + ((double)cameraTmer.getElapsedTime().asMilliseconds() / 600);
	camSpeed = camSpeed > maxCamSpeed ? maxCamSpeed : camSpeed;
	
	float moveLine = p_1->getPosition().x + (p_1->currentMoveDir-1 * 64);

	float h = (moveLine - screen.getCenter().x) * (p_1->currentMoveDir - 1); // if > 0 move cam

	Vector2f changePos(0, 0);


	if (h > 0)
	{
		changePos.x += camSpeed;
		changePos.x *= p_1->currentMoveDir-1;
		if ((screen.getCenter().x - SCRN_WIDTH / 2) + changePos.x < 0)
		{
			changePos.x = 0;
		}
	}
	else if (screen.getCenter().x - (SCRN_WIDTH /2) > p_1->getPosition().x)
	{
		changePos.x -= camSpeed;
	}
	
	float v = p_1->getPosition().y - screen.getCenter().y; // przesuniêcie osi Y
	if (v > 8)
	{
		changePos.y += camSpeed;
	}
	else if (v < -8)
	{
		changePos.y -= camSpeed;
	}
	else
	{
		changePos.y = v;
	}


	if (changePos.x + changePos.y == 0)
	{
		cameraTmer.restart();
	}

	//cout << changePos.x << endl;
	screen.move(changePos);
}

void game::endOfLevel()
{
	if (p_1->getPosition().x > level->getMapSizePx())
	{
		string nameLevel = f_manager.nextFile("data/Levels", currentMapName);
		p_1->setPosition(0, 0);

		RectangleShape curtain(Vector2f(SCRN_WIDTH, SCRN_HEIGHT));
		Vector2f posCurtain(screen.getCenter());
		posCurtain.x -= SCRN_WIDTH / 2;
		posCurtain.y -= SCRN_HEIGHT / 2;
		curtain.setPosition(posCurtain);
		Clock timer;
		Text newLevName;
		newLevName.setFont(settings.font_1);
		newLevName.setColor(Color(255, 255, 255));
		newLevName.setCharacterSize(42);
		newLevName.setString(string("NEW LEVEL: " + nameLevel));

		while (1)
		{
			newLevName.setPosition(screen.getCenter().x - (newLevName.getGlobalBounds().width / 2), screen.getCenter().y - (newLevName.getGlobalBounds().height / 2));
			
			Vector2f posCurtain(screen.getCenter());
			posCurtain.x -= SCRN_WIDTH / 2;
			posCurtain.y -= SCRN_HEIGHT / 2;
			curtain.setPosition(posCurtain);
			//curtain.setFillColor(Color(0, 0, 0, 200));
			if (timer.getElapsedTime().asMilliseconds() < 2000)
			{
				//printf("Color before 2000: %d\n", timer.getElapsedTime().asMilliseconds() / 8);
				curtain.setFillColor(Color(0, 0, 0, timer.getElapsedTime().asMilliseconds() / 8));
			}
			else
			{
				//printf("Color after 2000: %d\n", 255 - (timer.getElapsedTime().asMilliseconds() - 2000) / 8);
				curtain.setFillColor(Color(0, 0, 0, 255 - (timer.getElapsedTime().asMilliseconds() - 2000) / 8));
			}
			curtain.setSize(Vector2f(window.getSize()));
			camera();

			window.draw(curtain);
			window.draw(newLevName);
			window.setView(screen);
			window.display();
			if (timer.getElapsedTime().asMilliseconds() >= 2000)
			{

				if (timer.getElapsedTime().asMilliseconds() >= 4000)
				{
					delete level;
					level = new map_of_level(nameLevel);

					currentMapName = nameLevel;
					break;
				}
			}
		}

	}
}