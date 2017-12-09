#include "options.h"



options::options() : fullscreen(true)
{
	//tu czytaj z pliku
	ccapacity = Vector2i(1360, 768);
	settings.antialiasingLevel = 8;

	font_1.loadFromFile("data/Fonts/Jaapokki-Regular.otf");
}


options::~options()
{
}

VideoMode options::returnVMode()
{
	return VideoMode(ccapacity.x, ccapacity.y, 32);
}