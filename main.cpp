#include "define.h"

const file_path DEFAULT_PATH = fs::current_path() / "music";

int userAction;
file_path userPath;
vector<file> userFiles;
vector<song> userSongs;

void clearConsole() {
	if (OS == "win") {
		system("clr");
	}
	else {
		system("clear");
	}
}

enum action 
{
	ACTION_END_TASK,
	ACTION_CHANGE_DIR,
	ACTION_LIST_FILES,
	ACTION_LIST_MUSICS,
	ACTION_PRINT_TAGS
};

int main() {
	clearConsole();
	changeDirectory(userPath, DEFAULT_PATH);
	scanDirectory(userFiles, userPath);

	cout 
	<< "Current Path: " << userPath << endl;
	do {
        cout << 
		"Select Action:\n"
		"(1) Change Directory\n"
		"(2) Print all files in current directory\n"
		"(3) Print all songs in current directory\n"
		"(4) Print tags in songs\n"
		"(0) End Task"
        << endl;
	
		cin >> userAction;
		cin.ignore();

		switch (userAction) {
			case ACTION_CHANGE_DIR: {
				clearConsole();
				string newPath;
				cout << "Input Path: " << endl;
				getline(cin, newPath);
				changeDirectory(userPath, fs::path(newPath));
				clearConsole();
				cout << "Current Path: " << userPath << endl;
				break;
			}
			case ACTION_LIST_FILES: {
				clearConsole();
				scanDirectory(userFiles, userPath);
				listFiles(userFiles);
				break;
			}
			case ACTION_LIST_MUSICS: {
				clearConsole();
				scanSongs(userSongs, userFiles);
				listSongs(userSongs);
				break;
			}
			case ACTION_PRINT_TAGS: {
				clearConsole();
				for (song &m : userSongs) {
					m.listMetadata();
				}
				break;
			}
		}
	} while (userAction != ACTION_END_TASK);
	
	return 0;
}