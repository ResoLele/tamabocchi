#include <iostream>
#include <filesystem>
#include <fstream>
#include "define.h"

using namespace std;
namespace fs = std::filesystem;

// VARIABLES
fs::path USER_PATH;
string DEFAULT_PATH;

void fileInit() {
	USER_PATH = fs::current_path();
}

// FUNCTIONS
void fileChangeDir() {
	do {
		cin >> USER_PATH;
		if (fs::directory_entry{USER_PATH}.exists()) {
			cout << "Directory changed" << '\n';
			cout << "Current Directory: " << USER_PATH << endl;
		} else {
			cout << "Directory not found!" << endl;
		}
	} while (!fs::directory_entry{USER_PATH}.exists());
}

void fileListDir() {
	for (const auto& dirEntry : fs::directory_iterator(USER_PATH)) {
		cout << dirEntry.path() << '\n'; 
	}
}
