#include "tama_tag.h"

length readBEHeader(const header inputHeader) {
	return
	static_cast<uint8_t>(inputHeader[1]) << 16 |
	static_cast<uint8_t>(inputHeader[2]) << 8 |
	static_cast<uint8_t>(inputHeader[3]);
}

length readLEHeader(const header inputHeader) {
	return
	static_cast<uint8_t>(inputHeader[0]) |
	static_cast<uint8_t>(inputHeader[1]) << 8 |
	static_cast<uint8_t>(inputHeader[2]) << 16 |
	static_cast<uint8_t>(inputHeader[3]) << 24;
}

sample_rate song::sampleRate() {
	return
	(static_cast<uint8_t>(_streaminfo[10]) << 12) | 
	(static_cast<uint8_t>(_streaminfo[11]) << 4) |
	(static_cast<uint8_t>(_streaminfo[12]) >> 4);
}

channel_count song::channel() {
	return
	((static_cast<uint8_t>(_streaminfo[12]) & 0x0F) >> 1) + 1;
}

bits_per_sample song::bps() {
	return
	(((static_cast<uint8_t>(_streaminfo[12]) & 0x01) << 4) | 
	(static_cast<uint8_t>(_streaminfo[13]) >> 4)) + 1;
}

samples_count song::totalSamples() {
	return
	((static_cast<uint64_t>(_streaminfo[13]) & 0x0F) << 32) |
	(static_cast<uint32_t>(_streaminfo[14]) << 24) |
	(static_cast<uint16_t>(_streaminfo[15]) << 16) |
	(static_cast<uint8_t>(_streaminfo[16]) << 8)  |
	(static_cast<uint8_t>(_streaminfo[17]));
}

time_secs song::totalSecs() {
	return static_cast<double>(totalSamples()) / static_cast<double>(sampleRate());
}

time_mins song::minutes() {
	return static_cast<int>(totalSecs() / 60); 
}

time_secs song::seconds() {
	return fmod(totalSecs(), 60.0);
}

size_t song::userCommentCount() {
	return _vorbiusComment._userComments.size();
}

string song::userCommentField(const uint32_t pos) {
	return _vorbiusComment._userComments[pos].first;
}

string song::userCommentContent(const string field) {
	for (user_comment i : _vorbiusComment._userComments) {
		if (i.first == field) {
			return i.second;
		}
	}
	return "NOT FOUND!";
}

void song::setStreaminfo(const body inputBody) {
	_streaminfo = inputBody;
}

void song::setVorbiusComment(const body inputBody) {
	header vendorHeader(inputBody.begin(), inputBody.begin() + HEADER_SIZE);
	length vendorLength = readLEHeader(vendorHeader);
	
	body vendorBody(inputBody.begin() + HEADER_SIZE, inputBody.begin() + HEADER_SIZE + vendorLength);
	string vendor(reinterpret_cast<char*>(vendorBody.data()), vendorBody.size());
	_vorbiusComment._vendorHeader = vendorHeader;
	_vorbiusComment._vendor = vendor;

	header userCommentHeader(inputBody.begin() + HEADER_SIZE + vendorLength, inputBody.begin() + HEADER_SIZE * 2 + vendorLength); 
	int userCommentCount = readLEHeader(userCommentHeader);
	body userCommentBody(inputBody.begin() + HEADER_SIZE * 2 + vendorLength, inputBody.end());

	for (int i = 0; i < userCommentCount; ++i) {
		header currentHeader(userCommentBody.begin(), userCommentBody.begin() + HEADER_SIZE);
		length currentLength = readLEHeader(currentHeader);
		
		body currentBody(userCommentBody.begin() + HEADER_SIZE, userCommentBody.begin() + HEADER_SIZE + currentLength);
		string currentString(reinterpret_cast<char*>(currentBody.data()), currentBody.size());
		
		userCommentBody.erase(userCommentBody.begin(), userCommentBody.begin() + HEADER_SIZE + currentLength);

		length fieldLength = currentString.find('=');
		string field = currentString.substr(0, fieldLength);

		// from cppreference
		transform(field.begin(), field.end(), field.begin(), [](unsigned char c){return toupper(c);});
		string content = currentString.substr(fieldLength + 1);

		_vorbiusComment._userComments.push_back({field, content});
	}
}

void song::loadMetadata() {
	header metadataHeader(4);
	body metadataBody;
	fstream file(path(), ios::in);

	file.seekg(SIGNATURE_SIZE, ios::cur);
	
	do {
		file.read(reinterpret_cast<char*>(metadataHeader.data()), HEADER_SIZE);
		length metadataLength = readBEHeader(metadataHeader);
		metadataBody.resize(metadataLength);
		file.read(reinterpret_cast<char*>(metadataBody.data()), metadataLength);

		switch (static_cast<int>(metadataHeader[0])) {
			case STREAMINFO:
			setStreaminfo(metadataBody);
			break;

			case VORBIUS_COMMENT:
			setVorbiusComment(metadataBody);
			break;

			case PADDING:
			case APPLICATION: 
			case SEEKTABLE:
			case CUESHEET:
			case PICTURE:
			case INVALID:
			break;
		}
	} while ((static_cast<uint8_t>(metadataHeader[0]) & 0xF0) != 0x80);
	file.close();
}

void song::listMetadata() {
	cout
	<< "====================================================================================================\n"
	<< setw(30) << "Filename" << ": " << name() << '\n'
	<< setw(30) << "Path" << ": " << path() << '\n'
	<< setw(30) << "Sample Rate" << ": " << sampleRate() << '\n'
	<< setw(30) << "Channel" << ": " << channel() << '\n'
	<< setw(30) << "Bits per sample" << ": " << bps() << '\n'
	<< setw(30) << "Total Samples" << ": " << totalSamples() << '\n'
	<< setw(30) << "Duration" << ": " << totalSecs() << " (" << minutes() << ':' << seconds() << ')' << '\n'
	<< "\nUSER COMMENT SECTION:"
	<< '\n';

	for (int i = 0; i < userCommentCount(); i++) {
		cout << setw(30) << userCommentField(i) << ": " << userCommentContent(userCommentField(i)) << '\n'; 
	}
	cout << "====================================================================================================\n" << endl;
}

void scanSongs(vector<song>& songs, vector<file>& files) {
	songs.clear();
	for (int i = files.size() - 1; i >= 0; --i) {
		if (files[i].extension() == ".flac") {
			song m;
			m.setName(files[i].name());
			m.setPath(files[i].path());
			m.setExtension(files[i].extension());
			m.loadMetadata();
			songs.push_back(m);
			files.erase(files.begin() + i);
		}
	}
}

void listSongs(const vector<song>& songs) {
	cout << "Total Song(s): " << songs.size() << '\n';
	for (const song &m : songs) {
		cout << m.name() << '\n';
	}
	cout << endl;
}