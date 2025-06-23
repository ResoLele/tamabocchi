#include "define.h"

// helper functions
uint32_t readLEHeader(vector<byte> bytes) {
	return 
	static_cast<uint8_t>(bytes[0]) |
	static_cast<uint8_t>(bytes[1]) << 8 |
	static_cast<uint8_t>(bytes[2]) << 16 |
	static_cast<uint8_t>(bytes[3]) << 24;
}

uint32_t readBEHeader(vector<byte> bytes) {
	return
	static_cast<uint8_t>(bytes[1]) << 16 |
	static_cast<uint8_t>(bytes[2]) << 8 |
	static_cast<uint8_t>(bytes[3]);
}

// getter
string file::magic() {return _magic;}

uint32_t music::sampleRate() {
	return (
	(static_cast<uint8_t>(_streaminfo[10]) << 12) | 
	(static_cast<uint8_t>(_streaminfo[11]) << 4) |
	(static_cast<uint8_t>(_streaminfo[12]) >> 4)
	);
}

// I don't fucking know why using uint8_t does not work
uint16_t music::channel() {
	return (
	((static_cast<uint8_t>(_streaminfo[12]) & 0x0F) >> 1) + 1
	);
}

uint16_t music::bps() {
	return (
	(((static_cast<uint8_t>(_streaminfo[12]) & 0x01) << 4) | 
	(static_cast<uint8_t>(_streaminfo[13]) >> 4)) + 1
	);
}

uint64_t music::samples() {
	return (
	((static_cast<uint64_t>(_streaminfo[13]) & 0x0F) << 32) |
	(static_cast<uint32_t>(_streaminfo[14]) << 24) |
	(static_cast<uint16_t>(_streaminfo[15]) << 16) |
	(static_cast<uint8_t>(_streaminfo[16]) << 8)  |
	(static_cast<uint8_t>(_streaminfo[17]))
	);
}

double music::totalsecs() {
	return static_cast<double>(samples()) / static_cast<double>(sampleRate());
}

uint16_t music::minutes() {
	return static_cast<int>(totalsecs() / 60); 
}

double music::seconds() {
	return fmod(totalsecs(), 60.0);
}

unsigned int music::userCommentCount() {
	return _vorbiusComment._userComments.size();
}

string music::userCommentField(unsigned int keyPos) {
	return _vorbiusComment._userComments[keyPos].first;
} 

string music::userComment(string field) {
	for (userComment_t i : _vorbiusComment._userComments) {
		if (i.first == field) {
			return i.second;
		}
	}
	return "NOT FOUND!";
}

// Setter
void music::setStreaminfo(vector<byte> block) {_streaminfo = block;}

// Read from file & Set all
void music::setVorbiusComment(vector<byte> block) {
	// first subblock : vendor 
	vector<byte> vendorHeader(block.begin(), block.begin() + 4);
	uint32_t vendorLength = readLEHeader(vendorHeader);
	vector<byte> vendorBlock(block.begin() + 4, block.begin() + 4 + vendorLength);
	string vendor(reinterpret_cast<char*>(vendorBlock.data()), vendorBlock.size());
	_vorbiusComment._vendor = vendor;
	
	// check how many user comments
	vector<byte> userCommentHeader(block.begin() + vendorLength + 4, block.begin() + vendorLength + 8);
	uint32_t userCommentCount = readLEHeader(userCommentHeader);
	
	vector<byte> userCommentBlock(block.begin() + vendorLength + 8, block.end());

	// Store each user comment block into userComment structure
	// then store all as a vector
	for (int i = 0; i < userCommentCount; i++) {
		// Get those bullshit (DGAF It works now)
		vector<byte> curCommentHeader(userCommentBlock.begin(), userCommentBlock.begin() + 4);
		uint32_t curCommentLength = readLEHeader(curCommentHeader);
		vector<byte> curComment(userCommentBlock.begin() + 4, userCommentBlock.begin() + 4 + curCommentLength);
		string curCommentStr(reinterpret_cast<char*>(curComment.data()), curComment.size());
		userCommentBlock.erase(userCommentBlock.begin(), userCommentBlock.begin() + 4 + curCommentLength);

		// cout << curCommentStr;

		// real fun begins
		unsigned int fieldLength = curCommentStr.find('=');
		// cout << fieldLength;

		string field = curCommentStr.substr(0, fieldLength);
		// from cppreference (I think it is rough)
		transform(field.begin(), field.end(), field.begin(), [](unsigned char c){return toupper(c);});
		string content = curCommentStr.substr(fieldLength + 1);

		_vorbiusComment._userComments.push_back({field, content});
	}
}

void music::setInit() {
	vector<byte> magicBytes(4);
	vector<byte> metaHeader(4);
	vector<byte> metaBlock;
	fstream file(path(), ios::in);
	
	// Skip file header (fLaC)
	file.seekg(4, ios::cur);

	// set metadata blocks
	do {
		file.read(reinterpret_cast<char*>(metaHeader.data()), 4);
		uint16_t blockSize = readBEHeader(metaHeader); 
		metaBlock.resize(blockSize);
		file.read(reinterpret_cast<char*>(metaBlock.data()), blockSize);

		switch (static_cast<int>(metaHeader[0])) {
			case block_t::STREAMINFO:
			setStreaminfo(metaBlock);
			break;
			
			case block_t::VORBIUS_COMMENT:
			_vorbiusComment._blockSize = blockSize;
			setVorbiusComment(metaBlock);
			break;

			case block_t::PADDING:
			case block_t::APPLICATION: 
			case block_t::SEEKTABLE:
			case block_t::CUESHEET:
			case block_t::PICTURE:
			case block_t::INVALID:
			break;
		}
	} while((static_cast<uint8_t>(metaHeader[0]) & 0xF0) != 0x80); 
	file.close();
}

void printMetadata() {
	vector<string> userCommentFields = {};
	for (music &i : files) {  
		i.setInit();		
		cout
			<< "====================================================================================================\n"
			<< setw(30) << "Filename" << ": " << i.filename() << '\n'
			<< setw(30) << "Path" << ": " << i.path() << '\n'
			<< setw(30) << "Sample Rate" << ": " << i.sampleRate() << '\n'
			<< setw(30) << "Channel" << ": " << i.channel() << '\n'
			<< setw(30) << "Bits per sample" << ": " << i.bps() << '\n'
			<< setw(30) << "Total Samples" << ": " << i.samples() << '\n'
			<< setw(30) << "Duration" << ": " << i.totalsecs() << " (" << i.minutes() << ':' << i.seconds() << ')' << '\n'
			<< "\nUSER COMMENT SECTION:"
			<< '\n';

			// Modular User Comment
			for (int j = 0; j < i.userCommentCount(); j++) {
				cout << setw(30) << i.userCommentField(j) << ": " << i.userComment(i.userCommentField(j)) << '\n'; 
			}
		cout << "====================================================================================================\n" << endl;
	}   
}