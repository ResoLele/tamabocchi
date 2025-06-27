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
using FileEntry     = std::filesystem::directory_entry;

class File {
	protected:
	FileEntry _entry;
	
	private:
	// FileName      _name;
	// FilePath      _path;
	// FileExtension _extension;

	public:
	File(const FileEntry&);
	virtual ~File() = default;

	// void setName(const FileName);
	// void setPath(const FilePath);
	// void setExtension(const FileExtension);
	
	void write(const std::vector<std::byte>);
	void save();

	FileName filename() const;
	FilePath path() const;
	FileExtension extension() const;
};

#endif // TAMA_FILE_H