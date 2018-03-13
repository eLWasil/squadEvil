#pragma once
#include <string>
#include <vector>
#include "dirent.h"

using namespace std;

class FileMenager
{
public:
	FileMenager();
	~FileMenager();

	vector <string> list(const char *dirname);
	string nextFile(const char *dirname, string c_name);
	string getCurrentPath();
};

