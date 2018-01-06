#pragma once
#include "map_of_level.h"
#include "player.h"
#include <fstream>
using namespace sf;

class editor
{
public:
	editor(RenderWindow &window);
	editor(RenderWindow &window, string mapName);
	~editor();

	map_of_level level;
	void save(string k = "");

	string getMapName();

	void mainLoop();
private:
	int SCRN_WIDTH,
		SCRN_HEIGHT;

	int currentTileType;

	RenderWindow &edWindow;
	View screen;

	Sprite *workingSprite;
	vector <Sprite > interfaceSprites;
	Texture accessoryTex[14];

	//Sprite interfaceTiles[34];
	//Sprite allSprites[34];
	int mouseDelta;

	//Sprite accessoriesTiles[10];
	//Sprite accessoriesTilesInterface[10];
	float widthInterfaceTile, dist;
	RectangleShape interfaceShapes[20];
	int interfaceMode;
	Texture coinsModeTex[4];

	void loadTex();
	void draw();
	void drawInterface();

};

