#include "tama_tag.h"

void printBytes(vector<byte> bytes) {
	for (byte b : bytes) {
		cout << setw(2) << setfill('0') << hex << static_cast<int>(b) << ' ';
	}
	cout << endl;
}

header encodeBEHeader(const length inputLength, const block_type inputType, const bool isLast) {
	header encodeHeader(4);
	encodeHeader[0] = (isLast ? byte(inputType | 0x80) : byte(inputType));
	encodeHeader[1] = byte((inputLength >> 16) & 0xFF);
	encodeHeader[2] = byte((inputLength >> 8) & 0xFF);
	encodeHeader[3] = byte(inputLength & 0xFF);
	return encodeHeader;
}

header encodeLEHeader(const length inputLength) {
	header encodeHeader(4);
	encodeHeader[0] = byte(inputLength & 0xFF);
	encodeHeader[1] = byte((inputLength >> 8) & 0xFF);
	encodeHeader[2] = byte((inputLength >> 16) & 0xFF);
	encodeHeader[3] = byte((inputLength >> 24) & 0xFF);
	return encodeHeader;
}

length decodeBEHeader(const header inputHeader) {
	return
	static_cast<uint8_t>(inputHeader[1]) << 16 |
	static_cast<uint8_t>(inputHeader[2]) << 8 |
	static_cast<uint8_t>(inputHeader[3]);
}

length decodeLEHeader(const header inputHeader) {
	return
	static_cast<uint8_t>(inputHeader[0]) |
	static_cast<uint8_t>(inputHeader[1]) << 8 |
	static_cast<uint8_t>(inputHeader[2]) << 16 |
	static_cast<uint8_t>(inputHeader[3]) << 24;
}

sample_rate song::sampleRate() {
	return
	(static_cast<uint8_t>(_streaminfo._body[10]) << 12) | 
	(static_cast<uint8_t>(_streaminfo._body[11]) << 4) |
	(static_cast<uint8_t>(_streaminfo._body[12]) >> 4);
}

channel_count song::channel() {
	return
	((static_cast<uint8_t>(_streaminfo._body[12]) & 0x0F) >> 1) + 1;
}

bits_per_sample song::bps() {
	return
	(((static_cast<uint8_t>(_streaminfo._body[12]) & 0x01) << 4) | 
	(static_cast<uint8_t>(_streaminfo._body[13]) >> 4)) + 1;
}

samples_count song::totalSamples() {
	return
	((static_cast<uint64_t>(_streaminfo._body[13]) & 0x0F) << 32) |
	(static_cast<uint32_t>(_streaminfo._body[14]) << 24) |
	(static_cast<uint16_t>(_streaminfo._body[15]) << 16) |
	(static_cast<uint8_t>(_streaminfo._body[16]) << 8)  |
	(static_cast<uint8_t>(_streaminfo._body[17]));
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
	return _vorbiusComment._userComments[pos]._field;
}

string song::userCommentContent(const string field) {
	for (user_comment i : _vorbiusComment._userComments) {
		if (i._field == field) {
			return i._content;
		}
	}
	return "NOT FOUND!";
}

metadata& song::metadataBlock(const block_type type) {
	switch (type)
	{
		case STREAMINFO:
		return _streaminfo;
		break;

		case PADDING:
		return _padding;
		break;

		case VORBIUS_COMMENT:
		return _vorbiusComment;
		break;

		default:
		break;
	}

	return _streaminfo;
}

void song::setStreaminfo(const body inputBody) {
	_streaminfo._isExist = true;
	_streaminfo._body = inputBody;
}

void song::setVorbiusComment(const body inputBody) {
 	_vorbiusComment._isExist = true;
	
	header vendorHeader(inputBody.begin(), inputBody.begin() + HEADER_SIZE);
	length vendorLength = decodeLEHeader(vendorHeader);
	
	body vendorBody(inputBody.begin() + HEADER_SIZE, inputBody.begin() + HEADER_SIZE + vendorLength);
	string vendor(reinterpret_cast<char*>(vendorBody.data()), vendorBody.size());
	_vorbiusComment._vendor = vendor;

	header userCommentHeader(inputBody.begin() + HEADER_SIZE + vendorLength, inputBody.begin() + HEADER_SIZE * 2 + vendorLength); 
	int userCommentCount = decodeLEHeader(userCommentHeader);
	body userCommentBody(inputBody.begin() + HEADER_SIZE * 2 + vendorLength, inputBody.end());

	for (int i = 0; i < userCommentCount; ++i) {
		header currentHeader(userCommentBody.begin(), userCommentBody.begin() + HEADER_SIZE);
		length currentLength = decodeLEHeader(currentHeader);
		
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

void song::setPadding(const length paddingLength) {
	_padding._isExist = true;	
	_padding._length = paddingLength;
}

void song::loadMetadata() {
	header metadataHeader(4);
	body metadataBody;
	fstream file(path(), ios::in);

	file.seekg(SIGNATURE_SIZE, ios::cur);
	
	do {
		file.read(reinterpret_cast<char*>(metadataHeader.data()), HEADER_SIZE);
		length metadataLength = decodeBEHeader(metadataHeader);
		metadataBody.resize(metadataLength);
		file.read(reinterpret_cast<char*>(metadataBody.data()), metadataLength);

		switch (static_cast<int>(metadataHeader[0]) & 0x0F) {
			case STREAMINFO:
			setStreaminfo(metadataBody);
			break;
			
			case PADDING:
			setPadding(metadataLength);
			break;
			
			case VORBIUS_COMMENT:
			setVorbiusComment(metadataBody);
			break;


			case APPLICATION: 
			case SEEKTABLE:
			case CUESHEET:
			case PICTURE:
			break;
		}
	} while ((static_cast<uint8_t>(metadataHeader[0]) & 0xF0) != 0x80);
	file.close();
}

void song::listStreaminfo() {
	size_t WIDTH = 20;
	
	cout
	<< setw(WIDTH) << "Filename" << ": " << name() << '\n'
	<< setw(WIDTH) << "Path" << ": " << path() << '\n'
	<< setw(WIDTH) << "Sample Rate" << ": " << sampleRate() << '\n'
	<< setw(WIDTH) << "Channel" << ": " << channel() << '\n'
	<< setw(WIDTH) << "Bits per sample" << ": " << bps() << '\n'
	<< setw(WIDTH) << "Total Samples" << ": " << totalSamples() << '\n'
	<< setw(WIDTH) << "Duration" << ": " << totalSecs() << " (" << minutes() << ':' << seconds() << ')' << '\n';
}

void song::listVorbiusComment() {
	cout
	<< _vorbiusComment._vendor << '\n';
	for (int i = 0; i < userCommentCount(); i++) {
		cout 
		<< setw(15) << ((_vorbiusComment._userComments[i]._isModified ? "*" : "") + _vorbiusComment._userComments[i]._field)
		<< ": " << _vorbiusComment._userComments[i]._content << '\n'; 
	}
}

void song::listMetadata() {
	cout << "====================================================================================================\n";
	
	listStreaminfo();

	cout << "\nUSER COMMENT SECTION:\n";

	listVorbiusComment();

	cout << "====================================================================================================\n" << endl;
}

void song::editUserComment(user_comment newUserComment) {
	string &newField = newUserComment._field;
	string newContent = newUserComment._content;

	transform(newField.begin(), newField.end(), newField.begin(), [](unsigned char c){return toupper(c);});

	for (user_comment &i : _vorbiusComment._userComments) {
		if (i._field == newField) {
			i._content = newContent;
			i._isModified = true;
			return;
		}
	}

	_vorbiusComment._userComments.push_back(newUserComment);
}

block song::encodeStreaminfo(const bool isLast) {
	block encodeBlock;
	header metadataHeader = encodeBEHeader(_streaminfo._length, _streaminfo._type, isLast);

	encodeBlock.insert(encodeBlock.end(), metadataHeader.begin(), metadataHeader.end());
	encodeBlock.insert(encodeBlock.end(), _streaminfo._body.begin(), _streaminfo._body.end());

	return encodeBlock;
}

block song::encodePadding(const bool isLast) {
	block encodeBlock;
	header metadataHeader = encodeBEHeader(_padding._length, _padding._type, isLast);
	
	encodeBlock.insert(encodeBlock.end(), metadataHeader.begin(), metadataHeader.end());
	

	block paddingBlock(_padding._length, byte(0));
	encodeBlock.insert(encodeBlock.end(), paddingBlock.begin(), paddingBlock.end());

	return encodeBlock;
}

block song::encodeVorbiusComment(const bool isLast) {
	block encodeBlock;
	header metadataHeader;

	header vendorHeader = encodeLEHeader(_vorbiusComment._vendor.size());
	encodeBlock.insert(encodeBlock.end(), vendorHeader.begin(), vendorHeader.end());
	encodeBlock.insert(encodeBlock.end(), 
		reinterpret_cast<byte*>(_vorbiusComment._vendor.data()), 
		reinterpret_cast<byte*>(_vorbiusComment._vendor.data() + _vorbiusComment._vendor.size())
	);

	header userCommentCount = encodeLEHeader(_vorbiusComment._userComments.size());
	encodeBlock.insert(encodeBlock.end(), userCommentCount.begin(), userCommentCount.end());

	for (user_comment i : _vorbiusComment._userComments) {
		string field = i._field;
		string content = i._content;
		string stringToEncode = field + '=' + content;
		header userCommentHeader = encodeLEHeader((stringToEncode.size()));

		encodeBlock.insert(encodeBlock.end(), userCommentHeader.begin(), userCommentHeader.end());

		encodeBlock.insert(encodeBlock.end(), 
			reinterpret_cast<byte*>(stringToEncode.data()),
			reinterpret_cast<byte*>(stringToEncode.data() + stringToEncode.size())
		);
	}

	length blockLength = encodeBlock.size();

	metadataHeader = encodeBEHeader(blockLength, _vorbiusComment._type, isLast);
	encodeBlock.insert(encodeBlock.begin(), metadataHeader.begin(), metadataHeader.end());

	return encodeBlock;
}

block song::encodeMetadata() {
	block encodeBlock;
	
	encodeBlock.insert(encodeBlock.end(), FLAC_SIGNATURE.begin(), FLAC_SIGNATURE.end());
	
	bool isLast;
	block_type lastType;
	
	vector<block_type> encodeType;

	vector<block_type> checkType = {
		STREAMINFO, VORBIUS_COMMENT, PADDING
	};

	for (block_type type : checkType) {
		if (metadataBlock(type)._isExist) {
			encodeType.insert(encodeType.end(), type);
		}
	}

	lastType = encodeType[encodeType.size() - 1];
	metadataBlock(lastType)._isLast = true;

	for (block_type type : encodeType) {
		block encodeMetadata;

		switch (type) {
			case STREAMINFO: {
				encodeMetadata = encodeStreaminfo(metadataBlock(type)._isLast);
				break;
			}
	
			case PADDING: {
				encodeMetadata = encodePadding(metadataBlock(type)._isLast);
				break;
			}
	
			case VORBIUS_COMMENT: {
				encodeMetadata = encodeVorbiusComment(metadataBlock(type)._isLast);
				break;
			}
	}
		encodeBlock.insert(encodeBlock.end(), encodeMetadata.begin(), encodeMetadata.end());
	}

	return encodeBlock;
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