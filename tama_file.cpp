#include "define.h"

using namespace std;
namespace fs = std::filesystem;

// Class file
void file::setFilename(const string filename) {_filename = filename;}
void file::setPath(const fs::path path) {_path = path;}
string file::filename() {return _filename;}
fs::path file::path() {return _path;}

// Change Directory
fs::path dirChange() {
	string userInput;

	while(true) {
		cout << "Enter Path: (Leave empty using current path)" << endl;
		getline(cin, userInput);
		if (!userInput.length()) {
			osClear(USER_OS);
			cout << "Using current path" << endl;
			return (fs::current_path() / "music");
		}
		else if (fs::exists(fs::path(userInput))) {
			return fs::path(userInput);
		}
	}
}

bool dirScan(fs::path path) {

	int count = 0;

	for (const fs::directory_entry &dirEntry : fs::directory_iterator(path)) {
		if (dirEntry.path().extension() == ".flac") {
			music flac;
			count++;
			flac.setFilename(dirEntry.path().filename());
			flac.setPath(dirEntry.path());
			files.push_back(flac);
		}
	}
	return (count > 0);
}

void dirPrint(fs::path path) {
	cout << "File count: "<< files.size() << endl;
	for (music &i : files) {
		cout << i.filename() << endl;
	}
	cout << endl;
}