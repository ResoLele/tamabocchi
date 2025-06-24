#ifndef TAMA_FILE_H
#define TAMA_FILE_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

using FilePath      = fs::path;
using FileName      = string;
using FileExtension	= string;

class File {
	private:
	FileName      _name; 
	FilePath      _path;
	FileExtension _extension;

	public:
	void setName(const FileName);
	void setPath(const FilePath);
	void setExtension(const FileExtension);

	void write(const vector<byte>);
	void save();

	FileName name() const;
	FilePath path() const;
	FileExtension extension() const;
};

// void changeDirectory(file_path&, const file_path);
void scanDirectory(vector<File>&, const FilePath);
void listFiles(const vector<File>&);

#endif // TAMA_FILE_H