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

// Song* promptSelectSong(User& user) {
// 	int input;
	
// 	user.listDirectory<Song>();
// 	std::cout << "Select by index: ";
// 	std::cin >> input;
// 	std::cin.ignore();
	
// 	return user.getEntry<Song>(input);
// }

enum Action {
	ACTION_END_TASK,
	ACTION_CHANGE_DIR,
	ACTION_LIST_FILES,
	ACTION_PRINT_TAGS,
	ACTION_EDIT_COMMENT,
	// ACTION_SORT_COMMENT,
	// ACTION_ENCODE,
	ACTION_SAVE_AS_NEW,
};

int main() {

	int action;

	User user(DEFAULT_PATH);
	// clearConsole();
	
	std::cout 
	<< "Current Path: " << user.currentPath() << '\n';
	user.listAlbumMap();
	std::cout << '\n';

	do {
        std::cout <<
		"Select Action:\n"
		"(1) Change Directory\n"
		"(2) List all songs in current directory\n"
		"(3) Print song's tags\n"
		"(4) Edit user comment in songs\n"
		"(5) Save file\n"
		"(0) End Task"
        << '\n';
	
		std::cin >> action;
		std::cin.ignore();

		switch (action) {
			case ACTION_CHANGE_DIR: {
				clearConsole();
				std::string path;
				std::cout << "Input Path: " << '\n';
				getline(std::cin, path);
				if(!std::filesystem::exists(FilePath(path))) {
					std::cout << "PATH DOES NOT EXIST" << '\n';
					std::cout << "Current Path: " << user.currentPath() << '\n';
					break;
				};
				user.changeDirectory(FilePath(path));
				user.scanDirectory();
				user.listAlbumMap();
				std::cout << '\n';
				std::cout << "Current Path: " << user.currentPath() << '\n';
				break;
			}
			
			case ACTION_LIST_FILES: {
				clearConsole();
				user.listDirectory<File>();
				break;
			}
			
			case ACTION_PRINT_TAGS: {
				clearConsole();
				std::string input;
				std::cin >> input;
				user.searchHashMap(input);
				// File* result = user.searchHashMap(input);
				// std::cout << result->filename();
			// 	Song* song = promptSelectSong(user);
			// 	clearConsole();
			// 	song->listMetadata();
				break;
			}
			
			// case ACTION_EDIT_COMMENT: {
			// 	HashMap userComment;
			// 	userComment._isModified = true;

			// 	clearConsole();
			// 	Song* selectedSong = promptSelectSong(user);
			// 	clearConsole();
			// 	selectedSong->listVorbiusComment();
				
			// 	std::cout << "\nEnter field: " << '\n';
			// 	std::getline(std::cin, userComment._field);
				
			// 	std::cout << "\nEnter Content: " << '\n';
			// 	std::getline(std::cin, userComment._content);
				
			// 	clearConsole();
			// 	std::cout << "New user comment: " << userComment._field << ": " << userComment._content << '\n';
				
			// 	selectedSong->editUserComment(userComment);
			// 	selectedSong->listVorbiusComment();
				
			// 	std::cout << '\n';
			// 	break;
			// }
			
			// case ACTION_SORT_COMMENT: {
			// 	break;
			// }
		
			// case ACTION_SAVE_AS_NEW: {
			// 	clearConsole();
			// 	Song* song = promptSelectSong(user);
			// 	clearConsole();
				
			// 	std::vector<std::byte> metadatas = song->encodeMetadata();

			// 	std::fstream iStreamFile(song->path(), std::ios::in);
			// 	iStreamFile.seekg(metadatas.size(), std::ios::beg);
				
			// 	std::fstream oStreamFile(DEFAULT_PATH / "test.flac", std::ios::out);
			// 	oStreamFile.write(reinterpret_cast<const char*>(metadatas.data()), metadatas.size());
			// 	oStreamFile.seekg(metadatas.size(), std::ios::beg);

			// 	while (iStreamFile) {
			// 		std::vector<char> buffer(4096);
			// 		iStreamFile.read(buffer.data(), buffer.size());
			// 		oStreamFile.write(buffer.data(), iStreamFile.gcount());
			// 	}

			// 	iStreamFile.close();
			// 	oStreamFile.close();
			// 	break;
			// }
		}
	} while (action != ACTION_END_TASK);
	
	return 0;
}