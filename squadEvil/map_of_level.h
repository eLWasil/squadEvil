#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include "accessories.h"
using namespace std;

class map_of_level
{
public:
	map_of_level();
	map_of_level(string nameOfLevel);
	~map_of_level();

	vector <int > operator[](int row) { return TileMapInNumbers[row]; };


	/* Getters */
	const int getObjectsSpritesCount() { return allAccessoriesSprites.size(); };
	string getNameOfLevel() { return nameOfLevel; };
	string getTypeOfMap() { return typeOfMap; };
		// Areas 
	const int getSizeMapInNumbers() { return TileMapInNumbers.size(); }
	const int getAreaSpritesCount() { return mapAreaSprites.size(); };
	const int getTileType(Vector2f pos);
	int getMapSizePx() { return (TileMapInNumbers[0].size() - 1) * 64; };
	const Sprite getAreaSprite(int k) { return mapAreaSprites[k]; };
		// Objects 
	const int getObjectTypesCount() { return accessoryTypes::COUNT; };
	const int getObjectsCount() { return objectsVector.size(); };
	accessories* getObjectAt(int index) { return objectsVector[index]; };
	void deleteObjectAt(int index);

	/* Editor */
	void setTile(Sprite current, int type);
	void setObject(Sprite current, int type);
	accessories* getNearestAccessory(Vector2f mousePos);
	void setName(string k) { nameOfLevel = k; };
	void saveMap();


	vector <Sprite > allAccessoriesSprites; //only if loaded, for editor Interface, TODO: editor: HUD with tiles
	vector <accessories *> allAccessoriesObjects;
	vector <Texture > tileTextures;

	Sprite backgroundSprite;
	Texture backgroundTex;

private:
	string nameOfLevel, typeOfMap;

	vector <vector <int >> TileMapInNumbers;
	vector <accessories *> objectsVector;
	vector <Sprite > mapAreaSprites;

	void resizeMap(int x, int y);

	enum accessoryTypes {
		COIN, CHEST, CAMPFIRE, BOX, LADDER,
		CHICKEN, WARRIOR,
		BUSH,
		ENDOFLEVEL, COUNT
	} ;

	accessories* CreateObjectByTypeNumber(int accTypeNr);
	void backgroundBuilder();
	void tileTexLoader();
	void areaBuilder();
	void loadObjectsToVector();

	void sortObjects(int left, int right);
	void sortTiles(int left, int right);
};

