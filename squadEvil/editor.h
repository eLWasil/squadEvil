#pragma once
#include "map_level.h"
#include "player.h"
#include <fstream>
using namespace sf;

class editor
{
public:
	editor(RenderWindow &window);
	editor(RenderWindow &window, string mapName);
	~editor();

	map_level level;
	void save(string k = "");

	string getMapName();

	void mainLoop();
private:
	int SCRN_WIDTH,
		SCRN_HEIGHT;

	const int countSprite;
	int currentTileType;

	RenderWindow &edWindow;
	View screen;

	Sprite *currentAccessory;
	Sprite interfaceTiles[33];
	Sprite allSprites[33];
	int mouseDelta;

	Sprite accessoriesTiles[10];
	Sprite accessoriesTilesInterface[10];
	float widthInterfaceTile, dist;
	RectangleShape interfaceShapes[20];
	int interfaceMode;
	Texture coinsModeTex[4];

	void loadTex();
	void draw();
	void drawInterface();

	void quicksort(vector <accessories*> &arr, int left, int right);
};

