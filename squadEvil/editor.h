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

	int currentTileType;

	RenderWindow &edWindow;
	View screen;

	Sprite workingSprite;
	void setWorkingSprite(int newDelta);

	vector <Sprite > interfaceSprites;
	void loadInterace();
	int mouseDelta;

	float widthInterfaceTile, dist;
	RectangleShape interfaceShapes[20];
	int interfaceMode;
	Texture coinsModeTex[4];

	void draw();
	void drawInterface();

};

