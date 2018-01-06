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
	const int getAreaVectorCount() { return areaSprites.size(); };
	const Sprite getAreaSprite(int k) { return areaSprites[k]; };
		// Objects 
	const int getObjectTypesCount() { return accessoryTypes::COUNT; };
	const int getObjectsCount() { return objectsVector.size(); };
	accessories* getObjectAt(int index) { return objectsVector[index]; };
	 

	/* Editor */
	void setTile(Sprite current, int type);
	void setObject(Sprite current, int type);
	accessories* getNearestAccessory(Vector2f mousePos);
	void setName(string k) { nameOfLevel = k; };
	void saveMap();


	void loadObjectsSprites();
	vector <Sprite > allAccessoriesSprites; //only if loaded, for editor Interface, TODO: editor: HUD with tiles
	vector <Texture > tileTextures;

	Sprite backgroundSprite;
	Texture backgroundTex;

private:
	string nameOfLevel, typeOfMap;

	vector <vector <int >> TileMapInNumbers;
	vector <accessories *> objectsVector;
	vector <Sprite > areaSprites;

	void resizeMap(int x, int y);

	enum accessoryTypes {
		EMPTY,
		COIN, CHEST, CAMPFIRE, BOX, LADDER,
		CHICKEN, WARRIOR,
		BUSH,
		ENDOFLEVEL, COUNT
	} ;

	accessories* CreateObjectByTypeNumber(int accTypeNr);
	void backgroundBuilder();
	void tileTexLoader();
	void areaBuilder();

	void sortObjects(int left, int right);
	void sortTiles(int left, int right);
};

