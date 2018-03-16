#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include "accessories.h"
#include "FileMenager.h"
#include "enemies.h"
using namespace std;

struct filemapObject
{
	int type;
	float x, y;

	filemapObject(int t, float px, float py) : type(t), x(px), y(py) {};
};

struct mapFileHeader
{
	int numOfTitleChars;
	int mapWidth;
	int mapHeight;
	int numOfObjets;
};

enum TexNames
{
	BACKGROUND,
	EMPTY,
	CHICKEN,
	FLOWER, TURTLE,
	BUSH, CAMPFIRE, CHEST,
	COIN, BOX, LADDER,
	MUSHROOM_1, MUSHROOM_2,
	NEXTPLATE, ENDPLATE,
	TILESTOTHEEND
};

class map_level
{
public:
	map_level();
	map_level(string fileMapName);
	~map_level();
	
	Texture & getTexture(int idx);
	string getMapName() { return mapName; }
	
	void buildMapBody();
	void readMap(string fileName);
	void saveMap(string mapName);

	void setObject(Sprite, int typeIdx);
	const int numOfTileTypes;

	Vector2f getNearestObjectPosition(Vector2f mousePos);
	void deleteObject(Vector2f mousePos);

	Sprite* getTileAt(int idx);
	accessories* getObjectAt(int idx);
	enemies* getEnemieAt(int idx);

	int getTileType(Vector2f pos);
	const mapFileHeader getMapHeader();

	const int TILESIZE;
private:
	string mapName;
	mapFileHeader mapHeader;

	Sprite backgroundSprite;
	vector <Texture > textures;

	vector <Sprite> tileSpritesVector;
	vector <accessories *> objectsVector;
	vector <enemies *> enemiesVector;

	vector <vector <int>> tilesTypeMap;
	vector <filemapObject> objectsToFileSave;
	bool isObjectCorrect(filemapObject *);

	accessories* objectFactory(int type);
	string makeFileName();

	void setObject(filemapObject *);
	void addObjectToVector(Sprite sprite, int type);
	void resizeMap(Vector2i WidthHeightNewSize);
	void loadTextures();
	void synchronizeMap();

	void sortObjects(int, int);

	Vector2i itemToDeleteXVecYIdx;

	FileMenager filemenager;
};
