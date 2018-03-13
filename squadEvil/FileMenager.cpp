#include "FileMenager.h"
#include <direct.h>
#define GetCurrentDir _getcwd

FileMenager::FileMenager()
{

}


FileMenager::~FileMenager()
{
}

vector<string> FileMenager::list(const char *dirname)

{
	vector <string> files;
	DIR *dir;
	struct dirent *ent;//
	if ((dir = opendir(dirname)) != NULL)
	{
		int k = 0;
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type == DT_REG)
			{
				string name = ent->d_name;
				if (name.size() > 2)
				{
					files.push_back(ent->d_name);
					if (files[files.size() - 1].size() > 6)
					{
						files[files.size() - 1].replace(files[files.size() - 1].size() - 6, files[files.size() - 1].size() - 1, "");
					}
				}
			}
		}
		closedir(dir);
		return files;
	}
	else
	{
		printf("Blad pobierania listy plikow");
	}
}

string FileMenager::nextFile(const char* dirname, string c_name)
{
	printf("Dir: %s\n", dirname);
	vector <string> all = list(dirname);
	if (all.size() > 0)
	{
		for (size_t i = 0; i < all.size(); i++)
		{
			if (all[i] == c_name)
			{
				if (i < all.size() - 1)
				{
					return all[i + 1];
				}
				else
				{
					printf("%s to ostatni plik katalogu %s\n", c_name, dirname);
				}
			}
		}
	}
	else
	{
		printf("Katalog %s nie ma plikow\n", dirname);
	}
}

string FileMenager::getCurrentPath()
{
	char cCurrentPath[FILENAME_MAX];

	GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));

	string path(cCurrentPath);
	return path;
}
