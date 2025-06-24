#ifndef TAMA_USER_H
#define TAMA_USER_H

#include <iostream>

#include "tama_file.h"
#include "tama_tag.h"

using namespace std;

const file_path DEFAULT_PATH = fs::current_path() / "music";

class user
{
	private:
	file_path _path;
	vector<song> _songs;

	public:
	user();
	
	file_path currentPath();
	void listDirectory();
	
	void scanDirectory();
	void changeDirectory(const file_path);
	
	song& selectSong(const int);
};

#endif // TAMA_USER_H