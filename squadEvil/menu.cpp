#include "menu.h"
#include <math.h>


menu::menu(RenderWindow &window, options &option) : mainWindow(window), settings(option), currentMenuChoice(1)
{

	screen.setSize(window.getSize().x, window.getSize().y);
	screen.setCenter(window.getSize().x / 2, window.getSize().y / 2);

	backgroundTex.loadFromFile("data/Graphics/interfaceBackground2.png");
	background.setTexture(backgroundTex);
	eBackgroundTex.loadFromFile("data/Graphics/ingameMenuBackground.png");
	eBackground.setTexture(eBackgroundTex);


	mainLoop();
}


menu::~menu()
{
}

void menu::mainLoop()
{
	vector <string> mainMenuOptions;
	mainMenuOptions.push_back("New Game");
	mainMenuOptions.push_back("Editor");
	mainMenuOptions.push_back("Quit");

	while (true)
	{
		int currentMenuChoice = choiceLoop(mainMenuOptions);
		if (currentMenuChoice == 0)
		{
			gameSettings();
		}
		else if (currentMenuChoice == 1)
		{
			editorSettings();
		}
		else if (currentMenuChoice == 2)
		{

		}
		else if (currentMenuChoice == 3)
		{
			mainWindow.close();
			break;
		}
		else if (currentMenuChoice == 4)
		{
			mainWindow.close();
			break;
		}
	}
}

void menu::gameSettings()
{
	Text menuOptions[2];
	vector <string> gameOptions = { "Start demo", "Back" };

	while (true)
	{
		int currentMenuChoice = choiceLoop(gameOptions);
		if (currentMenuChoice == 0)
		{
			game *p = new game(mainWindow, &settings);
			delete p;
		}
		else if (currentMenuChoice == 2)
		{
			break;
		}
	}
}

void menu::editorSettings()
{
	vector <string> menuOptions = { "New map", "Load Map", "Back" };


	editor *editorHandle = nullptr;
	while (true)
	{
		int currentMenuChoice = choiceLoop(menuOptions);
		if (currentMenuChoice == 0)
		{
			editorHandle = new editor(mainWindow);
			editorHandle->mainLoop();

			editorMenu(editorHandle);
		}
		else if (currentMenuChoice == 1)
		{
			string mapName = mapChoice();
			if (mapName.size() > 0)
			{
				editorHandle = new editor(mainWindow, mapName);
				editorHandle->mainLoop();
				editorMenu(editorHandle);
			}
		}
		else if (currentMenuChoice == 2)
		{
			break;
		}
		else if (currentMenuChoice == 3)
		{
			break;
		}
	}
	if (editorHandle != nullptr)
	{
		//delete editorHandle;
	}
}

void menu::editorMenu(editor *map)
{
	mainWindow.draw(eBackground);
	vector <string> editorOptions = { "Resume", "Reset", "Save", "Quit" };

	while (true)
	{
		int choice = choiceLoop(editorOptions, true);
		if (choice == 0)
		{
			map->mainLoop();
		}
		else if (choice == 1)
		{
			delete map;
			map = new editor(mainWindow);
			map->mainLoop();
			break;
		}
		else if (choice == 2)
		{
			string mapName = setMapName(map->getMapName());
			map->saveMap(mapName);
		}
		else if (choice == 3)
		{
			break;
		}
		else
		{
			break;
		}
	}
}

void menu::draw()
{
	mainWindow.draw(background);

	for (int i = 0; i < 4; i++)
	{
		menuOptions[i].setColor(Color(255, 255, 255));
		if (i == currentMenuChoice)
		{
			menuOptions[i].setColor(Color(85, 0, 0));
		}
		mainWindow.draw(menuOptions[i]);
	}
}

int menu::choiceLoop(vector <string> options, bool ingame)
{
	Text *menuOptions = new Text[options.size()];

	for (int i = 0; i < options.size(); i++)
	{
		menuOptions[i].setFont(settings.font_1);
		menuOptions[i].setString(options[i]);
		menuOptions[i].setCharacterSize(60);
		if (!ingame)
		{
			menuOptions[i].setPosition(mainWindow.getSize().x - 550, (mainWindow.getSize().y - 340) + (i * 75));
		}
		else
		{
			float width = menuOptions[i].getGlobalBounds().width;
			menuOptions[i].setPosition((mainWindow.getSize().x / 2) - (width / 2), (mainWindow.getSize().y - 450) + (i * 75));
		}
	}

	int current = 0;
	while (true)
	{
		Event handler;
		if (mainWindow.pollEvent(handler))
		{
			if (handler.type == Event::KeyPressed)
			{
				if (handler.key.code == Keyboard::Escape)
				{
					current = options.size();
					break;
				}
				else if (handler.key.code == Keyboard::Up)
				{
					current -= 1;
					if (current < 0)
					{
						current = options.size() - 1;
					}
				}
				else if (handler.key.code == Keyboard::Down)
				{
					current = (++current) % options.size();
				}
				else if (handler.key.code == Keyboard::Return)
				{
					break;
				}
			}
		}

		mainWindow.setView(screen);

		if (!ingame)
		{
			mainWindow.draw(background);
		}
		else
		{
			mainWindow.draw(eBackground);
		}

		for (int i = 0; i < options.size(); i++)
		{
			menuOptions[i].setColor(Color(255, 255, 255));
			if (i == current)
			{
				menuOptions[i].setColor(Color(85, 0, 0));
			}
			mainWindow.draw(menuOptions[i]);
		}

		mainWindow.display();
	}

	delete[] menuOptions;
	return current;
}

string menu::mapChoice()
{
	vector<string> files = filesManager.list("data/Levels/");
	if (files.size() == 0)
	{
		return "";
	}

	Sprite curtain_1;
	Texture curtain_1Tex;
	curtain_1Tex.loadFromFile("data/Graphics/Menu/curtain.png");
	curtain_1.setPosition(mainWindow.getSize().x - 550, (mainWindow.getSize().y - 310));
	curtain_1.setTexture(curtain_1Tex);

	Sprite curtain_2;
	Texture curtain_2Tex;
	curtain_2Tex.loadFromFile("data/Graphics/Menu/curtain_r.png");
	curtain_2.setPosition(mainWindow.getSize().x - 550, (mainWindow.getSize().y - 310) + 120);
	curtain_2.setTexture(curtain_2Tex);

	Text maps[3];
	int k = 0;
	string mapList[3] = { k - 1 < 0 ? "" : files[k], files[k], k + 1 < files.size() ? files[k + 1] : "" };

	float curtainWidth = curtain_1.getGlobalBounds().width;
	for (int i = 0; i < 3; i++)
	{
		maps[i].setFont(settings.font_1);
		maps[i].setString(mapList[i]);
		maps[i].setCharacterSize(60);
		float width = maps[i].getGlobalBounds().width;
		float left = (curtainWidth - width) / 2;
		maps[i].setPosition(mainWindow.getSize().x - 540 + left, (mainWindow.getSize().y - 320) + (i * 75));
	}
	maps[1].setColor(sf::Color(128, 21, 21));

	while (true)
	{
		Event handler;
		if (mainWindow.pollEvent(handler))
		{
			if (handler.type == Event::KeyPressed)
			{
				if (handler.key.code == Keyboard::Escape)
				{
					return "";
					break;
				}
				else if (handler.key.code == Keyboard::Down)
				{
					if (k < files.size() - 1)
					{
						k++;
					}
				}
				else if (handler.key.code == Keyboard::Up)
				{
					if (k > 0)
					{
						k--;
					}
				}
				else if (handler.key.code == Keyboard::Return)
				{
					return mapList[1];
				}
				else if (handler.key.code == Keyboard::Delete)
				{
					string name = "data/Levels/" + mapList[1] + ".level";
					remove(name.c_str());
					return mapChoice();
				}
			}
		}


		mainWindow.setView(screen);
		mainWindow.draw(background);

		mapList[0] = (k - 1 < 0 ? "" : files[k - 1]);
		mapList[1] = files[k];
		mapList[2] = (k + 1 < files.size() ? files[k + 1] : "");
		for (int i = 0; i < 3; i++)
		{
			maps[i].setString(mapList[i]);
			float width = maps[i].getGlobalBounds().width;
			float left = (curtainWidth - width) / 2;
			maps[i].setPosition(mainWindow.getSize().x - 540 + left, (mainWindow.getSize().y - 320) + (i * 75));
			mainWindow.draw(maps[i]);
		}
		mainWindow.draw(curtain_1);
		mainWindow.draw(curtain_2);

		mainWindow.display();
	}
	return "";
}



string menu::setMapName(string prevName)
{
	String texts[5] = { "Resume", "Reset", "Save as:", prevName, "Quit" };
	Text menuOptions[5];
	int menuOptionsSize = 4;

	for (int i = 0; i < 5; i++)
	{
		menuOptions[i].setFont(settings.font_1);
		menuOptions[i].setString(texts[i]);
		menuOptions[i].setCharacterSize(60);

		float width = menuOptions[i].getGlobalBounds().width;
		menuOptions[i].setPosition((mainWindow.getSize().x / 2) - (width / 2), (mainWindow.getSize().y - 450) + (i * 75));
	}

	string str = prevName;
	while (true)
	{
		Event handler;
		if (mainWindow.pollEvent(handler))
		{
			if (handler.type == Event::TextEntered)
			{
				if (handler.key.code != 13)
				{
					if (handler.text.unicode == '\b')
					{
						str = str.substr(0, str.size() - 1);
					}
					else if (handler.text.unicode < 128)
					{
						str += static_cast<char>(handler.text.unicode);
					}
					menuOptions[3].setString(str);
				}
			}

			if (handler.type == Event::KeyPressed)
			{
				if (handler.key.code == Keyboard::Return)
				{
					string k = menuOptions[3].getString();

					if (k.size() > 0)
					{
						return k;
					}
					else
					{
						return prevName;
					}
				}
				else if (handler.key.code == Keyboard::Escape)
				{
					return prevName;
				}
			}
		}

		mainWindow.setView(screen);
		mainWindow.draw(eBackground);

		menuOptions[3].setColor(Color(85, 0, 0));
		float width = menuOptions[3].getGlobalBounds().width;
		menuOptions[3].setPosition((mainWindow.getSize().x / 2) - (width / 2), (mainWindow.getSize().y - 450) + (3 * 75));

		for (int i = 0; i < 5; i++)
		{
			mainWindow.draw(menuOptions[i]);
		}

		mainWindow.display();
	}
}

float shiftOther(float time)
{
	return -(pow(time, 2) / 1500);
}
