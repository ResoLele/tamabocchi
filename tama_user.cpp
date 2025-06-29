#include "tama_user.h"

User::User(const FilePath path) {
	_files.clear();

	changeDirectory(path);
	scanDirectory();
}

FilePath User::currentPath() {
	return _path;
}

void User::changeDirectory(const FilePath newPath) {
	if (std::filesystem::exists(newPath)) {
		_path = newPath;
	}
}

template <typename T>
void User::listDirectory() {
	int i = 1;
	for (const std::pair<const FilePath, std::unique_ptr<File>>& file : _files) {
		if (T* temp = dynamic_cast<T*>(file.second.get())) {
			std::cout << '(' << i << ")\t" << temp->filename() << '\n';
			i++;
		}
	}
	std::cout << "Total File(s): " << --i << '\n';
	std::cout << '\n';
}

void User::scanDirectory() {
	_files.clear();
	_albumMap.clear();

	for (const std::filesystem::directory_entry &entry : std::filesystem::recursive_directory_iterator(_path)) {
		if (entry.path().extension() == ".flac") {
			std::unique_ptr<Song> song = std::make_unique<Song>(entry);
			// file->lazyLoad();
			song->loadMetadata();
			_albumMap[song->getTag("ALBUM")].push_back(song.get());
			std::sort(_albumMap[song->getTag("ALBUM")].begin(), _albumMap[song->getTag("ALBUM")].end(), [](Song* m, Song* n) {
				return m->filename() < n->filename();
			});
			_files[song->path()] = std::move(song);
		}	
		// else {
		// 	std::unique_ptr<File> file = std::make_unique<File>(entry);
		// 	_files[file->path()] = std::move(file);
		// }
	}
}

void User::searchHashMap(const std::string search) {
	for (const std::pair<const FilePath, std::unique_ptr<File>>& file : _files) {
		if (file.first.string().find(search) != STRING_NOT_FOUND) {
			std::cout << file.first << '\n';
		}
	}
}

void User::listAlbumMap() {
	for (const std::pair<const std::string, const std::vector<Song*>>& album : _albumMap) {
		std::cout << album.first << '\n';
		for (const Song* song : album.second) {
			std::cout << '\t' << song->filename() << '\n';
		}
	}
}

// include those for templates else can't compile
template void User::listDirectory<File>();
template void User::listDirectory<Song>();