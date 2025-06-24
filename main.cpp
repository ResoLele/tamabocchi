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
	ACTION_PRINT_TAGS,
	ACTION_EDIT_COMMENT,
	ACTION_SORT_COMMENT,
	ACTION_ENCODE
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
		"(2) Scan all files in current directory\n"
		"(3) Scan all songs in current directory\n"
		"(4) Print tags in songs\n"
		"(5) Edit user comment in songs\n"
		"(6) Sort user comment\n"
		"(7) Encode\n"
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
			
			case ACTION_EDIT_COMMENT: {
				int inputIndex;
				user_comment newUserComment;
				newUserComment._isModified = true;

				clearConsole();
				listSongs(userSongs);
				cout << "Select song: " << endl;
				cin >> inputIndex;
				cin.ignore();
				clearConsole();
				userSongs[inputIndex - 1].listVorbiusComment();
				
				cout << "\nEnter field: " << endl;
				getline(cin, newUserComment._field);
				
				cout << "\nEnter Content: " << endl;
				getline(cin, newUserComment._content);
				
				clearConsole();
				cout << "New user comment: " << newUserComment._field << ": " << newUserComment._content << endl;
				
				userSongs[inputIndex - 1].editUserComment(newUserComment);
				userSongs[inputIndex - 1].listVorbiusComment();
				
				cout << '\n';
				break;
			}
			
			case ACTION_SORT_COMMENT: {
				break;
			}
			
			case ACTION_ENCODE: {
				int inputIndex;
				
				clearConsole();
				listSongs(userSongs);
				cout << "Select song: " << endl;
				cin >> inputIndex;
				cin.ignore();
				clearConsole();
				
				song &selectedSong = userSongs[inputIndex - 1];

				// selectedSong.setVorbiusComment(selectedSong.encodeVorbiusComment());
				
				// selectedSong.listVorbiusComment();

				selectedSong.encodeMetadata();
				
				break;
			}
		}
	} while (userAction != ACTION_END_TASK);
	
	return 0;
}