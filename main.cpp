#include "define.h"

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

song& promptSelectSong(user& user) {
	int inputIndex;
	
	user.listDirectory();
	cout << "Select song by index: ";
	cin >> inputIndex;
	cin.ignore();
	
	return user.selectSong(inputIndex - 1);
}

enum action 
{
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
	user user;
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
	
		cin >> userAction;
		cin.ignore();

		switch (userAction) {
			case ACTION_CHANGE_DIR: {
				clearConsole();
				string inputPath;
				cout << "Input Path: " << endl;
				getline(cin, inputPath);
				if(!fs::exists(fs::path(inputPath))) {
					cout << "PATH DOES NOT EXIST" << endl;
					cout << "Current Path: " << user.currentPath() << endl;
					break;
				};
				user.changeDirectory(fs::path(inputPath));
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
				song& selectedSong = promptSelectSong(user);
				clearConsole();
				selectedSong.listMetadata();
				break;
			}
			
			case ACTION_EDIT_COMMENT: {
				user_comment newUserComment;
				newUserComment._isModified = true;

				clearConsole();
				song& selectedSong = promptSelectSong(user);
				clearConsole();
				selectedSong.listVorbiusComment();
				
				cout << "\nEnter field: " << endl;
				getline(cin, newUserComment._field);
				
				cout << "\nEnter Content: " << endl;
				getline(cin, newUserComment._content);
				
				clearConsole();
				cout << "New user comment: " << newUserComment._field << ": " << newUserComment._content << endl;
				
				selectedSong.editUserComment(newUserComment);
				selectedSong.listVorbiusComment();
				
				cout << '\n';
				break;
			}
			
			// case ACTION_SORT_COMMENT: {
			// 	break;
			// }
		
			case ACTION_SAVE_AS_NEW: {
				clearConsole();
				song& selectedSong = promptSelectSong(user);
				clearConsole();
				
				vector<byte> metadatas = selectedSong.encodeMetadata();

				fstream iStreamFile(selectedSong.path(), ios::in);
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
	} while (userAction != ACTION_END_TASK);
	
	return 0;
}