#pragma once
#include "skillsLoader.h"

class skillsBook
{
public:
	skillsBook();
	~skillsBook();

	skills& operator[](int number);
};

