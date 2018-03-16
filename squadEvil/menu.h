#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "editor.h";
#include "options.h"
#include "FileMenager.h"
#include "game.h"
using namespace sf;

class menu
{
public:
	menu(RenderWindow &, options &);
	~menu();

	void mainLoop();
	RenderWindow &mainWindow;
private:
	Texture backgroundTex;
	Sprite background;
	options &settings;
	View screen;
	Texture eBackgroundTex;
	Sprite eBackground;


	Text menuOptions[4];
	int currentMenuChoice;

	void draw();
	int choiceLoop(vector <string>, bool inGameMenu = false);

	/* SECTIONS */
	void gameSettings(); //Open from main menu
	void editorSettings(); //Open from main menu
	void editorMenu(editor *); //Open from editor

	string mapChoice();
	string setMapName(string);

	FileMenager filesManager;
};

