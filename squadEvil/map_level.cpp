#include "map_level.h"
#include "coin.h"
#include "potions.h"
#include "box.h"
#include "chest.h"
#include "campFire.h"
#include "ladder.h"
#include "turtle.h"
#include "warrior.h"
#include "flower.h"
#include "chicken.h"
#include "bush.h"


map_level::map_level() : type("Normal"), height(1), width(1), name("New map")
{

	loadTex();
}

map_level::map_level(string fileName)
{
	name = fileName;
	string file = "data/Levels/" + fileName + ".level";

	fstream myMap;
	myMap.open(file.c_str(), ios::in);

	if (!myMap.is_open())
	{
		cout << "Nie moge otworzyc pliku: " << fileName << endl;
	}
	else
	{
		myMap >> type >> width >> height;
		loadTex();

		resize(width, height);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				myMap >> tileMap[j][i];
				if (tileMap[j][i] != 0)
				{
					sf::Sprite tile;
					tile.setPosition(j * 64, i * 64);
					/*
					if (tileMap[j][i] < 0)
					{
						int k = tileMap[j][i] * -1;
						tile.setTexture(TilesTex_r[k]);
					}
					else
					{
						tile.setTexture(TilesTex[tileMap[j][i]]);
					}
					*/
					tile.setTexture(TilesTex[tileMap[j][i]]);
					tileSprites.push_back(tile);
				}
			}
		}
		
		const int typeCount = 13;
		string types[typeCount] = { "[COIN]", "[CHEST]", "[CAMPFIRE]", "[BOX]", "[LADDER]", "[CHICKEN]", "[WARRIOR]", "[FLOWER]", "[TURTLE]", "[BUSH_1]", "[BUSH_2]", "[BUSH_3]", "[BUSH_4]" };
		while (!myMap.eof())
		{
			sf::Sprite temp;
			string type;
			int x, y;
			myMap >> type;
			for (int i = 0; i < typeCount; i++)
			{
				if (types[i] == type)
				{
					myMap >> x >> y;
					temp.setPosition(x, y);
					temp.setTexture(accessoryTex[i + 1]);
					setAccessories(temp, (i + 1));
					break;
				}
			}
		}

		myMap.close();
	}
}


map_level::~map_level()
{
	for (int i = 0; i < others.size(); i++)
	{
		delete others[i];
	}
}

void map_level::loadTex()
{
	backgroundTex.loadFromFile("data/Graphics/Tileset/" + type + "/BG.png");
	background.setTexture(backgroundTex);
	background.setColor(sf::Color(255, 255, 255, 200));

	int k = 1;
	TilesTex[0].loadFromFile("data/Graphics/Others/empty.png");
	for (size_t i = 1; i < 19; i++)
	{
		TilesTex[i].loadFromFile("data/Graphics/Tileset/Tiles/" + to_string(i) + ".png");
	}
	enum accessoryTiles {
		EMPTY,
		COIN, CHEST, CAMPFIRE, BOX, LADDER,
		CHICKEN, WARRIOR, FLOWER, TURTLE
	};
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

	resize(width, height);
}

void map_level::resize(int x, int y)
{
	if (tileMap.size() <= x)
	{
		tileMap.resize(x + 1); 
		for (int i = 0; i < tileMap.size(); i++)
		{
			tileMap[i].resize(tileMap[0].size());
		}
	}
	if (tileMapHeaven.size() <= x)
	{
		tileMapHeaven.resize(x + 1);
		for (int i = 0; i < tileMapHeaven.size(); i++)
		{
			tileMapHeaven[i].resize(tileMapHeaven[0].size());
		}
	}

	if (y >= 0)
	{
		if (tileMap[0].size() <= y)
		{
			for (int i = 0; i < tileMap.size(); i++)
			{
				tileMap[i].resize(y + 1);
			}
		}
	}
	else if (y < 0)
	{
		y *= -1;
		if (tileMapHeaven[0].size() <= y)
		{
			for (int i = 0; i < tileMapHeaven.size(); i++)
			{
				tileMapHeaven[i].resize(y + 1);
			}
		}
	}
}

void map_level::setTile(sf::Sprite current, int type)
{
	int i = current.getPosition().x / 64;
	int j = current.getPosition().y / 64;

	resize(i, j);
	
	if (j >= 0)
	{
		if ((tileMap[i][j] == 1 && type == 1) || (tileMap[i][j] == 10 && type == 10))
		{
			for (int k = j; k < tileMap[i].size(); k++)
			{
				sf::Sprite under;
				under.setTexture(TilesTex[type]);
				under.setPosition(i * 64, k * 64);
				tileSprites.push_back(under);

				tileMap[i][k] = type;
			}
		}
		else if (type == 0)
		{
			for (int i = 0; i < tileSprites.size(); i++)
			{
				if (current.getPosition() == tileSprites[i].getPosition())
				{
					tileSprites.erase(tileSprites.begin() + i);
					type = 0;
					break;
				}
			}
		}
		else
		{
			if (tileMap[i][j] != 0)
			{
				for (int k = 0; k < tileSprites.size(); k++)
				{
					if (tileSprites[k].getPosition() == current.getPosition())
					{
						tileSprites.erase(tileSprites.begin() + k);
						break;
					}
				}
			}
			tileSprites.push_back(current);
		}
		tileMap[i][j] = type;
	}
	else
	{
		j *= -1;
		if (type == 0)
		{
			for (int i = 0; i < tileSprites.size(); i++)
			{
				if (current.getPosition() == tileSprites[i].getPosition())
				{
					tileSprites.erase(tileSprites.begin() + i);
					type = 0;
					break;
				}
			}
		}
		else
		{
			if (tileMapHeaven[i][j] != 0)
			{
				for (int k = 0; k < tileSprites.size(); k++)
				{
					if (tileSprites[k].getPosition() == current.getPosition())
					{
						tileSprites.erase(tileSprites.begin() + k);
						break;
					}
				}
			}
			tileSprites.push_back(current);
		}
		tileMapHeaven[i][j] = type;
	}

	if (type == -1 || type == -10)
	{
		type *= -1;
		setTile(current, type);
	}

	
}

void map_level::setAccessories(sf::Sprite current, int type)
{
	accessories *temp;
	switch (accessoryTiles(type))
	{
	case map_level::EMPTY:
		for (int i = 0; i < others.size(); i++)
		{
			if (current.getPosition() == others[i]->sprite.getPosition())
			{
				delete others[i];
				others.erase(others.begin() + i);
				break;
			}
		}
		break;
	case map_level::COIN:
		temp = new coin(current, "[COIN]");
		others.push_back(temp);
		break;
	case map_level::CHEST:
		temp = new chest(current, "[CHEST]");
		others.push_back(temp);
		break;
	case map_level::CAMPFIRE:
		temp = new campFire(current, "[CAMPFIRE]");
		others.push_back(temp);
		break;
	case map_level::BOX:
		temp = new box(current, "[BOX]");
		others.push_back(temp);
		break;
	case map_level::LADDER:
		temp = new ladder(current, "[LADDER]");
		others.push_back(temp);
		break;
	case map_level::CHICKEN:
		temp = new chicken(current, "[CHICKEN]");
		others.push_back(temp);
		break;
	case map_level::WARRIOR:
		temp = new warrior(current, "[WARRIOR]");
		dynamic_cast <warrior *>(temp)->setMap(tileMap);
		others.push_back(temp);
		break;
	case map_level::FLOWER:
		temp = new flower(current, "[FLOWER]");
		others.push_back(temp);
		break;
	case map_level::TURTLE:
		temp = new turtle(current, "[TURTLE]");
		others.push_back(temp);
		break;
	case BUSH_1:
		temp = new bush(current, "[BUSH_1]");
		others.push_back(temp);
		break;
	case BUSH_2:
		temp = new bush(current, "[BUSH_2]");
		others.push_back(temp);
		break;
	case BUSH_3:
		temp = new bush(current, "[BUSH_3]");
		others.push_back(temp);
		break;
	case BUSH_4:
		temp = new bush(current, "[BUSH_4]");
		others.push_back(temp);
		break;
	default:
		break;
	}
}

int map_level::getTileType(sf::Vector2f position)
{
	int i = position.x / 64;
	int j = position.y / 64;

	resize(i, j);
	return tileMap[i][j];
}

void map_level::setHeight(int height)
{
	int x = tileMap.size();
	int y = (height / 64) + 1;
	resize(x, y);
}

void map_level::updateMap()
{
	for (int i = 0; i < tileSprites.size(); i++)
	{
		int tileType = getTileType(tileSprites[i].getPosition());
		tileSprites[i].setTexture(TilesTex[tileType]);
	}
}

sf::Vector2i map_level::mapSize()
{
	return sf::Vector2i(width, height);
}

accessories* map_level::getNearestAccessory(Vector2f mousePos)
{
	accessories* temp = 0;
	float range = 32;
	for (int i = 0; i < others.size(); i++)
	{
		Vector2f pos = others[i]->sprite.getPosition();
		if (abs(mousePos.x - pos.x) < range)
		{
			if (abs(mousePos.y - pos.y) < range)
			{
				temp = others[i];
				break;
			}
		}
	}
	return temp;
}