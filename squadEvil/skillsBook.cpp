#include "skillsBook.h"


skillsBook::skillsBook()
{
}


skillsBook::~skillsBook()
{
}

skills& skillsBook::operator[](int n)
{
	skills *temp;
	switch (n)
	{
	case 1:
		temp = new fireball(0, 0);
		break;
	default:
		temp = new fireball(0, 0); // Do zmiany
		break;
	}
	return *temp;
}