#include "tama_file.h"

File::File(const FileEntry& entry) {
	_entry = entry;
}

// void File::setName(const FileName name) {
// 	_name = name;
// }

// void File::setPath(const FilePath path) {
// 	_path = path;
// }

// void File::setExtension(const FileExtension extension) {
// 	_extension = extension;
// }

FileName File::filename() const {
	return _entry.path().filename();
}

FilePath File::path() const {
	return _entry.path();
}

FileExtension File::extension() const {
	return _entry.path().extension();
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