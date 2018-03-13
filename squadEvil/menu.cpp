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


	String texts[4] = { "New Game", "Editor", "Options", "Quit" };

	for (int i = 0; i < 4; i++)
	{
		menuOptions[i].setFont(settings.font_1);
		menuOptions[i].setString(texts[i]);
		menuOptions[i].setCharacterSize(60);
		menuOptions[i].setPosition(mainWindow.getSize().x - 550, (mainWindow.getSize().y - 340) + (i * 75));
	}
	
	mainLoop();
}


menu::~menu()
{
}

void menu::mainLoop()
{
	while (true)
	{
		int currentMenuChoice = choiceLoop(menuOptions, 4, background);
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

int menu::choiceLoop(Text menuOptions[], int size, Sprite bg)
{
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
					return size;
				}
				else if (handler.key.code == Keyboard::Up)
				{
					current -= 1;
					if (current < 0)
					{
						current = size - 1;
					}
				}
				else if (handler.key.code == Keyboard::Down)
				{
					current = (++current) % size;
				}
				else if (handler.key.code == Keyboard::Return)
				{
					return current;
				}
			}
		}

		mainWindow.setView(screen);

		mainWindow.draw(bg);

		for (int i = 0; i < size; i++)
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
}

void menu::editorSettings()
{
	Text menuOptions[4];
	int menuOptionsSize = 4;
	String texts[4] = { "New Map", "Load Map", "Back" };

	for (int i = 0; i < 3; i++)
	{
		menuOptions[i].setFont(settings.font_1);
		menuOptions[i].setString(texts[i]);
		menuOptions[i].setCharacterSize(60);
		menuOptions[i].setPosition(mainWindow.getSize().x - 550, (mainWindow.getSize().y - 340) + (i * 75));
	}
	
	editor *editorHandle = nullptr;
	while (true)
	{
		int currentMenuChoice = choiceLoop(menuOptions, 3, background);
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

void menu::editorMenu(editor *map)
{
	mainWindow.draw(eBackground);
	String texts[4] = { "Resume", "Reset", "Save", "Quit" };
	Text menuOptions[4];
	int menuOptionsSize = 4;

	for (int i = 0; i < 4; i++)
	{
		menuOptions[i].setFont(settings.font_1);
		menuOptions[i].setString(texts[i]);
		menuOptions[i].setCharacterSize(60);
		
		float width = menuOptions[i].getGlobalBounds().width;
		menuOptions[i].setPosition((mainWindow.getSize().x / 2) - (width / 2), (mainWindow.getSize().y - 450) + (i * 75));
	}

	while (true)
	{
		int choice = choiceLoop(menuOptions, menuOptionsSize, eBackground);
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
		else if (choice == menuOptionsSize)
		{
			break;
		}
	}
	
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

void menu::gameSettings()
{
	Text menuOptions[3];
	int menuOptionsSize = 3;
	String texts[3] = { "Choose Avatar", "Create new Avatar!", "Back" };

	for (int i = 0; i < menuOptionsSize; i++)
	{
		menuOptions[i].setFont(settings.font_1);
		menuOptions[i].setString(texts[i]);
		menuOptions[i].setCharacterSize(60);
		menuOptions[i].setPosition(mainWindow.getSize().x - 650, (mainWindow.getSize().y - 300) + (i * 75));
	}

	while (true)
	{
		int currentMenuChoice = choiceLoop(menuOptions, menuOptionsSize, background);
		if (currentMenuChoice == 0)
		{
			game *p = new game(mainWindow, &settings);
			delete p;
		}
		else if (currentMenuChoice == 1)
		{
			Clock timer;
			while (1)
			{
				RectangleShape curtain;
				curtain.setFillColor(Color(0, 0, 0, timer.getElapsedTime().asMilliseconds() / 8));
				curtain.setPosition(0, 0);
				curtain.setSize(Vector2f(mainWindow.getSize()));
				mainWindow.draw(curtain);
				mainWindow.display();
				if (timer.getElapsedTime().asMilliseconds() >= 2000)
				{
					choiceClass();
					break;
				}
			}
		}
		else if (currentMenuChoice == 2)
		{
			break;
		}
	}
}

float shiftOther(float time)
{
	return -(pow(time, 2) / 1500);
}

void menu::choiceClass()
{
	// £adowanie i na³o¿enie tekstur
	Texture backgroundTex, swordsTex, arcTex, hatTex;
	backgroundTex.loadFromFile("data/Graphics/BG_class.jpg");
	swordsTex.loadFromFile("data/Graphics/Class/Swords.png");
	arcTex.loadFromFile("data/Graphics/Class/Arc.png");
	hatTex.loadFromFile("data/Graphics/Class/Hat.png");
	
	Sprite BG, swords, arc, hat;
	BG.setTexture(backgroundTex);
	swords.setTexture(swordsTex);
	arc.setTexture(arcTex);
	hat.setTexture(hatTex);
	
	// Rozmieszczenie ikon klas oraz t³o
	const float widthClassBox = 1000;
	float side = (mainWindow.getSize().x - widthClassBox) / 2;
	float top = (mainWindow.getSize().y / 2) - 256;
	swords.setPosition(side, top);
	arc.setPosition(side + 256 + 116, top);
	hat.setPosition(side + 512 + 116 + 116, top);
	swords.setColor(Color(255, 255, 255, 200));
	arc.setColor(Color(255, 255, 255, 200));
	hat.setColor(Color(255, 255, 255, 200));
	Sprite *classes[] = { &swords, &arc, &hat };

	RectangleShape curtain;
	curtain.setSize(Vector2f(mainWindow.getSize()));
	curtain.setPosition(0, 0);
	Clock timer;
	BG.setPosition(0, 0);

	// Tekst
	bool choosen = false;
	string name = "";
	Text nameBox;
	nameBox.setFont(settings.font_1);
	Vector2f centerPos(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2);
	nameBox.setString("Wpisz swoje imie");
	nameBox.setCharacterSize(40);
	nameBox.setColor(Color(255, 255, 255, 125));
	bool nameBoxClicked = false;

	enum ClassMarks
	{
		SWORD, ARC, HAT, COUNT
	} current;
	current = ClassMarks::SWORD;

	float currentPosX = classes[current]->getPosition().x;

	while (1)
	{
		mainWindow.draw(BG);
		//curtain.setFillColor(Color(0, 0, 0, 0));

		if (!choosen)
		{
			mainWindow.draw(swords);
			mainWindow.draw(arc);
			mainWindow.draw(hat);
			

			Event handler;
			if (mainWindow.pollEvent(handler))
			{
				if (handler.type == Event::KeyPressed)
				{
					if (handler.key.code == Keyboard::Right)
					{
						int a = current;
						current = (ClassMarks)(++a%ClassMarks::COUNT);
					}
					else if (handler.key.code == Keyboard::Left)
					{
						int a = current;
						current = (ClassMarks)(a == 0 ? ClassMarks::COUNT-1 : --a%ClassMarks::COUNT);
					}
					else if (handler.key.code == Keyboard::Return)
					{
						Clock timer;
						unsigned int time = 1000;
						float move = (centerPos.x - classes[current]->getGlobalBounds().width / 2) - currentPosX;
						float shift = move / time;
						
						float startPosY = classes[current]->getPosition().y;
						while (timer.getElapsedTime().asMilliseconds() < time)
						{
							if (shift != 0)
							{
								classes[current]->setPosition(currentPosX + (shift * timer.getElapsedTime().asMilliseconds()), classes[current]->getPosition().y);
							}
							classes[(current + 1) % 3]->setPosition(classes[(current + 1) % 3]->getPosition().x, startPosY + (shiftOther(timer.getElapsedTime().asMilliseconds())));
							classes[(current + 2) % 3]->setPosition(classes[(current + 2) % 3]->getPosition().x, startPosY + (shiftOther(timer.getElapsedTime().asMilliseconds())));
							//cout << shiftOther(timer.getElapsedTime().asMilliseconds()) << endl;

							mainWindow.draw(BG);
							mainWindow.draw(swords);
							mainWindow.draw(arc);
							mainWindow.draw(hat);
							mainWindow.display();
						}
						choosen = true;
					}
					else if (handler.key.code == Keyboard::Escape)
					{
						return;
					}
				}
			}
			

			for (size_t i = 0; i < ClassMarks::COUNT; i++)
			{
				//if (classes[i]->getGlobalBounds().contains((Vector2f)Mouse::getPosition(mainWindow)))
				//
				//TODO: change by keys
				//
				
				

				if (i == current)
				{
					classes[i]->setColor(Color(255, 255, 255, 255));
					
				}
				else
				{
					classes[i]->setColor(Color(255, 255, 255, 155));
				}
			}

			if (timer.getElapsedTime().asMilliseconds() < 1000)
			{
				curtain.setFillColor(Color(0, 0, 0, (255 - (timer.getElapsedTime().asMilliseconds()) / 4)));
				mainWindow.draw(curtain);
			}
		}
		else
		{
			mainWindow.draw(*classes[current]);
			nameBox.setPosition(centerPos.x - nameBox.getGlobalBounds().width / 2, centerPos.y - nameBox.getGlobalBounds().height / 2 + 200);
			mainWindow.draw(nameBox);

			if (true)
			{
				Clock timer;
				float dist = centerPos.x - currentPosX;
				unsigned int msec = 1600;
				float step = dist / msec;
				while (timer.getElapsedTime().asMilliseconds() < msec)
				{

				}

				Event handler;
				if (mainWindow.pollEvent(handler))
				{
					

					if (handler.type == Event::KeyPressed)
					{
						if (handler.type == Event::TextEntered)
						{
							if (handler.key.code != 13)
							{
								if (handler.text.unicode == '\b')
								{
									name = name.substr(0, name.size() - 1);
								}
								else if (handler.text.unicode < 128)
								{
									name += static_cast<char>(handler.text.unicode);
								}

								nameBox.setCharacterSize(60);
								nameBox.setColor(Color(255, 255, 255, 255));
								nameBox.setString(name);
							}
						}
					}
				}
			}
			/* else
			{
				Vector2i mousePosition = Mouse::getPosition();
				cout << "Mouse position: " << mousePosition.x << ", " << mousePosition.y << endl;
				cout << "nameBox position: " << nameBox.getPosition().x << " - " << nameBox.getPosition().x + nameBox.getGlobalBounds().width
					<< ", " << nameBox.getPosition().y << " - " << nameBox.getPosition().y + nameBox.getGlobalBounds().height << endl;
				if (mousePosition.y > nameBox.getPosition().y)
				{
					if (mousePosition.y < nameBox.getPosition().y + nameBox.getGlobalBounds().height)
					{
						if (mousePosition.x > nameBox.getPosition().x)
						{
							if (mousePosition.x < nameBox.getPosition().x + nameBox.getGlobalBounds().width)
							{
								nameBox.setColor(Color(255, 255, 255, 255));
								if (Mouse::isButtonPressed(Mouse::Left))
								{
									nameBoxClicked = true;
								}
							}
							else
							{
								nameBox.setColor(Color(255, 255, 255, 125));
								if (Mouse::isButtonPressed(Mouse::Left))
								{
									nameBoxClicked = false;
								}
							}
						}
						else
						{
							nameBox.setColor(Color(255, 255, 255, 125));
							if (Mouse::isButtonPressed(Mouse::Left))
							{
								nameBoxClicked = false;
							}
						}
					}
					else
					{
						nameBox.setColor(Color(255, 255, 255, 125));
						if (Mouse::isButtonPressed(Mouse::Left))
						{
							nameBoxClicked = false;
						}
					}
				}
				else
				{
					nameBox.setColor(Color(255, 255, 255, 125));
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						nameBoxClicked = false;
					}
				}
			} */
			
		}

		

		mainWindow.display();
	}
	
}