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

#define DIR "/data/Levels/"
#define TILESIZE 64

map_level::map_level()
{
	mapName = "MyMap";

	mapHeader = { mapName.length(), 40, 20, 0 };

	backgroundSprite.setPosition(0, 0);
	backgroundTex.loadFromFile("data/Graphics/Tileset/Normal/BG.png");
	backgroundSprite.setTexture(backgroundTex);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 200));
}

map_level::map_level(string fileName)
{
	
}


map_level::~map_level()
{
	
}

void map_level::buildMap()
{
	tilesTypeMap.resize(mapHeader.mWidth);
	for (int row = 0; row < mapHeader.mWidth; row++)
	{
		tilesTypeMap[row].resize(mapHeader.mHeight);
	}
}

void map_level::readMap(string fileName)
{
	string path = DIR + fileName;

	ifstream file(fileName, ios::binary);

	char* sizeOfTitle = new char[sizeof(int)];
	file.read(sizeOfTitle, sizeof(int));
	int* size = (int *)sizeOfTitle;

	cout << path << " " << size << endl;

}

void map_level::saveMap(string mapName)
{
	string fName = makeFileName(mapName);
	string fPath = DIR + fName;


	ofstream file(fPath, ios::binary);

}

void map_level::setTile(Sprite sprite, int type)
{
	Vector2i inArrayPosition(sprite.getPosition().x / TILESIZE, sprite.getPosition().y / TILESIZE);
	resizeMap(inArrayPosition);

	tilesTypeMap[inArrayPosition.x][inArrayPosition.y] = type;
	tileSpritesVector.push_back(sprite);
}

string map_level::makeFileName(string mapName)
{
	string fName = mapName;
	fName.erase(std::remove_if(fName.begin(), fName.end(), [](char c) { return isspace(c) || isalpha(c); }), fName.end());
	
	return fName;
}

void map_level::resizeMap(Vector2i WidthHeightNewSize)
{
	bool toResize = false;
	if (WidthHeightNewSize.x >= mapHeader.mWidth)
	{
		mapHeader.mWidth = WidthHeightNewSize.x + 1;
		toResize = true;
	}
	else if (WidthHeightNewSize.y >= mapHeader.mHeight)
	{
		mapHeader.mHeight = WidthHeightNewSize.y + 1;
		toResize = true;
	}

	if (toResize)
	{
		buildMap();
	}
}
