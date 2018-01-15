#include "map_of_level.h"
#include "coin.h"
#include "chest.h"
#include "campFire.h"
#include "box.h"
#include "ladder.h"
#include "warrior.h"
#include "chicken.h"
#include "bush.h"
#include "plate.h"

#define TILESIZE 64 //pixels


map_of_level::map_of_level() : nameOfLevel("New map"), typeOfMap("Normal")
{
	backgroundBuilder();
	tileTexLoader();
	loadObjectsToVector();
	resizeMap(20, 40);

}

map_of_level::map_of_level(string fileName)
{
	nameOfLevel = fileName;
	string file = "data/Levels/" + fileName + ".level";

	fstream myMap;
	myMap.open(file.c_str(), ios::in);

	if (!myMap.is_open())
	{
		cout << "Nie moge otworzyc pliku: " << fileName << endl;
	}
	else
	{
		int columns, rows;
		myMap >> typeOfMap >> rows >> columns;
		resizeMap(rows, columns);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				myMap >> TileMapInNumbers[i][j];
			}
		}

		while (!myMap.eof())
		{
			int type, x, y;
			myMap >> type >> x >> y;
			sf::Vector2f pos(x, y);

			accessories* temp = CreateObjectByTypeNumber(type);
			if (temp != nullptr)
			{
				temp->setPosition(pos);
				objectsVector.push_back(temp);
			}
		}
		myMap.close();

		backgroundBuilder();
		tileTexLoader();
		areaBuilder();
	}
	loadObjectsToVector();
}


map_of_level::~map_of_level()
{
	for (int i = 0; i < objectsVector.size(); i++)
	{
		delete objectsVector[i];
	}

	for (int i = 0; i < allAccessoriesObjects.size(); i++)
	{
		delete allAccessoriesObjects[i];
	}
}

void map_of_level::backgroundBuilder()
{
	backgroundSprite.setPosition(0, 0);
	backgroundTex.loadFromFile("data/Graphics/Tileset/" + typeOfMap + "/BG.png");
	backgroundSprite.setTexture(backgroundTex);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 200));
}

void map_of_level::tileTexLoader()
{
	Texture temp;
	temp.loadFromFile("data/Graphics/Others/empty.png");

	tileTextures.push_back(temp);

	for (size_t i = 1; i < 19; i++)
	{
		Texture tmp;
		tmp.loadFromFile("data/Graphics/Tileset/Tiles/" + to_string(i) + ".png");
		tileTextures.push_back(tmp);
	}
}

void map_of_level::areaBuilder()
{
	for (int i = 0; i < TileMapInNumbers.size(); i++)
	{
		for (int j = 0; j < TileMapInNumbers[0].size(); j++)
		{
			//cout << i << ", " << j << endl;
			if (TileMapInNumbers[i][j])
			{
				Sprite sprite;
				sprite.setPosition(j*TILESIZE, i*TILESIZE);
				sprite.setTexture(tileTextures[TileMapInNumbers[i][j]]);

				mapAreaSprites.push_back(sprite);
			}
		}
	}
	if (mapAreaSprites.size() > 20)
	{
		sortTiles(0, mapAreaSprites.size() - 1);
	}
}

void map_of_level::loadObjectsToVector()
{
	for (int i = 0; i < accessoryTypes::COUNT; i++)
	{
		allAccessoriesObjects.push_back(CreateObjectByTypeNumber(i));
	}
}

void map_of_level::resizeMap(int numberOfRows, int numberOfColumns)
{
	if (numberOfRows > 0)
	{
		if (TileMapInNumbers.size() < numberOfRows)
		{
			TileMapInNumbers.resize(numberOfRows + 1);
			for (int i = 0; i < TileMapInNumbers.size(); i++)
			{
				TileMapInNumbers[i].resize(TileMapInNumbers[0].size());
			}
		}
	}
	else if (numberOfRows < 0)
	{
		while (numberOfRows++ < 0)
		{
			vector <int> newBeginVec(TileMapInNumbers[0].size(), 0);
			TileMapInNumbers.insert(TileMapInNumbers.begin(), newBeginVec);
		}
	}
	

	if (numberOfColumns > 0)
	{
		if (TileMapInNumbers[0].size() < numberOfColumns)
		{
			for (int i = 0; i < TileMapInNumbers.size(); i++)
			{
				TileMapInNumbers[i].resize(numberOfColumns + 1);
			}
		}
	}
	else if (numberOfColumns < 0)
	{
		for (int i = 0; i < TileMapInNumbers.size(); i++)
		{
			TileMapInNumbers[i].insert(TileMapInNumbers[i].begin(), 0);
		}
	}
}

accessories* map_of_level::CreateObjectByTypeNumber(int type)
{
	accessories* temp;
	switch (accessoryTypes(type))
	{
	case map_of_level::COIN:
		temp = new coin();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::CHEST:
		temp = new chest();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::CAMPFIRE:
		temp = new campFire();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::BOX:
		temp = new box();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::LADDER:
		temp = new ladder();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::CHICKEN:
		temp = new chicken();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::WARRIOR:
		temp = new warrior();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::BUSH:
		temp = new bush();
		temp->setObjectTypeNumber(type);
		break;
	case map_of_level::ENDOFLEVEL:
		temp = new plate();
		temp->setObjectTypeNumber(type);
		break;
	default:
		temp = 0;
		break;
	}
	return temp;
}

const int map_of_level::getTileType(Vector2f pos)
{
	int row = pos.y / 64;
	int col = pos.x / 65;

	return TileMapInNumbers[row][col];
}

void map_of_level::deleteObjectAt(int index)
{
	delete objectsVector[index];
	objectsVector.erase(objectsVector.begin() + index);
}

void map_of_level::setTile(Sprite current, int type)
{
	int row = current.getPosition().y / 64;
	int col = current.getPosition().x / 64;
	resizeMap(row, col);


	if (type > 0)
	{
		TileMapInNumbers[row][col] = type;
		mapAreaSprites.push_back(current);
	}
	else
	{
		TileMapInNumbers[row][col] = 0;
		for (int i = 0; i < mapAreaSprites.size(); i++)
		{
			if (mapAreaSprites[i].getPosition() == current.getPosition())
			{
				mapAreaSprites.erase(mapAreaSprites.begin() + i);
			}
		}
	}
}

void map_of_level::setObject(Sprite current, int type)
{
	//cout << "type: " << type << endl;
	if (type > 0)
	{
		accessories* temp = CreateObjectByTypeNumber(type);
		temp->setPosition(current.getPosition());
		objectsVector.push_back(temp);
	}
	else if (type == map_of_level::COUNT)
	{
		for (int i = 0; i < objectsVector.size(); i++)
		{
			if (current.getPosition() == objectsVector[i]->getSprite().getPosition())
			{
				delete objectsVector[i];
				objectsVector.erase(objectsVector.begin() + i);
				break;
			}
		}
	}
}

accessories* map_of_level::getNearestAccessory(Vector2f mousePos)
{
	accessories* temp = 0;
	float range = 32;	
	for (int i = 0; i < objectsVector.size(); i++)
	{
		Vector2f pos = objectsVector[i]->getSprite().getPosition();
		if (abs(mousePos.x - pos.x) < range)
		{
			if (abs(mousePos.y - pos.y) < range)
			{
				temp = objectsVector[i];
				break;
			}
		}
	}
	return temp;
}

void map_of_level::sortObjects(int left, int right)
{
	int i = left;
	int j = right;
	int x = objectsVector[(left + right) / 2]->getSprite().getPosition().x;

	do
	{
		while (objectsVector[i]->getSprite().getPosition().x < x)
		{
			i++;
		}
		while (objectsVector[j]->getSprite().getPosition().x > x)
		{
			j--;
		}
		if (i <= j)
		{
			accessories *temp = objectsVector[i];
			objectsVector[i] = objectsVector[j];
			objectsVector[j] = temp;

			i++;
			j--;
		}
	} while (i <= j);

	if (left < j)
	{
		sortObjects(left, j);
	}
	if (right > i)
	{
		sortObjects(i, right);
	}
} 

void map_of_level::sortTiles(int left, int right)
{
	int i = left;
	int j = right;

	int x = mapAreaSprites[(left + right) / 2].getPosition().x;

	do
	{
		while (mapAreaSprites[i].getPosition().x < x)
		{
			i++;
		}
		while (mapAreaSprites[j].getPosition().x > x)
		{
			j--;
		}
		if (i <= j)
		{
			Sprite temp = mapAreaSprites[i];
			mapAreaSprites[i] = mapAreaSprites[j];
			mapAreaSprites[j] = temp;

			i++;
			j--;
		}
	} while (i <= j);

	if (left < j)
	{
		sortTiles(left, j);
	}
	if (right > i)
	{
		sortTiles(i, right);
	}
}

void map_of_level::saveMap()
{
	string name = "data/Levels/" + nameOfLevel + ".level";

	fstream myMap;
	myMap.open(name.c_str(), ios::out);

	if (!myMap.is_open())
	{
		cout << "Nie mozna otworzyc pliku: " << name << endl;
	}
	else
	{
		myMap << typeOfMap << endl;
		myMap << TileMapInNumbers.size() << " " << TileMapInNumbers[0].size() << endl;

		for (int i = 0; i < TileMapInNumbers.size(); i++)
		{
			for (int j = 0; j < TileMapInNumbers[0].size(); j++)
			{
				myMap << TileMapInNumbers[i][j] << " ";
			}
			myMap << endl;
		}

		if (objectsVector.size())
		{
			sortObjects(0, objectsVector.size() - 1);

			for (int i = 0; i < objectsVector.size(); i++)
			{
				myMap << objectsVector[i]->getObjectTypeNumber() << " " << objectsVector[i]->getSprite().getPosition().x
					<< " " << objectsVector[i]->getSprite().getPosition().y << endl;
			}
		}
		
		myMap.close();
	}
}