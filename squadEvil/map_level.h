#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include "accessories.h"
using namespace std;

class map_level
{
public:
	map_level();
	map_level(string fileName);
	~map_level();

	vector <int > operator[](int k) { return tileMap[k]; };

	enum tileTypes {
		NOTHING,
		LONG_LEFT_CORNER, GROUND, LONG_RIGHT_CORNER, 
		SHORT_RIGHT_CORNER, SHORT_LEFT_CORNER, UNDERGROUND,
		FLYING_ISLAND_LEFT, FLYING_ISLAND_CENTER, FLYING_ISLAND_RIGHT,
		COUNT
	};
	enum accessoryTiles {
		EMPTY,
		COIN, CHEST, CAMPFIRE, BOX, LADDER, 
		CHICKEN, WARRIOR, FLOWER, TURTLE,
		BUSH_1, BUSH_2, BUSH_3, BUSH_4
	};

	vector <vector <int>> tileMap, tileMapHeaven, *whichMap; //should be private, will be fixed

	float getMapSizeX() { return tileMap.size() * 64; };
	vector <accessories*> others;
	vector <sf::Sprite> tileSprites;
	sf::Texture TilesTex[19];
	sf::Texture accessoryTex[14];
	sf::Sprite background;

	/* FOR EDITOR */
	void setTile(sf::Sprite, int type);
	void setAccessories(sf::Sprite, int);
	int getTileType(sf::Vector2f);
	void setHeight(int height);
	void updateMap();
	accessories* getNearestAccessory(Vector2f mousePos);

	string name, type;
	sf::Vector2i mapSize();
private:
	int height,
		width;

	sf::Texture backgroundTex;

	void resize(int x, int y);
	void loadTex();
};

