#ifndef TAMA_USER_H
#define TAMA_USER_H

#include <iostream>

#include "tama_file.h"
#include "tama_tag.h"

const FilePath DEFAULT_PATH = std::filesystem::current_path() / "music";

class User {
	private:
	FilePath _path;
	std::vector<Song> _songs;

	public:
	User();
	
	FilePath currentPath();
	void listDirectory();
	
	void scanDirectory();
	void changeDirectory(const FilePath);
	
	Song& song(const int);
};

#endif // TAMA_USER_H