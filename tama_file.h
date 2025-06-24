#ifndef TAMA_FILE_H
#define TAMA_FILE_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

using file_path = fs::path;
using file_name = string;
using file_ext = string;

class file
{
	private:
	file_name _name; 
	file_path _path;
	file_ext _extension;

	public:
	void setName(const file_name);
	void setPath(const file_path);
	void setExtension(const file_ext);

	void write(const vector<byte>);
	void save();

	file_name name() const;
	file_path path() const;
	file_ext extension() const;
};

// void changeDirectory(file_path&, const file_path);
void scanDirectory(vector<file>&, const file_path);
void listFiles(const vector<file>&);

#endif // TAMA_FILE_H