#pragma once
#include "map_of_level.h"
#include "Mage.h"
#include "options.h"
#include "FileMenager.h"
#include "physics.h"
using namespace sf;

class game
{
public:
	game(RenderWindow &window, options *setting);
	~game();

	map_of_level *level;
	void mainLoop();
	options &settings;
	string currentMapName;
private:
	RenderWindow &window;
	View screen;
	FileMenager f_manager;
	physics Physics;

	int SCRN_WIDTH,
		SCRN_HEIGHT;
	
	void draw();
	void camera();
	void camera2();
	Vector2f cameraTarget;

	player *p_1;

	// Timers
	Clock cameraTmer;
	Clock skillsTimer;
	
	void endOfLevel();

	vector <skills *> skillsArray;
};

