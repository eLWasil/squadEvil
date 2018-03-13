#include <time.h>
#include "menu.h"
#include "options.h"
#include "map_level.h"

int main()
{
	srand((unsigned)time(NULL));

	options defaultO;
	RenderWindow window;

	bool fullscreen = false;
	if (fullscreen)
	{
		window.create(defaultO.returnVMode(), "SquadEvvil", Style::Default | Style::Fullscreen, defaultO.settings);
	}
	else
	{
		window.create(defaultO.returnVMode(), "SquadEvvil", Style::Default, defaultO.settings);
	}
	window.setFramerateLimit(60);

	menu sayHello(window, defaultO);
	

}
