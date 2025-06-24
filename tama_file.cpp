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

void File::write(const vector<byte> bytes) {
	fstream writeFile(path(), ios::out);
	writeFile.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
	writeFile.close();
}

void File::save() {
	fstream newFile(path(), ios::out);
	
	newFile.close();
}

void scanDirectory(vector<File> &files, const FilePath path) {
	files.clear();
	for (const fs::directory_entry &entry : fs::directory_iterator(path)) {
		File f;
		f.setName(entry.path().filename());
		f.setPath(entry.path());
		f.setExtension(entry.path().extension());
		files.push_back(f);
	}
}

void listFiles(const vector<File> &files) {
	cout << "Total File(s): " << files.size() << '\n';
	for (const File &i : files) {
		cout << i.name() << '\n';
	}
	cout << endl;
}