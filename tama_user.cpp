#include "tama_user.h"

user::user() {
	_songs.clear();

	changeDirectory(DEFAULT_PATH);
	scanDirectory();
}

file_path user::currentPath() {
	return _path;
}

void user::changeDirectory(const file_path newPath) {
	if (fs::exists(newPath)) {
		_path = newPath;
	}
}

void user::listDirectory() {
	cout << "Total Song(s): " << (_songs.size()) << '\n';
	for (const song &currentFile : _songs) {
		cout << currentFile.name() << '\n';
	}
	cout << endl;
}

song& user::selectSong(const int inputIndex) {
	return _songs[inputIndex];
}

void user::scanDirectory() {
	_songs.clear();

	for (const fs::directory_entry &entry : fs::directory_iterator(_path)) {
		if (entry.path().extension() == ".flac") {
			song newSong;
			newSong.setName(entry.path().filename());
			newSong.setPath(entry.path());
			newSong.setExtension(entry.path().extension());
			newSong.loadMetadata();
			_songs.push_back(move(newSong));
		}	
	}
}


