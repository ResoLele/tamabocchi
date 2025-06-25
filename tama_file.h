#ifndef TAMA_FILE_H
#define TAMA_FILE_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>

using FilePath      = std::filesystem::path;
using FileName      = std::string;
using FileExtension	= std::string;

class File {
	private:
	FileName      _name; 
	FilePath      _path;
	FileExtension _extension;

	public:
	virtual ~File() = default;

	void setName(const FileName);
	void setPath(const FilePath);
	void setExtension(const FileExtension);
	
	void write(const std::vector<std::byte>);
	void save();

	FileName name() const;
	FilePath path() const;
	FileExtension extension() const;
};

#endif // TAMA_FILE_H