#include "tama_user.h"

User::User() {
	_songs.clear();

	changeDirectory(DEFAULT_PATH);
	scanDirectory();
}

FilePath User::currentPath() {
	return _path;
}

void User::changeDirectory(const FilePath newPath) {
	if (fs::exists(newPath)) {
		_path = newPath;
	}
}

void User::listDirectory() {
	cout << "Total Song(s): " << (_songs.size()) << '\n';
	for (const Song &currentFile : _songs) {
		cout << currentFile.name() << '\n';
	}
	cout << endl;
}

Song& User::song(const int inputIndex) {
	return _songs[inputIndex];
}

void User::scanDirectory() {
	_songs.clear();

	for (const fs::directory_entry &entry : fs::directory_iterator(_path)) {
		if (entry.path().extension() == ".flac") {
			Song newSong;
			newSong.setName(entry.path().filename());
			newSong.setPath(entry.path());
			newSong.setExtension(entry.path().extension());
			newSong.loadMetadata();
			_songs.push_back(move(newSong));
		}	
	}
}


