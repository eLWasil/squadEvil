#include "editor.h"
#include "accessories.h"
#include "SFML\Graphics\Sprite.hpp"

const int tileCount = 19;
//const int countOfAllSprites = 34;
const int accessoriesTileMove = 16;
const int areaTileMove = 64;
bool editorVisable;
bool reverseTile = false, darkMode = false;

editor::editor(RenderWindow &window) : edWindow(window), currentTileType(1), interfaceMode(0)
{
	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);


	loadInterace();
	mouseDelta = 0;
	setWorkingSprite(mouseDelta);


	mainLoop();
}

editor::editor(RenderWindow &window, string name) : level(name), edWindow(window), currentTileType(1), interfaceMode(0)
{
	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);

	loadInterace();
	mouseDelta = 0;
	setWorkingSprite(mouseDelta);

	mainLoop();
}



editor::~editor()
{
	
}

void editor::mainLoop() 
{
	edWindow.setMouseCursorVisible(true);
	editorVisable = true;

	while (editorVisable)
	{
		Event handler;
		while (edWindow.pollEvent(handler))
		{
			if (handler.type == Event::Closed)
			{
				edWindow.close();
			}

			if (handler.type == Event::LostFocus)
			{
				edWindow.setFramerateLimit(0);
			}
			else if (handler.type == Event::GainedFocus)
			{
				edWindow.setFramerateLimit(60);
			}

			if (handler.type == Event::MouseWheelMoved)
			{
				mouseDelta += handler.mouseWheel.delta;
				setWorkingSprite(mouseDelta);
			}
			else if (Mouse::isButtonPressed(Mouse::Left))
			{
				//const int countOfTiles = map_level::tileTypes::TILECOUNT;
				const int countOfAccessories = level.getObjectTypesCount();

				if (mouseDelta >= 0 && mouseDelta <= level.tileTextures.size() - 1)
				{
					//cout << "editor.cpp(116), mouseDelta: " << mouseDelta << endl;
					level.setTile(workingSprite, mouseDelta);
				}
				else
				{
					level.setObject(workingSprite, mouseDelta - 19);
				}
			}

			if (handler.type == Event::KeyPressed)
			{
				if (handler.key.code == Keyboard::Escape)
				{
					editorVisable = false;
				}

				
				if (handler.key.code == Keyboard::W)
				{
					screen.move(0, -64);
				}
				else if (handler.key.code == Keyboard::S)
				{
					screen.move(0, 64);
				}
				else if (handler.key.code == Keyboard::A)
				{
					if (workingSprite.getPosition().x <= 0)
					{
					}
					else
					{
						if (screen.getCenter().x - (SCRN_WIDTH / 2) > 0)
						{
							screen.move(-64, 0);
						}
					}
				}
				else if (handler.key.code == Keyboard::D)
				{
					screen.move(64, 0);
				}
			}
		}
		Vector2f mousePos(Mouse::getPosition(edWindow));
		//mousePos.x += screen.getCenter().x - (SCRN_WIDTH / 2);
		//mousePos.y += screen.getCenter().y - (SCRN_HEIGHT / 2);
		//cout << "MouseDelta: " << mouseDelta << endl;

		if (mouseDelta < level.tileTextures.size())
		{
			mousePos.x = int((mousePos.x + screen.getCenter().x - (SCRN_WIDTH / 2)) / 64) * 64;
			mousePos.y = int((mousePos.y + screen.getCenter().y - (SCRN_HEIGHT / 2)) / 64) * 64;
		}
		else if (mouseDelta == interfaceSprites.size() - 1)
		{
			if (level.getNearestAccessory(mousePos))
			{
				mousePos = level.getNearestAccessory(mousePos)->getSprite().getPosition();
			}
		}
		else
		{
			mousePos.x = int(mousePos.x + screen.getCenter().x - (SCRN_WIDTH / 2));
			mousePos.y = int(mousePos.y + screen.getCenter().y - (SCRN_HEIGHT / 2));
		}

		workingSprite.setPosition(mousePos);

		edWindow.clear(Color(149, 192, 247));

		

		edWindow.setView(screen);		
		draw();

		edWindow.display();
	}


}

void editor::setWorkingSprite(int newDelta)
{
	int deltaMax = interfaceSprites.size();
	if (newDelta < 0)
	{
		newDelta = 0;
	}
	else if (newDelta >= deltaMax)
	{
		newDelta = deltaMax - 1;
	}

	workingSprite = interfaceSprites[newDelta];

	mouseDelta = newDelta;
}

void editor::loadInterace()
{
	for (int i = 0; i < level.tileTextures.size(); i++)
	{
		Sprite sprite;
		sprite.setTexture(level.tileTextures[i]);
		interfaceSprites.push_back(sprite);
	}

	for (int i = 0; i < level.allAccessoriesObjects.size(); i++)
	{
		interfaceSprites.push_back(level.allAccessoriesObjects[i]->getSprite());
	}

	interfaceSprites.push_back(interfaceSprites[0]);
}

void editor::draw()
{
	Vector2f screenPosition(screen.getCenter().x - (SCRN_WIDTH / 2), screen.getCenter().y - (SCRN_HEIGHT / 2));
	level.backgroundSprite.setPosition(screenPosition);
	edWindow.draw(level.backgroundSprite);

	for (int i = 0; i < level.getAreaSpritesCount(); i++)
	{
		if (level.getAreaSprite(i).getPosition().x > screenPosition.x - 64)
		{
			if (level.getAreaSprite(i).getPosition().x < screenPosition.x + SCRN_WIDTH + 64)
			{
				edWindow.draw(level.getAreaSprite(i));
			}
		}
	}

	for (int i = 0; i < level.getObjectsCount(); i++)
	{
		if (level.getObjectAt(i)->getSprite().getPosition().x >(screen.getCenter().x - (SCRN_WIDTH / 2)) - 64)
		{
			if (level.getObjectAt(i)->getSprite().getPosition().x < (screen.getCenter().x + (SCRN_WIDTH / 2)) + 64)
			{
				edWindow.draw(level.getObjectAt(i)->getSprite());
			}
		}
	}
	drawInterface();


	edWindow.draw(workingSprite);
}

void editor::drawInterface()
{
	Vector2f positionOfFirst;
	positionOfFirst.x = screen.getCenter().x + (SCRN_WIDTH / 2) - 64;
	positionOfFirst.y = screen.getCenter().y + (mouseDelta * 64);

	for (size_t i = 1; i < interfaceSprites.size(); i++)
	{
		interfaceSprites[i].setPosition(positionOfFirst.x - (i == mouseDelta ? 16 : 0), positionOfFirst.y - (i * 64));
		edWindow.draw(interfaceSprites[i]);
	}
}

string editor::getMapName()
{
	return level.getNameOfLevel();
}

void editor::save(string k)
{
	if (k.size() > 0)
	{
		level.setName(k);
	}
	level.saveMap();
}
