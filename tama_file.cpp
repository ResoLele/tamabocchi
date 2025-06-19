#include <iostream>
#include <filesystem>
#include <fstream>
#include "define.h"

using namespace std;
namespace fs = std::filesystem;

// VARIABLES
fs::path USER_PATH;
fs::path USER_FILE;
string DEFAULT_PATH;

void fileInit(string &selectedPath) {
	USER_PATH = fs::current_path();
	selectedPath = USER_PATH.string();
	
}

// FUNCTIONS
void fileChangeDir(string &selectedPath) {
	do {
		cout << "Please enter the Path" << endl;
		cin >> USER_PATH;
		if (fs::directory_entry{USER_PATH}.exists()) {
			selectedPath = USER_PATH.string();
			cout << "Directory changed" << '\n';
			cout << "Current Directory: " << selectedPath << endl;
		} else {
			cout << "Directory not found!" << endl;
		}
	} while (!fs::directory_entry{USER_PATH}.exists());
}

void fileListDir() {
	for (const auto &dirEntry : fs::directory_iterator(USER_PATH)) {
		if (dirEntry.path().extension() == ".flac") {
			cout << "FOUND FLAC: " << dirEntry.path() << '\n';
		}
	}
	cout << "END OF DIRECTORY" << endl;
}

void fileSelectFile(string &selectedPath, string &selectedSong)
{
	do {
		fileListDir();
		cout << "Please enter the filename" << endl;
		cin >> USER_FILE; 
		selectedSong = USER_PATH.string() + '/' + USER_FILE.string();
		if (fs::directory_entry{selectedSong}.exists()) {
			cout << "File Selected!" << '\n';
			cout << "Current File: " << selectedSong << endl;
		} else {
			cout << "File not found!" << endl;
		}
	} while (!fs::directory_entry{selectedSong}.exists());
}