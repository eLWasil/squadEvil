#pragma once
#include "map_level.h"
#include "Mage.h"
#include "options.h"
#include "FileMenager.h"
using namespace sf;

class game
{
public:
	game(RenderWindow &window, options *setting);
	~game();

	map_level *level;
	void mainLoop();
	options &settings;
	string currentMapName;
private:
	RenderWindow &window;
	View screen;
	FileMenager f_manager;

	int SCRN_WIDTH,
		SCRN_HEIGHT;
	
	void draw();
	void camera();
	player *p_1;

	// Timers
	Clock cameraTmer;
	
	void endOfLevel();

	vector <skills *> skillsArray;
};

