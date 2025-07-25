#include "tama_tag.h"

std::string Song::getTag(std::string field) {
	transform(field.begin(), field.end(), field.begin(), [](unsigned char c){return toupper(c);});
	if (_vorbiusComment._tags.find(field) != _vorbiusComment._tags.end()) {
		return _vorbiusComment._tags[field];
	}
	return "NOT FOUND!";
}

Song::Song(const FileEntry& entry) : File(entry) {
	_entry = entry;
}

void printBytes(std::vector<std::byte> bytes) {
	for (std::byte b : bytes) {
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(b) << ' ';
	}
	std::cout << '\n';
}

Header encodeBEHeader(const Size size, const MetadataType type, const bool isLast) {
	Header encodeHeader(4);
	encodeHeader[0] = (isLast ? std::byte(type | 0x80) : std::byte(type));
	encodeHeader[1] = std::byte((size >> 16) & 0xFF);
	encodeHeader[2] = std::byte((size >> 8) & 0xFF);
	encodeHeader[3] = std::byte(size & 0xFF);
	return encodeHeader;
}

Header encodeLEHeader(const Size size) {
	Header encodeHeader(4);
	encodeHeader[0] = std::byte(size & 0xFF);
	encodeHeader[1] = std::byte((size >> 8) & 0xFF);
	encodeHeader[2] = std::byte((size >> 16) & 0xFF);
	encodeHeader[3] = std::byte((size >> 24) & 0xFF);
	return encodeHeader;
}

Size decodeBEHeader(const Header header) {
	return
	static_cast<uint8_t>(header[1]) << 16 |
	static_cast<uint8_t>(header[2]) << 8 |
	static_cast<uint8_t>(header[3]);
}

Size decodeLEHeader(const Header header) {
	return
	static_cast<uint8_t>(header[0]) |
	static_cast<uint8_t>(header[1]) << 8 |
	static_cast<uint8_t>(header[2]) << 16 |
	static_cast<uint8_t>(header[3]) << 24;
}

SampleRate Song::sampleRate() {
	return
	(static_cast<uint8_t>(_streaminfo._body[10]) << 12) | 
	(static_cast<uint8_t>(_streaminfo._body[11]) << 4) |
	(static_cast<uint8_t>(_streaminfo._body[12]) >> 4);
}

ChannelCount Song::channel() {
	return
	((static_cast<uint8_t>(_streaminfo._body[12]) & 0x0F) >> 1) + 1;
}

BitsPerSample Song::bps() {
	return
	(((static_cast<uint8_t>(_streaminfo._body[12]) & 0x01) << 4) | 
	(static_cast<uint8_t>(_streaminfo._body[13]) >> 4)) + 1;
}

SampleCount Song::totalSamples() {
	return
	((static_cast<uint64_t>(_streaminfo._body[13]) & 0x0F) << 32) |
	(static_cast<uint32_t>(_streaminfo._body[14]) << 24) |
	(static_cast<uint16_t>(_streaminfo._body[15]) << 16) |
	(static_cast<uint8_t>(_streaminfo._body[16]) << 8)  |
	(static_cast<uint8_t>(_streaminfo._body[17]));
}

TimeSecs Song::totalSecs() {
	return static_cast<double>(totalSamples()) / static_cast<double>(sampleRate());
}

TimeMins Song::minutes() {
	return static_cast<int>(totalSecs() / 60); 
}

TimeSecs Song::seconds() {
	return fmod(totalSecs(), 60.0);
}

// size_t Song::userCommentCount() {
// 	return _vorbiusComment._userComment.size();
// }

// std::string Song::userCommentField(const uint32_t pos) {
// 	return _vorbiusComment._userComment;
// }

// std::string Song::userCommentContent(const std::string field) {
// 	for (HashMap i : _vorbiusComment._userComment) {
// 		if (i._field == field) {
// 			return i._content;
// 		}
// 	}
// 	return "NOT FOUND!";
// }

Metadata& Song::metadataBlock(const MetadataType type) {
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

void Song::setStreaminfo(const Body body) {
	_streaminfo._isExist = true;
	_streaminfo._body = body;
}

void Song::setVorbiusComment(const Body body) {
 	_vorbiusComment._isExist = true;
	
	Header vendorHeader(body.begin(), body.begin() + HEADER_SIZE);
	Size vendorLength = decodeLEHeader(vendorHeader);
	
	Body vendorBody(body.begin() + HEADER_SIZE, body.begin() + HEADER_SIZE + vendorLength);
	std::string vendor(reinterpret_cast<char*>(vendorBody.data()), vendorBody.size());
	
	_vorbiusComment._tags["VENDOR"] = vendor;

	Header userCommentHeader(body.begin() + HEADER_SIZE + vendorLength, body.begin() + HEADER_SIZE * 2 + vendorLength); 
	int userCommentCount = decodeLEHeader(userCommentHeader);
	Body userCommentBody(body.begin() + HEADER_SIZE * 2 + vendorLength, body.end());

	for (int i = 0; i < userCommentCount; ++i) {
		Header currentHeader(userCommentBody.begin(), userCommentBody.begin() + HEADER_SIZE);
		Size currentLength = decodeLEHeader(currentHeader);
		
		Body currentBody(userCommentBody.begin() + HEADER_SIZE, userCommentBody.begin() + HEADER_SIZE + currentLength);
		std::string currentString(reinterpret_cast<char*>(currentBody.data()), currentBody.size());
		
		userCommentBody.erase(userCommentBody.begin(), userCommentBody.begin() + HEADER_SIZE + currentLength);

		Size fieldLength = currentString.find('=');
		std::string field = currentString.substr(0, fieldLength);

		// from cppreference
		transform(field.begin(), field.end(), field.begin(), [](unsigned char c){return toupper(c);});
		std::string content = currentString.substr(fieldLength + 1);

		_vorbiusComment._tags[field] = content;
		// _vorbiusComment._userComment.push_back({field, content});
	}
}

void Song::lazyLoad() {
	Header signature(4);
	Header metadataHeader(4);
	Body metadataBody;
	std::fstream file(path(), std::ios::in);
	file.read(reinterpret_cast<char*>(signature.data()), HEADER_SIZE);
	if (signature == FLAC_SIGNATURE) {
		std::cout << "LOADING: HAHA" << path() << '\n';
		do {
			file.read(reinterpret_cast<char*>(metadataHeader.data()), HEADER_SIZE);
			Size metadataLength = decodeBEHeader(metadataHeader);
	
			if ((static_cast<int>(metadataHeader[0]) & 0x0F) == VORBIUS_COMMENT) {
				metadataBody.resize(metadataLength);
				file.read(reinterpret_cast<char*>(metadataBody.data()), metadataLength);
				setVorbiusComment(metadataBody);
			}
			else {
				file.seekg(metadataLength, std::ios::cur);
				std::cout << "SKIP!\n";
			}
	
		} while ((static_cast<uint8_t>(metadataHeader[0]) & 0xF0) != 0x80);
	}
	// file.seekg(SIGNATURE_SIZE, std::ios::cur);
	file.close();
}

void Song::setPadding(const Size paddingLength) {
	_padding._isExist = true;	
	_padding._length = paddingLength;
}

void Song::loadMetadata() {
	Header signature(4);
	Header metadataHeader(4);
	Body metadataBody;
	std::fstream file(path(), std::ios::in);

	// file.seekg(SIGNATURE_SIZE, std::ios::cur);
	file.read(reinterpret_cast<char*>(signature.data()), HEADER_SIZE);
	if (signature == FLAC_SIGNATURE) {
		std::cout << "LOADING: " << path() << '\n';
		do {
			file.read(reinterpret_cast<char*>(metadataHeader.data()), HEADER_SIZE);
			Size metadataLength = decodeBEHeader(metadataHeader);
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
	}
	file.close();
}

void Song::listStreaminfo() {
	size_t WIDTH = 20;
	
	std::cout
	<< std::setw(WIDTH) << "Filename" << ": " << filename() << '\n'
	<< std::setw(WIDTH) << "Path" << ": " << path() << '\n'
	<< std::setw(WIDTH) << "Sample Rate" << ": " << sampleRate() << '\n'
	<< std::setw(WIDTH) << "Channel" << ": " << channel() << '\n'
	<< std::setw(WIDTH) << "Bits per sample" << ": " << bps() << '\n'
	<< std::setw(WIDTH) << "Total Samples" << ": " << totalSamples() << '\n'
	<< std::setw(WIDTH) << "Duration" << ": " << totalSecs() << " (" << minutes() << ':' << seconds() << ')' << '\n';
}

void Song::listVorbiusComment() {
	for (const std::pair<const std::string, const std::string>& pair : _vorbiusComment._tags) {
		std::cout << std::setw(30) << pair.first << ": " << pair.second << '\n'; 
	}
}

void Song::listMetadata() {
	std::cout << "====================================================================================================\n";
	
	// listStreaminfo();

	std::cout << "\nUSER COMMENT SECTION:\n";

	listVorbiusComment();

	std::cout << "====================================================================================================\n" << '\n';
}

void Song::setTag(std::string field, const std::string content) {
	transform(field.begin(), field.end(), field.begin(), [](unsigned char c){return toupper(c);});
	_vorbiusComment._tags[field] = content;
}

Block Song::encodeStreaminfo(const bool isLast) {
	Block encodeBlock;
	Header metadataHeader = encodeBEHeader(_streaminfo._length, _streaminfo._type, isLast);

	encodeBlock.insert(encodeBlock.end(), metadataHeader.begin(), metadataHeader.end());
	encodeBlock.insert(encodeBlock.end(), _streaminfo._body.begin(), _streaminfo._body.end());

	return encodeBlock;
}

Block Song::encodePadding(const bool isLast) {
	Block encodeBlock;
	Header metadataHeader = encodeBEHeader(_padding._length, _padding._type, isLast);
	
	encodeBlock.insert(encodeBlock.end(), metadataHeader.begin(), metadataHeader.end());
	

	Block paddingBlock(_padding._length, std::byte(0));
	encodeBlock.insert(encodeBlock.end(), paddingBlock.begin(), paddingBlock.end());

	return encodeBlock;
}

// Block Song::encodeVorbiusComment(const bool isLast) {
// 	Block encodeBlock;
// 	Header metadataHeader;

// 	Header vendorHeader = encodeLEHeader(_vorbiusComment._vendor.size());
// 	encodeBlock.insert(encodeBlock.end(), vendorHeader.begin(), vendorHeader.end());
// 	encodeBlock.insert(encodeBlock.end(), 
// 		reinterpret_cast<std::byte*>(_vorbiusComment._vendor.data()), 
// 		reinterpret_cast<std::byte*>(_vorbiusComment._vendor.data() + _vorbiusComment._vendor.size())
// 	);

// 	Header userCommentCount = encodeLEHeader(_vorbiusComment._userComment.size());
// 	encodeBlock.insert(encodeBlock.end(), userCommentCount.begin(), userCommentCount.end());

// 	for (HashMap i : _vorbiusComment._userComment) {
// 		std::string field = i._field;
// 		std::string content = i._content;
// 		std::string stringToEncode = field + '=' + content;
// 		Header userCommentHeader = encodeLEHeader((stringToEncode.size()));

// 		encodeBlock.insert(encodeBlock.end(), userCommentHeader.begin(), userCommentHeader.end());

// 		encodeBlock.insert(encodeBlock.end(), 
// 			reinterpret_cast<std::byte*>(stringToEncode.data()),
// 			reinterpret_cast<std::byte*>(stringToEncode.data() + stringToEncode.size())
// 		);
// 	}

// 	Size blockLength = encodeBlock.size();

// 	metadataHeader = encodeBEHeader(blockLength, _vorbiusComment._type, isLast);
// 	encodeBlock.insert(encodeBlock.begin(), metadataHeader.begin(), metadataHeader.end());

// 	return encodeBlock;
// }

Block Song::encodeMetadata() {
	Block encodeBlock;
	
	encodeBlock.insert(encodeBlock.end(), FLAC_SIGNATURE.begin(), FLAC_SIGNATURE.end());
	
	bool isLast;
	MetadataType lastType;
	
	std::vector<MetadataType> encodeType;

	std::vector<MetadataType> checkType = {
		STREAMINFO, VORBIUS_COMMENT, PADDING
	};

	for (MetadataType type : checkType) {
		if (metadataBlock(type)._isExist) {
			encodeType.insert(encodeType.end(), type);
		}
	}

	lastType = encodeType[encodeType.size() - 1];
	metadataBlock(lastType)._isLast = true;

	for (MetadataType type : encodeType) {
		Block encodeMetadata;

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
				// encodeMetadata = encodeVorbiusComment(metadataBlock(type)._isLast);
				break;
			}
	}
		encodeBlock.insert(encodeBlock.end(), encodeMetadata.begin(), encodeMetadata.end());
	}

	return encodeBlock;
}

// void scanSongs(std::vector<Song>& songs, std::vector<File>& files) {
// 	songs.clear();
// 	for (int i = files.size() - 1; i >= 0; --i) {
// 		if (files[i].extension() == ".flac") {
// 			Song song;
// 			song.setName(files[i].filename());
// 			song.setPath(files[i].path());
// 			song.setExtension(files[i].extension());
// 			song.loadMetadata();
// 			songs.push_back(song);
// 			files.erase(files.begin() + i);
// 		}
// 	}
// }

// Outdated
// void listSongs(const std::vector<Song>& songs) {
// 	std::cout << "Total Song(s): " << songs.size() << '\n';
// 	for (const Song &song : songs) {
// 		std::cout << song.filename() << '\n';
// 	}
// 	std::cout << '\n';
// }