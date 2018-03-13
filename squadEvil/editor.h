#pragma once
#include "map_level.h"
#include "player.h"
#include <fstream>
using namespace sf;

class editor
{
public:
	editor(RenderWindow &window, string filename = "");
	~editor();

	void saveMap(string title = "");
	void mainLoop();

	string getMapName();
private:
	int SCRN_WIDTH,
		SCRN_HEIGHT;

	RenderWindow &edWindow;
	View screen;
	map_level level;

	Sprite workingSprite, background;
	vector <Sprite > interfaceSprites;
	void loadInterace();
	int mouseDelta;

	int interfaceMode;
	Texture coinsModeTex[4];

	void draw();
	void drawTiles();
	void drawObjects();
	void drawEnemies();
	void drawInterface();

};

