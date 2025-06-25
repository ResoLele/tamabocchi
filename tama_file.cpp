#include "tama_file.h"

void File::setName(const FileName name) {
	_name = name;
}

void File::setPath(const FilePath path) {
	_path = path;
}

void File::setExtension(const FileExtension extension) {
	_extension = extension;
}

FileName File::name() const {
	return _name;
}

FilePath File::path() const {
	return _path;
}

FileExtension File::extension() const {
	return _extension;
}

void File::write(const std::vector<std::byte> bytes) {
	std::fstream file(path(), std::ios::out);
	file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
	file.close();
}

void File::save() {
	std::fstream newFile(path(), std::ios::out);
	
	newFile.close();
}