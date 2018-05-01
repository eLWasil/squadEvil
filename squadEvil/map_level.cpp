// File structure:
// 1. Map header
// 2. numOfObjects x (filemapObject)
// 3. tiles as filemapObjects to the end 
//	  but tilesTypeMap[x][y] = type

#define _CRT_SECURE_NO_WARNINGS

#include "map_level.h"
#include "coin.h"
#include "potions.h"
#include "box.h"
#include "chest.h"
#include "campFire.h"
#include "ladder.h"
#include "warrior.h"
#include "chicken.h"
#include "bush.h"
#include "plate.h"

//accessories

map_level::map_level() : numOfTileTypes(18), TILESIZE(64)
{
	loadTextures();
	
	mapName = "My Map";
	
	mapHeader.mapWidthAsTiles = 40;
	mapHeader.mapHeightAsTiles = 20;
	mapHeader.numOfObjets = 0;
	mapHeader.numOfTitleChars = mapName.size();

	buildMapBody();
}

map_level::map_level(string fileMapName) : numOfTileTypes(18), TILESIZE(64)
{
	loadTextures();
	readMap(fileMapName);
}

map_level::~map_level() 
{
	for (int j = 0; j < enemiesVector.size(); j++)
	{
		enemiesVector[j].reset();
	}

	for (int i = 0; i < objectsVector.size(); i++)
	{
		objectsVector[i].reset();
	}
}

Texture & map_level::getTexture(int idx)
{
	if (idx >= textures.size())
	{
		return textures[textures.size() - 1];
	}
	else if (idx < 0)
	{
		return textures[0];
	}
	else
	{
		return textures[idx];
	}
}

void map_level::buildMapBody()
{
	tilesTypeMap.resize(mapHeader.mapWidthAsTiles);
	for (int col = 0; col < mapHeader.mapWidthAsTiles; col++)
	{
		tilesTypeMap[col].resize(mapHeader.mapHeightAsTiles, 0);
	}
}

void map_level::readMap(string fileName)
{
	string dir = filemenager.getCurrentPath();
	dir += "\\data\\Levels\\";
	string fPath = dir + fileName;

	ifstream file(fPath, ios::binary);
	if (!file.is_open())
	{
		cout << "Nie moge otworzyc pliku " << fPath << endl;
		return;
	}

	char *fmHeader = new char[sizeof(mapFileHeader)];
	file.read(fmHeader, sizeof(mapFileHeader));
	mapHeader = *(mapFileHeader *)(fmHeader);
	delete fmHeader;

	buildMapBody();
	
	mapName.resize(mapHeader.numOfTitleChars);
	file.read((char *)&mapName, mapHeader.numOfTitleChars);

	int k = 0;
	char *temp = new char[sizeof(filemapObject)];
	while (!file.eof())
	{
		k++;
		file.read(temp, sizeof(filemapObject));

		filemapObject *object = (filemapObject *)temp;

		if (isObjectCorrect(object))
		{
			setObject(object);
		}
	}
	delete temp;

	file.close();
}

bool map_level::isObjectCorrect(filemapObject *test)
{
	if (test->type < 0)
	{
		return false;
	}

	if (test->x < 0)
	{
		return false;
	}

	if (test->y < 0)
	{
		return false;
	}

	return true;
}

void map_level::saveMap(string title)
{
	mapName = title;
	string fName = makeFileName();
	string dir = filemenager.getCurrentPath();
	dir += "\\data\\Levels\\";
	string fPath = dir + fName;

	synchronizeMap();

	ofstream file(fPath, ios::binary | ios::out | ios::trunc);
	
	if (!file.is_open())
	{
		cout << "Nie moge otworzyc pliku " << fPath << endl;
		return;
	}
	file.write((char *)(&mapHeader), sizeof(mapFileHeader));
	

	file.write((char *)mapName.c_str(), strlen(mapName.c_str()));

	for (int i = 0; i < objectsToFileSave.size(); i++)
	{
		file.write((char *)&objectsToFileSave[i], sizeof(filemapObject));
	}
	
	//SAVING SORTED BY POSITION X 
	for (int i = 0; i < tilesTypeMap.size(); i++)
	{
		for (int j = 0; j < tilesTypeMap[i].size(); j++)
		{
			if (tilesTypeMap[i][j] != 0)
			{
				filemapObject *temp = new filemapObject(tilesTypeMap[i][j], i * TILESIZE, j * TILESIZE);
				file.write((char *)temp, sizeof(filemapObject));
				delete temp;
			}
		}
	}

	file.close();
}

void map_level::setObject(filemapObject *object)
{
	Sprite temp;
	temp.setPosition(object->x, object->y);
	temp.setTexture(getTexture(object->type));
	setObject(temp, object->type);
}

Vector2f map_level::getNearestObjectPosition(Vector2f mousePos)
{
	//accessories *temp = nullptr;
	int range = 32;
	bool founded = false;
	Vector2f position(mousePos);
	itemToDeleteXVecYIdx = Vector2i(4, 0);

	int idx = 0;
	for (auto &tile : tileSpritesVector)
	{
		Vector2f pos = tile.getPosition();

		if (abs(mousePos.x - pos.x) < range)
		{
			if (abs(mousePos.y - pos.y) < range)
			{
				position = tile.getPosition();
				founded = true;
				
				itemToDeleteXVecYIdx = Vector2i(0, idx);
				break;
			}
		}
		idx++;
	}
	if (!founded)
	{
		idx = 0;
		for (auto &accessories : objectsVector)
		{
			Vector2f pos = accessories->getSprite().getPosition();
			if (abs(mousePos.x - pos.x) < range)
			{
				if (abs(mousePos.y - pos.y) < range)
				{
					position = pos;
					founded = true;
					itemToDeleteXVecYIdx = Vector2i(1, idx);
					break;
				}
			}
			idx++;
		}
	}
	
	if (!founded)
	{
		idx = 0;
		for (auto &enemies : enemiesVector)
		{
			Vector2f pos = enemies->getSprite().getPosition();
			if (abs(mousePos.x - pos.x) < range)
			{
				if (abs(mousePos.y - pos.y) < range)
				{
					position = pos;
					itemToDeleteXVecYIdx = Vector2i(2, idx);
					break;
				}
			}
			idx++;
		}
	}
	return position;
}

void map_level::deleteObject(Vector2f mousePos)
{
	Vector2f position = getNearestObjectPosition(mousePos);
	if (itemToDeleteXVecYIdx.x == 0)
	{
		Vector2i inArrayPos(position.x / TILESIZE, position.y / TILESIZE);
		tilesTypeMap[inArrayPos.x][inArrayPos.y] = 0;
		tileSpritesVector.erase(tileSpritesVector.begin() + itemToDeleteXVecYIdx.y);
	}
	else if (itemToDeleteXVecYIdx.x == 1)
	{
		objectsVector.erase(objectsVector.begin() + itemToDeleteXVecYIdx.y);
	}
	else if (itemToDeleteXVecYIdx.x == 2)
	{
		enemiesVector.erase(enemiesVector.begin() + itemToDeleteXVecYIdx.y);
	}

	if (itemToDeleteXVecYIdx.x > 0 && itemToDeleteXVecYIdx.x < 4)
	{
		for (int i = 0; i < objectsToFileSave.size(); i++)
		{
			if (objectsToFileSave[i].x == position.x && objectsToFileSave[i].y == position.y)
			{
				objectsToFileSave.erase(objectsToFileSave.begin() + i);
			}
		}
	}
}

Sprite* map_level::getTileAt(int idx)
{
	if (idx >= 0 && idx < tileSpritesVector.size())
	{
		return &tileSpritesVector[idx];
	}
	return nullptr;
}

accessories* map_level::getObjectAt(int idx)
{
	if (idx >= 0 && idx < objectsVector.size())
	{
		return objectsVector[idx].get();
	}
	return nullptr;
}

enemies* map_level::getEnemieAt(int idx)
{
	if (idx >= 0 && idx < enemiesVector.size())
	{
		return enemiesVector[idx].get();
	}
	return nullptr;
}

int map_level::getTileType(Vector2f pos)
{
	if (pos.x > 0)
	{
		pos.x /= TILESIZE;
	}

	if (pos.y > 0)
	{
		pos.y /= TILESIZE;
	}

	return tilesTypeMap[pos.x][pos.y];
}

const mapFileHeader map_level::getMapHeader()
{
	return mapHeader;
}

void map_level::setObject(Sprite sprite, int type)
{
	if (type >= TexNames::TILESTOTHEEND)
	{
		Vector2i inArrayPosition((int)sprite.getPosition().x / TILESIZE, (int)sprite.getPosition().y / TILESIZE);

		tilesTypeMap[inArrayPosition.x][inArrayPosition.y] = type;

		//sprite.setPosition(inArrayPosition.x, inArrayPosition.y);
		tileSpritesVector.push_back(sprite);
	}
	else if(type > TexNames::EMPTY)
	{
		objectsToFileSave.push_back(filemapObject(type, sprite.getPosition().x, sprite.getPosition().y));
		addObjectToVector(sprite, type);
	}
	else if (type == TexNames::EMPTY)
	{
		deleteObject(sprite.getPosition());
	}
}

unique_ptr<accessories> map_level::objectFactory(int type)
{
	unique_ptr<accessories> temp = nullptr;
	switch (TexNames(type))
	{
	case CHICKEN:
		temp = make_unique<accessories>(chicken());
		break;
	case BUSH:
		temp = make_unique<accessories>(bush());
		break;
	case CAMPFIRE:
		temp = make_unique<accessories>(campFire());
		break;
	case CHEST:
		temp = make_unique<accessories>(chest());
		break;
	case COIN:
		temp = make_unique<accessories>(coin());
		break;
	case BOX:
		temp = make_unique<accessories>(box());
		break;
	case LADDER:
		temp = make_unique<accessories>(ladder());
		break;
	case NEXTPLATE:
		temp = make_unique<accessories>(plate());
		break;
	}
	return temp;
}

void map_level::addObjectToVector(Sprite sprite, int type)
{
	unique_ptr<accessories> temp = objectFactory(type);

	if (temp == nullptr)
	{
		tileSpritesVector.push_back(sprite);
	}
	else
	{
		temp->setPosition(sprite.getPosition());
		
		enemies *isEnemie = dynamic_cast<enemies *>(temp.get());
		if (isEnemie)
		{
			unique_ptr <enemies> enemie = make_unique<enemies>(*isEnemie);
			enemiesVector.push_back(std::move(enemie));
		}
		else
		{
			objectsVector.push_back(std::move(temp));
		}
	}
}

string map_level::makeFileName()
{
	string fName = mapName;
	//fName.erase(std::remove_if(fName.begin(), fName.end(), ' '), fName.end());
	fName.erase(std::remove(fName.begin(), fName.end(), ' '), fName.end());
	std::transform(fName.begin(), fName.end(), fName.begin(), ::tolower);

	return fName;
}

void map_level::resizeMap(Vector2i WidthHeightNewSize)
{
	bool toResize = false;
	if (WidthHeightNewSize.x >= mapHeader.mapWidthAsTiles)
	{
		mapHeader.mapWidthAsTiles = WidthHeightNewSize.x + 1;
		toResize = true;
	}
	else if (WidthHeightNewSize.y >= mapHeader.mapHeightAsTiles)
	{
		mapHeader.mapHeightAsTiles = WidthHeightNewSize.y + 1;
		toResize = true;
	}

	if (toResize)
	{
		buildMapBody();
	}
}

void map_level::loadTextures()
{
	const int tilesAmount = numOfTileTypes;
	textures.resize(TexNames::TILESTOTHEEND + tilesAmount);

	textures[(int)TexNames::BACKGROUND].loadFromFile("data/Graphics/ingameBackground.png");
	//textures[TexNames::MENUBACKGROUND].loadFromFile("data/Graphics/ingameMenuBackground.png");
	textures[TexNames::EMPTY].loadFromFile("data/Graphics/Others/empty.png");
	
	textures[TexNames::CHICKEN].loadFromFile("data/Graphics/Others/Enemies/angryChickenTileset.png", IntRect(0, 0, 64, 46));
	textures[TexNames::FLOWER].loadFromFile("data/Graphics/Others/Enemies/flower.png");
	textures[TexNames::TURTLE].loadFromFile("data/Graphics/Others/Enemies/turtle.png");

	textures[TexNames::BUSH].loadFromFile("data/Graphics/Others/Objects/bush.png");
	textures[TexNames::CAMPFIRE].loadFromFile("data/Graphics/Others/Objects/CampFire.png", IntRect(0, 0, 64, 64));
	textures[TexNames::CHEST].loadFromFile("data/Graphics/Others/Objects/chests.png", IntRect(0, 0, 32, 32));
	textures[TexNames::COIN].loadFromFile("data/Graphics/Others/Objects/coin.png");
	textures[TexNames::BOX].loadFromFile("data/Graphics/Others/Objects/box.png");
	textures[TexNames::LADDER].loadFromFile("data/Graphics/Others/Objects/ladder.png");

	textures[TexNames::MUSHROOM_1].loadFromFile("data/Graphics/Others/Objects/mushroom1.png");
	textures[TexNames::MUSHROOM_2].loadFromFile("data/Graphics/Others/Objects/mushroom2.png");
	textures[TexNames::NEXTPLATE].loadFromFile("data/Graphics/Others/Objects/plate1.png");
	textures[TexNames::ENDPLATE].loadFromFile("data/Graphics/Others/Objects/plate2.png");
	
	for (int i = 0; i < tilesAmount; i++)
	{
		textures[TexNames::TILESTOTHEEND + i].loadFromFile("data/Graphics/Tileset/Tiles/" + std::to_string(i+1) + ".png");
	}
}

void map_level::synchronizeMap()
{
	mapHeader.mapWidthAsTiles = tilesTypeMap.size();
	mapHeader.mapHeightAsTiles = tilesTypeMap[0].size();
	mapHeader.numOfObjets = objectsToFileSave.size();
	mapHeader.numOfTitleChars = mapName.size();

	if (objectsToFileSave.size() > 2)
	{
		sortObjects(0, objectsToFileSave.size() - 1);
	}
}

void map_level::sortObjects(int left, int right)
{
	int i = left;
	int j = right;

	int x = objectsToFileSave[(left + right) / 2].x;

	do
	{
		while (objectsToFileSave[i].x < x)
		{
			i++;
		}
		while (objectsToFileSave[j].x > x)
		{
			j--;
		}
		if (i <= j)
		{
			filemapObject temp = objectsToFileSave[i];
			objectsToFileSave[i] = objectsToFileSave[j];
			objectsToFileSave[j] = temp;

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
