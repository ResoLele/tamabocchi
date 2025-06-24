#include "define.h"

const size_t BUFFER_SIZE = 4096;

void clearConsole() {
	if (OS == "win") {
		system("clr");
	}
	else {
		system("clear");
	}
}

Song& promptSelectSong(User& user) {
	int input;
	
	user.listDirectory();
	cout << "Select song by index: ";
	cin >> input;
	cin.ignore();
	
	return user.song(input - 1);
}

enum Action {
	ACTION_END_TASK,
	ACTION_CHANGE_DIR,
	ACTION_LIST_SONGS,
	ACTION_PRINT_TAGS,
	ACTION_EDIT_COMMENT,
	// ACTION_SORT_COMMENT,
	// ACTION_ENCODE,
	ACTION_SAVE_AS_NEW,
};

int main() {
	int action;

	User user;
	clearConsole();
	
	cout 
	<< "Current Path: " << user.currentPath() << endl;
	do {
        cout <<
		"Select Action:\n"
		"(1) Change Directory\n"
		"(2) List all songs in current directory\n"
		"(3) Print song's tags\n"
		"(4) Edit user comment in songs\n"
		"(5) Save file\n"
		"(0) End Task"
        << endl;
	
		cin >> action;
		cin.ignore();

		switch (action) {
			case ACTION_CHANGE_DIR: {
				clearConsole();
				string path;
				cout << "Input Path: " << endl;
				getline(cin, path);
				if(!fs::exists(fs::path(path))) {
					cout << "PATH DOES NOT EXIST" << endl;
					cout << "Current Path: " << user.currentPath() << endl;
					break;
				};
				user.changeDirectory(fs::path(path));
				user.scanDirectory();
				cout << "Current Path: " << user.currentPath() << endl;
				break;
			}
			
			case ACTION_LIST_SONGS: {
				clearConsole();
				user.listDirectory();
				break;
			}
			
			case ACTION_PRINT_TAGS: {
				clearConsole();
				Song& song = promptSelectSong(user);
				clearConsole();
				song.listMetadata();
				break;
			}
			
			case ACTION_EDIT_COMMENT: {
				UserComment userComment;
				userComment._isModified = true;

				clearConsole();
				Song& selectedSong = promptSelectSong(user);
				clearConsole();
				selectedSong.listVorbiusComment();
				
				cout << "\nEnter field: " << endl;
				getline(cin, userComment._field);
				
				cout << "\nEnter Content: " << endl;
				getline(cin, userComment._content);
				
				clearConsole();
				cout << "New user comment: " << userComment._field << ": " << userComment._content << endl;
				
				selectedSong.editUserComment(userComment);
				selectedSong.listVorbiusComment();
				
				cout << '\n';
				break;
			}
			
			// case ACTION_SORT_COMMENT: {
			// 	break;
			// }
		
			case ACTION_SAVE_AS_NEW: {
				clearConsole();
				Song& song = promptSelectSong(user);
				clearConsole();
				
				vector<byte> metadatas = song.encodeMetadata();

				fstream iStreamFile(song.path(), ios::in);
				iStreamFile.seekg(metadatas.size(), ios::beg);
				
				fstream oStreamFile(DEFAULT_PATH / "test.flac", ios::out);
				oStreamFile.write(reinterpret_cast<const char*>(metadatas.data()), metadatas.size());
				oStreamFile.seekg(metadatas.size(), ios::beg);

				while (iStreamFile) {
					vector<char> buffer(4096);
					iStreamFile.read(buffer.data(), buffer.size());
					oStreamFile.write(buffer.data(), iStreamFile.gcount());
				}

				iStreamFile.close();
				oStreamFile.close();
				break;
			}
		}
	} while (action != ACTION_END_TASK);
	
	return 0;
}