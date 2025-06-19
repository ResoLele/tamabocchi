#include <iostream>
#include <filesystem>
#include <fstream>
#include "define.h"

using namespace std;
namespace fs = std::filesystem;

// Class file
void file::setFilename(const string str) {_filename = str;}
void file::setTitle(const string str) {_title = str;}
void file::setAlbum(const string str) {_album = str;}
void file::setDate(const string str) {_date = str;}
void file::setPath(const fs::path str) {_path = str;}

string file::filename() {return _filename;}
string file::title() {return _title;}
string file::album() {return _album;}
string file::date() {return _date;}
string file::path() {return _path;}

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
			file music;
			music.setFilename(dirEntry.path().filename());
			music.setPath(dirEntry.path());
			files.push_back(music);
		}
	}
	return (count > 0);
}

void dirPrint(fs::path path) {
	cout << "File count: "<< files.size() << endl;
	for (file i : files) {
		cout << i.filename() << endl;
	}
	cout << endl;
}