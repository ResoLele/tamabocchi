#ifndef TAMA_USER_H
#define TAMA_USER_H

#include <iostream>

#include "tama_file.h"
#include "tama_tag.h"

using namespace std;

const FilePath DEFAULT_PATH = fs::current_path() / "music";

class User {
	private:
	FilePath _path;
	vector<Song> _songs;

	public:
	User();
	
	FilePath currentPath();
	void listDirectory();
	
	void scanDirectory();
	void changeDirectory(const FilePath);
	
	Song& song(const int);
};

#endif // TAMA_USER_H