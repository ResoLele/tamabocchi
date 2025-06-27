#include "tama_user.h"

User::User() {
	_files.clear();

	changeDirectory(DEFAULT_PATH);
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
	for (const std::unique_ptr<File>& file : _files) {
		if (T* temp = dynamic_cast<T*>(file.get())) {
			std::cout << '(' << i << ")\t" << temp->filename() << '\n';
			i++;
		}
	}
	std::cout << "Total File(s): " << --i << '\n';
	std::cout << '\n';
}

template <typename T>
T* User::getEntry(const int index) {
	std::vector<T*> filteredResult;

	for (const std::unique_ptr<File>& file : _files) {
		if (T* temp = dynamic_cast<T*>(file.get())) {
			filteredResult.push_back(temp);
		}
	}

	return filteredResult[index - 1];
}

// template <typename T>
// std::unique_ptr<T> User::createEntry(const std::filesystem::directory_entry entry) {
// 	std::unique_ptr<T> file = std::make_unique<T>();
// 	file->setName(entry.path().filename());
// 	file->setPath(entry.path());
// 	file->setExtension(entry.path().extension());
// 	return file;
// };

void User::scanDirectory() {
	_files.clear();

	for (const std::filesystem::directory_entry &entry : std::filesystem::recursive_directory_iterator(_path)) {
		if (entry.path().extension() == ".flac") {
			std::unique_ptr<Song> file = std::make_unique<Song>(entry);
			// file->lazyLoad();
			file->loadMetadata();
			_files.push_back(std::move(file));
		}	
		// else {
		// 	std::unique_ptr<File> file = std::make_unique<File>(entry);
		// 	_files.push_back(std::move(file));
		// }
	}
}

// include those else can't compile
template void User::listDirectory<File>();
template void User::listDirectory<Song>();

template File* User::getEntry<File>(const int);
template Song* User::getEntry<Song>(const int);

// template std::unique_ptr<File> User::createEntry(const std::filesystem::directory_entry);
// template std::unique_ptr<Song> User::createEntry(const std::filesystem::directory_entry);