#include "define.h"

using namespace std;
namespace fs = std::filesystem;

// Class file
void file::setFilename(const string filename) {_filename = filename;}
void file::setPath(const fs::path path) {_path = path;}
string file::filename() {return _filename;}
fs::path file::path() {return _path;}

void music::setTitle(const string str) {_title = str;}
void music::setAlbum(const string str) {_album = str;}
void music::setDate(const string str) {_date = str;}
string music::title() {return _title;}
string music::album() {return _album;}
string music::date() {return _date;}

// Change Directory
fs::path dirChange() {
	string userInput;

	while(true) {
		cout << "Enter Path: (Leave empty using current path)" << endl;
		getline(cin, userInput);
		if (!userInput.length()) {
			cout << "Using current path" << endl;
			return (fs::current_path() / "music");
		}
		else if (fs::exists(fs::path(userInput))) {
			return fs::path(userInput);
		}
	}
}

bool dirScan(fs::path path) {

	int count;

	for (const fs::directory_entry dirEntry : fs::directory_iterator(path)) {
		if (dirEntry.path().extension() == ".flac") {
			count++;
			music music;
			music.setFilename(dirEntry.path().filename());
			music.setPath(dirEntry.path());
			files.push_back(music);
		}
	}
	return (count > 0);
}

void dirPrint(fs::path path) {
	cout << "File count: "<< files.size() << endl;
	for (music i : files) {
		cout << i.filename() << endl;
	}
	cout << endl;
}