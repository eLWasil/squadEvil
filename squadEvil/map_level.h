#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include "accessories.h"
#include "enemies.h"
using namespace std;

struct mapFileHeader
{
	int numOfTitleChars;
	int mWidth;
	int mHeight;
	int numOfObjets;
};


class map_level
{
public:
	map_level();
	map_level(string fileName);
	~map_level();

	void buildMap();
	void readMap(string fileName);
	void saveMap(string mapName);
	Sprite getBackgroundSprite() { return backgroundSprite; }

	void setTile(Sprite, int type);

private:
	mapFileHeader mapHeader;
	vector <vector <int>> tilesTypeMap;
	string mapName;
	Texture backgroundTex;
	Sprite backgroundSprite;

	vector <Sprite> tileSpritesVector;
	vector <accessories *> objectsVector;
	vector <enemies *> enemiesVector;



	string makeFileName(string mapTitle);
	void resizeMap(Vector2i WidthHeightNewSize);

	
};
