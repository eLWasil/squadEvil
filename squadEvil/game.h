#pragma once
#include "map_level.h"
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

	map_level *level;
	void mainLoop();
	options &settings;
	string currentMapName;

private:
	RenderWindow &window;
	View screen;
	FileMenager f_manager;
	physics Physics;

	player *p_1;

	vector <skills *> skillsArray;

	
	bool isVisible(Sprite *);
	void draw();
	void drawTiles();
	void drawObjects();
	void drawEnemies();
	void drawSkills();

	void skillIntersectEnemies(skills *skill);

	void camera();
	void camera2();
	Vector2f cameraTarget;


	// Timers
	Clock cameraTmer;
	Clock skillsTimer;
	
	void endOfLevel();


	int SCRN_WIDTH,
		SCRN_HEIGHT;

	Sprite background;
};

