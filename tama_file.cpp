#include "tama_file.h"

void file::setName(const file_name name) {
	_name = name;
}

void file::setPath(const file_path path) {
	_path = path;
}

void file::setExtension(const file_ext extension) {
	_extension = extension;
}

file_name file::name() const {
	return _name;
}

file_path file::path() const {
	return _path;
}

file_ext file::extension() const {
	return _extension;
}

void changeDirectory(file_path &oldPath, const file_path newPath) {

	if (fs::exists(newPath)) {
		oldPath = newPath;
		return;
	}
	cerr << "Path does not exist!" << endl;
}

void scanDirectory(vector<file> &files, const file_path path) {
	files.clear();
	for (const fs::directory_entry &entry : fs::directory_iterator(path)) {
		file f;
		f.setName(entry.path().filename());
		f.setPath(entry.path());
		f.setExtension(entry.path().extension());
		files.push_back(f);
	}
}

void listFiles(const vector<file> &files) {
	cout << "Total File(s): " << files.size() << '\n';
	for (const file &i : files) {
		cout << i.name() << '\n';
	}
	cout << endl;
}