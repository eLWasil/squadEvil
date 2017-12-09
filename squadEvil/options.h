#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;


class options
{
public:
	options();
	~options();

	bool fullscreen;
	Vector2i capacityList[3];
	Vector2i ccapacity;
	ContextSettings settings;

	Font font_1;

	VideoMode returnVMode();
};

