#ifndef TAMA_USER_H
#define TAMA_USER_H

#include <iostream>

#include "tama_file.h"
#include "tama_tag.h"

const FilePath DEFAULT_PATH = std::filesystem::current_path() / "music";

class User {
	private:
	FilePath _path;
	std::vector<std::unique_ptr<File>> _files;

	public:
	User();

	FilePath currentPath();

	template <typename T>
	T* getEntry(const int);
	
	template <typename T>
	T* getEntry(const std::string);
	
	template <typename T>
	std::unique_ptr<T> createEntry(const std::filesystem::directory_entry entry);
	
	template <typename T>
	void listDirectory();

	void scanDirectory();
	void changeDirectory(const FilePath);
};

#endif // TAMA_USER_H