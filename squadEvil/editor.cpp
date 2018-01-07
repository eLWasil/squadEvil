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
	mouseDelta = 0;

	loadTex();
}

editor::editor(RenderWindow &window, string name) : level(name), edWindow(window), currentTileType(1), interfaceMode(0)
{
	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);
	mouseDelta = 0;

	loadTex();
}



editor::~editor()
{
	delete workingSprite;
}

void editor::loadTex()
{
	for (int i = 0; i < level.tileTextures.size(); i++)
	{
		Sprite sprite;
		sprite.setTexture(level.tileTextures[i]);

		interfaceSprites.push_back(sprite);
	}
	level.loadObjectsSprites();

	accessoryTex[0].loadFromFile("data/Graphics/Others/empty.png");
	accessoryTex[1].loadFromFile("data/Graphics/Others/Object/coin.png");
	accessoryTex[2].loadFromFile("data/Graphics/Others/Object/chests.png", sf::IntRect(0, 0, 32, 32));
	accessoryTex[3].loadFromFile("data/Graphics/Others/Object/CampFire.png", sf::IntRect(0, 0, 64, 64));
	accessoryTex[4].loadFromFile("data/Graphics/Others/Object/Crate.png");
	accessoryTex[5].loadFromFile("data/Graphics/Others/Object/ladder.png");
	accessoryTex[6].loadFromFile("data/Graphics/Others/enemies/angry_chicken.png");
	accessoryTex[7].loadFromFile("data/Graphics/Others/enemies/warrior.png");
	accessoryTex[8].loadFromFile("data/Graphics/Others/enemies/flower.png");
	accessoryTex[9].loadFromFile("data/Graphics/Others/enemies/turtle.png");
	accessoryTex[10].loadFromFile("data/Graphics/Others/Object/Bush (1).png");
	accessoryTex[11].loadFromFile("data/Graphics/Others/Object/Bush (2).png");
	accessoryTex[12].loadFromFile("data/Graphics/Others/Object/Bush (3).png");
	accessoryTex[13].loadFromFile("data/Graphics/Others/Object/Bush (4).png");

	for (int i = 0; i < level.allAccessoriesSprites.size(); i++)
	{
		Sprite sprite;
		sprite.setTexture(accessoryTex[i]);
		interfaceSprites.push_back(sprite);
	}

	workingSprite = new Sprite(interfaceSprites[0]);
	mainLoop();
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
				if (mouseDelta < 0)
				{
					mouseDelta = 0;
				}
				else if (mouseDelta >= interfaceSprites.size())
				{
					mouseDelta = interfaceSprites.size() - 1;
				}
				*workingSprite = interfaceSprites[mouseDelta];
			}
			else if (Mouse::isButtonPressed(Mouse::Left))
			{
				//const int countOfTiles = map_level::tileTypes::TILECOUNT;
				const int countOfAccessories = level.getObjectTypesCount();

				if (mouseDelta >= 0 && mouseDelta <= level.tileTextures.size() - 1)
				{
					//cout << "editor.cpp(116), mouseDelta: " << mouseDelta << endl;
					level.setTile(*workingSprite, mouseDelta);
				}
				else
				{
					level.setObject(*workingSprite, mouseDelta - 19);
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
					if (workingSprite->getPosition().x <= 0)
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

		if (mouseDelta < 19)
		{
			mousePos.x = int((mousePos.x + screen.getCenter().x - (SCRN_WIDTH / 2)) / 64) * 64;
			mousePos.y = int((mousePos.y + screen.getCenter().y - (SCRN_HEIGHT / 2)) / 64) * 64;
		}
		else if (mouseDelta == 19)
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

		workingSprite->setPosition(mousePos);

		edWindow.clear(Color(149, 192, 247));

		

		edWindow.setView(screen);		
		draw();

		edWindow.display();
	}


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


	edWindow.draw(*workingSprite);
}

void editor::drawInterface()
{
	Vector2f positionOfFirst;
	positionOfFirst.x = screen.getCenter().x + (SCRN_WIDTH / 2) - 64;
	positionOfFirst.y = screen.getCenter().y + (mouseDelta * 64);

	for (size_t i = 0; i < interfaceSprites.size(); i++)
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
