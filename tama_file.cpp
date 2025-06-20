#include "define.h"

using namespace std;
namespace fs = std::filesystem;

// Class file
void file::setFilename(const string str) {_filename = str;}
void file::setPath(const fs::path str) {_path = str;}
void file::setSampleRate(const uint32_t sampleRate) {_sampleRate = sampleRate;}
void file::setChannel(const uint16_t channel) {_channel = channel;}
void file::setSPB(const uint16_t SPB) {_SPB = SPB;}
void file::setSamples(const uint64_t samples) {_samples = samples;}
void file::setTitle(const string str) {_title = str;}
void file::setAlbum(const string str) {_album = str;}
void file::setDate(const string str) {_date = str;}

string file::filename() {return _filename;}
string file::path() {return _path;}
uint32_t file::sampleRate() {return _sampleRate;}
uint16_t file::channel() {return _channel;}
uint16_t file::SPB() {return _SPB;}
uint64_t file::samples() {return _samples;}
string file::title() {return _title;}
string file::album() {return _album;}
string file::date() {return _date;}

// Change Directory
fs::path dirChange() {
	string userInput;

	while(true) {
		cout << "Enter Path: (Leave empty using current path)" << endl;
		getline(cin, userInput);
		if (!userInput.length()) {
			cout << "Using current path" << endl;
			return (fs::current_path() / "music");
		}
		else if (fs::exists(fs::path(userInput))) {
			return fs::path(userInput);
		}
	}
}

bool dirScan(fs::path path) {

	int count;

	for (const fs::directory_entry dirEntry : fs::directory_iterator(path)) {
		if (dirEntry.path().extension() == ".flac") {
			count++;
			file music;
			music.setFilename(dirEntry.path().filename());
			music.setPath(dirEntry.path());
			files.push_back(music);
		}
	}
	return (count > 0);
}

void dirPrint(fs::path path) {
	cout << "File count: "<< files.size() << endl;
	for (file i : files) {
		cout << i.filename() << endl;
	}
	cout << endl;
}