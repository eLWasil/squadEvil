#include "editor.h"
#include "accessories.h"
#include "SFML\Graphics\Sprite.hpp"

const int accessoriesTileMove = 16;
const int areaTileMove = 64;
bool editorVisable;
bool reverseTile = false, darkMode = false;

editor::editor(RenderWindow &window, string filename) : edWindow(window), interfaceMode(0), mouseDelta(0)
{
	SCRN_WIDTH = window.getSize().x;
	SCRN_HEIGHT = window.getSize().y;
	screen.setSize(SCRN_WIDTH, SCRN_HEIGHT);
	screen.setCenter(SCRN_WIDTH / 2, SCRN_HEIGHT / 2);


	loadInterace();
	workingSprite = interfaceSprites[0];

	if (filename.size() > 0)
	{
		level.readMap(filename);
	}

}

editor::~editor()
{
	
}

void editor::mainLoop() 
{
	int firstUsableObjectNumber = TexNames::EMPTY;
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
					mouseDelta = interfaceSprites.size() - 1;
				}
				else if (mouseDelta >= interfaceSprites.size())
				{
					mouseDelta = 0;
				}

				Vector2f pos = workingSprite.getPosition();
				workingSprite = interfaceSprites[mouseDelta];
				workingSprite.setPosition(pos);
			}
			else if (Mouse::isButtonPressed(Mouse::Left))
			{
				level.setObject(workingSprite, mouseDelta + firstUsableObjectNumber);
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

		int selectedObjectType = mouseDelta + firstUsableObjectNumber;

		if (selectedObjectType >= TexNames::TILESTOTHEEND)
		{
			mousePos.x = int((mousePos.x + screen.getCenter().x - (SCRN_WIDTH / 2)) / 64) * 64;
			mousePos.y = int((mousePos.y + screen.getCenter().y - (SCRN_HEIGHT / 2)) / 64) * 64;
		}
		else if (selectedObjectType == TexNames::EMPTY)
		{
			mousePos = level.getNearestObjectPosition(mousePos);
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

void editor::loadInterace()
{
	int firstUsableObjectNumber = TexNames::EMPTY;

	interfaceSprites.resize(TexNames::TILESTOTHEEND + level.numOfTileTypes);
	for (int i = 0; i < interfaceSprites.size(); i++)
	{
		Vector2f newpos(SCRN_WIDTH - 64 - 32, i * 64 + 128);
		interfaceSprites[i].setPosition(newpos);
		interfaceSprites[i].setTexture(level.getTexture(i + firstUsableObjectNumber));
	}

	background.setTexture(level.getTexture(0));
}

void editor::draw()
{
	Vector2f screenPosition(screen.getCenter().x - (SCRN_WIDTH / 2), screen.getCenter().y - (SCRN_HEIGHT / 2));
	background.setPosition(screenPosition);
	
	edWindow.draw(background);

	drawTiles();
	drawObjects();
	drawEnemies();
	
	edWindow.draw(workingSprite);

	drawInterface();
}

void editor::drawTiles()
{
	int idx = 0;
	Sprite *temp = nullptr;
	Vector2f drawingArea(screen.getCenter().x - (SCRN_WIDTH / 2) - 128, screen.getCenter().x + (SCRN_WIDTH / 2) + 128); // 128 mean 2x TILESIZE, safe border
	while (true)
	{
		temp = level.getTileAt(idx++);

		if (!temp) break;

		if (temp->getPosition().x > drawingArea.x);
		{
			if (temp->getPosition().x < drawingArea.y)
			{
				edWindow.draw(*temp);
			}
			else
			{
				break;
			}
		}
	}
}
void editor::drawObjects()
{
	int idx = 0;
	accessories *temp = nullptr;
	Vector2f drawingArea(screen.getCenter().x - (SCRN_WIDTH / 2) - 128, screen.getCenter().x + (SCRN_WIDTH / 2) + 128); // 128 mean 2x TILESIZE, safe border

	while (true)
	{
		temp = level.getObjectAt(idx++);

		if (!temp) break;

		if (temp->getSprite().getPosition().x > drawingArea.x);
		{
			if (temp->getSprite().getPosition().x < drawingArea.y)
			{
				edWindow.draw(temp->getSprite());
			}
			else
			{
				break;
			}
		}
	}
}
void editor::drawEnemies()
{
	int idx = 0;
	enemies *temp = nullptr;
	Vector2f drawingArea(screen.getCenter().x - (SCRN_WIDTH / 2) - 128, screen.getCenter().x + (SCRN_WIDTH / 2) + 128); // 128 mean 2x TILESIZE, safe border
	while (true)
	{
		temp = level.getEnemieAt(idx++);

		if (!temp) break;

		if (temp->getSprite().getPosition().x > drawingArea.x);
		{
			if (temp->getSprite().getPosition().x < drawingArea.y)
			{
				edWindow.draw(temp->getSprite());
			}
			else
			{
				break;
			}
		}
	}
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
	return level.getMapName();
}

void editor::saveMap(string title)
{
	level.saveMap(title);
}
