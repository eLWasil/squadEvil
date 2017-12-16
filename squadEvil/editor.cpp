#include "editor.h"
#include "accessories.h"
#include "SFML\Graphics\Sprite.hpp"

const int accessoriesTileMove = 16;
const int areaTileMove = 64;
bool editorVisable;
bool reverseTile = false, darkMode = false;

editor::editor(RenderWindow &window) : edWindow(window), currentTileType(1), interfaceMode(0), countSprite(33)
{
	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);
	mouseDelta = 0;

	level.setHeight(SCRN_HEIGHT);

	loadTex();
}

editor::editor(RenderWindow &window, string name) : level(name), edWindow(window), currentTileType(1), interfaceMode(0), countSprite(33)
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
}

void editor::loadTex()
{
	for (int i = 0; i < countSprite; i++)
	{
		if (i < 19)
		{
			interfaceTiles[i].setTexture(level.TilesTex[i]);
			allSprites[i].setTexture(level.TilesTex[i]);
		}
		else
		{
			interfaceTiles[i].setTexture(level.accessoryTex[i - 19]);
			allSprites[i].setTexture(level.accessoryTex[i - 19]);
		}
	}

	widthInterfaceTile = SCRN_WIDTH / 16;
	dist = ((SCRN_WIDTH / 16) - 64) / 2;


	for (int i = 0; i < 10; i++)
	{
		accessoriesTiles[i].setTexture(level.accessoryTex[i]);
		accessoriesTiles[i].setPosition(-64, -64);
		accessoriesTilesInterface[i].setTexture(level.accessoryTex[i]);
	}

	currentAccessory = &allSprites[mouseDelta];

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
				else if (mouseDelta >= countSprite)
				{
					mouseDelta = countSprite - 1;
				}
				currentAccessory = &allSprites[mouseDelta];
			}
			else if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (mouseDelta >= 0 && mouseDelta <= 18)
				{
					level.setTile(*currentAccessory, mouseDelta);
				}
				else
				{
					level.setAccessories(*currentAccessory, mouseDelta - 19);
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
					if (currentAccessory->getPosition().x <= 0)
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

		if (mouseDelta <= 18)
		{
			mousePos.x = int((mousePos.x - currentAccessory->getGlobalBounds().width / 2) / 64) * 64;
			mousePos.y = int((mousePos.y - currentAccessory->getGlobalBounds().height / 2) / 64) * 64;
		}
		else if (mouseDelta == 19)
		{
			if (level.getNearestAccessory(mousePos))
			{
				mousePos = level.getNearestAccessory(mousePos)->sprite.getPosition();
			}
		}

		currentAccessory->setPosition(mousePos);


		edWindow.clear(Color(149, 192, 247));

		

		edWindow.setView(screen);		
		draw();

		edWindow.display();
	}


}

void editor::draw()
{
	Vector2f screenPosition(screen.getCenter().x - (SCRN_WIDTH / 2),
	screen.getCenter().y - (SCRN_HEIGHT / 2));
	level.background.setPosition(screenPosition);
	edWindow.draw(level.background);

	for (int i = 0; i < level.tileSprites.size(); i++)
	{
		if (level.tileSprites[i].getPosition().x > screenPosition.x - 64)
		{
			if (level.tileSprites[i].getPosition().x < screenPosition.x + SCRN_WIDTH + 64)
			{
				edWindow.draw(level.tileSprites[i]);
			}
		}
	}

	for (int i = 0; i < level.others.size(); i++)
	{
		if (level.others[i]->sprite.getPosition().x >(screen.getCenter().x - (SCRN_WIDTH / 2)) - 64)
		{
			if (level.others[i]->sprite.getPosition().x < (screen.getCenter().x + (SCRN_WIDTH / 2)) + 64)
			{
				edWindow.draw(level.others[i]->sprite);
			}
		}
	}
	drawInterface();


	edWindow.draw(*currentAccessory);
}

void editor::drawInterface()
{
	Vector2f positionOfFirst;
	positionOfFirst.x = screen.getCenter().x + (SCRN_WIDTH / 2) - 64;
	positionOfFirst.y = screen.getCenter().y + (mouseDelta * 64);

	for (size_t i = 0; i < countSprite; i++)
	{
		interfaceTiles[i].setPosition(positionOfFirst.x - (i == mouseDelta ? 16 : 0), positionOfFirst.y - (i * 64));
		edWindow.draw(interfaceTiles[i]);
	}
}

string editor::getMapName()
{
	return level.name;
}

void editor::save(string k)
{

	if (k.size() > 0)
	{
		level.name = k;
	}

	string name = "data/Levels/" + level.name + ".level";
	fstream myMap;
	myMap.open(name.c_str(), ios::out);

	if (!myMap.is_open())
	{
		cout << "Blad, nie mozna otworzyc pliku!\n" << name << endl;
	}

	Vector2i mapSizes(level.tileMap.size(), level.tileMap[0].size());
	Vector2i mapHeavenSizes(level.tileMapHeaven.size(), level.tileMapHeaven[0].size());

	myMap << level.type << "\n";
	myMap << mapSizes.x << " " << (mapSizes.y + mapHeavenSizes.y) << "\n";

	for (int i = mapHeavenSizes.y - 1; i >= 0 ; i--)
	{
		for (int j = 0; j < mapHeavenSizes.x; j++)
		{
			myMap << level.tileMapHeaven[j][i] << " ";
		}
		myMap << "\n";
	}

	for (int i = 0; i < mapSizes.y; i++)
	{
		for (int j = 0; j < mapSizes.x; j++)
		{
			myMap << level.tileMap[j][i] << " ";
		}
		myMap << "\n";
	}

	if (level.others.size() > 10)
	{
		quicksort(level.others, 0, level.others.size() - 1);
	}

	for (int i = 0; i < level.others.size(); i++)
	{
		int x = level.others[i]->sprite.getPosition().x;
		int y = level.others[i]->sprite.getPosition().y;
		if (mapHeavenSizes.y > 0)
		{
			y += (mapHeavenSizes.y * 64);
		}
		myMap << level.others[i]->getName() << " " << x << " " << y << "\n";
	}

	myMap.close();
}

void editor::quicksort(vector <accessories*> &Array, int left, int right)
{
	int i = left;
	int j = right;
	int x = Array[(left + right) / 2]->sprite.getPosition().x;
	
	do
	{
		while (Array[i]->sprite.getPosition().x < x)
		{
			i++;
		}
		while (Array[j]->sprite.getPosition().x > x)
		{
			j--;
		}
		if (i <= j)
		{
			accessories *temp = Array[i];
			Array[i] = Array[j];
			Array[j] = temp;

			i++;
			j--;
		}
	} while (i <= j);

	if (left < j)
	{
		quicksort(Array, left, j);
	}
	if (right > i)
	{
		quicksort(Array, i, right);
	}
}