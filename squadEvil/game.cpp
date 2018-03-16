#include "game.h"
#include "warrior.h"
#include "ladder.h"

game::game(RenderWindow &window, options *sett) : window(window), settings(*sett)
{
	p_1 = new Mage("Lukasz");

	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;	
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);

	currentMapName = "mymap";
	level = new map_level(currentMapName);
	Physics.setLevel(level);

	cameraTarget = screen.getCenter();
	
	background.setTexture(level->getTexture(0));
	background.setPosition(0, 0);

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
					if (skillsTimer.getElapsedTime().asMilliseconds() > 150)
					{
						skills* ptr = p_1->getSkill(1);
						if (ptr != nullptr)
						{
							skillsArray.push_back(ptr);
							skillsTimer.restart();
						}
					}
				}
				else if (handler.key.code == Keyboard::Z)
				{
					
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

bool game::isVisible(Sprite *holder)
{
	if (holder->getPosition().x > screen.getCenter().x - (SCRN_WIDTH / 2) - 128)
	{
		if (holder->getPosition().x < screen.getCenter().x + (SCRN_WIDTH / 2) + 128)
		{
			if (holder->getPosition().y > screen.getCenter().y - (SCRN_HEIGHT / 2) - 128)
			{
				if (holder->getPosition().y < screen.getCenter().y + (SCRN_HEIGHT / 2) + 128)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void game::draw()
{
	Vector2f screenPosition(screen.getCenter().x - (SCRN_WIDTH / 2),
							screen.getCenter().y - (SCRN_HEIGHT / 2));

	background.setPosition(screenPosition);
	window.draw(background);


	drawTiles();
	drawObjects();
	drawEnemies();
	drawSkills();

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
}

void game::drawTiles()
{
	int idx = 0;
	Sprite *tileHolder;
	
	do
	{
		tileHolder = level->getTileAt(idx);
		if (!tileHolder)
		{
			break;
		}

		if (isVisible(tileHolder))
		{
			window.draw(*tileHolder);
		}
		else if (tileHolder->getPosition().x > screen.getCenter().x - (SCRN_WIDTH / 2) + 128)
		{
			break;
		}

	} while (++idx);
}

void game::drawObjects()
{
	int idx = 0;
	accessories *objectHolder;

	do
	{
		objectHolder = level->getObjectAt(idx);
		if (!objectHolder)
		{
			break;
		}

		if (isVisible(&objectHolder->getSprite()))
		{
			window.draw(objectHolder->getSprite());
			objectHolder->update();
			objectHolder->eventP(*p_1);
		}
		else if (objectHolder->getSprite().getPosition().x > screen.getCenter().x - (SCRN_WIDTH / 2) + 128)
		{
			break;
		}

	} while (++idx);
}

void game::drawEnemies()
{
	int idx = 0;
	enemies *enemieHolder;

	do
	{
		enemieHolder = level->getEnemieAt(idx);
		if (!enemieHolder)
		{
			break;
		}

		if (isVisible(&enemieHolder->getSprite()))
		{
			window.draw(enemieHolder->getSprite());
			enemieHolder->update();
			enemieHolder->eventP(*p_1);
		}
		else if (enemieHolder->getSprite().getPosition().x > screen.getCenter().x - (SCRN_WIDTH / 2) + 128)
		{
			break;
		}

	} while (++idx);
}

void game::drawSkills()
{
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
}

void game::camera2()
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

void game::camera()
{
	Vector2f moveCam(0, 0);
	int delayAsMilisec = 400;

	if (cameraTmer.getElapsedTime().asMilliseconds() > delayAsMilisec)
	{
		cameraTarget = p_1->getPosition();

		if (cameraTarget.x - SCRN_WIDTH / 2 < 0)
		{
			cameraTarget.x = SCRN_WIDTH / 2;
		}

		moveCam.x = 0;
		cameraTmer.restart();
	}
	else
	{
		Vector2f dist(0, 0);
		dist.x = cameraTarget.x - screen.getCenter().x;

		float speed = dist.x == 0 ? 0 : (dist.x / (delayAsMilisec*2));

		float deltaTime = 1000 / 60;

		moveCam.x = speed * deltaTime;
	}



	// Old Version
	/*
	float xLLine = screen.getCenter().x - SCRN_WIDTH / 8;
	float xRLine = screen.getCenter().x + SCRN_WIDTH / 8;

	if (p_1->getPosition().x < xLLine)
	{
		if (screen.getCenter().x - SCRN_WIDTH / 2 <= 0)
		{
			moveCam.x = -(screen.getCenter().x - SCRN_WIDTH / 2);
		}
		else
		{
			moveCam.x = -p_1->getSpeed() / 2;
		}
	}
	else if (p_1->getPosition().x > xRLine)
	{
		moveCam.x = p_1->getSpeed() / 2;
	}
	*/


	float v = p_1->getPosition().y - screen.getCenter().y; // przesuniêcie osi Y
	if (v > 8)
	{
		moveCam.y += 4 / 2;
	}
	else if (v < -8)
	{
		moveCam.y -= 4 / 2;
	}
	else
	{
		moveCam.y = v;
	}

	screen.move(moveCam);
}

void game::endOfLevel()
{
	if (p_1->fEndOfLevel)
	{
		p_1->fEndOfLevel = false;

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
					level = new map_level(nameLevel);

					currentMapName = nameLevel;
					break;
				}
			}
		}

	}
}