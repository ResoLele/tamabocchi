#ifndef TAMA_USER_H
#define TAMA_USER_H

#include <iostream>
#include <unordered_map>

#include "tama_file.h"
#include "tama_tag.h"

using HashMapDir = std::unordered_map<FilePath, std::unique_ptr<File>>;

const size_t STRING_NOT_FOUND = std::string::npos; 

const FilePath DEFAULT_PATH = std::filesystem::current_path() / "music";

class User { 
	private:
	FilePath _path;
	HashMapDir _files;
	std::unordered_map<std::string, std::vector<Song*>> _albumMap;

	public:
	User(const FilePath);

	FilePath currentPath();

	template <typename T>
	std::unique_ptr<T> createEntry(const std::filesystem::directory_entry entry);
	
	template <typename T>
	void listDirectory();

	void scanDirectory();
	void changeDirectory(const FilePath);

	void searchHashMap(const std::string);

	void listAlbumMap();
};

#endif // TAMA_USER_H