#pragma once
#include <iostream>
#include <string>
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
	void mainLoop();
	int choiceLoop(Text[], int size, Sprite);

	/* SECTIONS */
	void editorSettings(); //Open from main menu
	string mapChoice();
	void editorMenu(editor *); //Open from editor
	string setMapName(string);
	void gameSettings(); //Open from main menu
	void choiceClass();

	FileMenager filesManager;
};

