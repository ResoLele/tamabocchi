#include "define.h"

// helper functions
uint32_t readLEHeader(vector<byte> bytes) {
    return 
    static_cast<uint32_t>(bytes[0]) |
    static_cast<uint32_t>(bytes[1]) << 8 |
    static_cast<uint32_t>(bytes[2]) << 16 |
    static_cast<uint32_t>(bytes[3]) << 24;
}

uint32_t readBEHeader(vector<byte> bytes) {
    return
    static_cast<uint32_t>(bytes[1]) << 16 |
    static_cast<uint32_t>(bytes[2]) << 8 |
    static_cast<uint32_t>(bytes[3]);
}

// getter
string file::magic() {return _magic;}
uint16_t music::length() {return _streaminfo._length;}
uint32_t music::sampleRate() {return _streaminfo._sampleRate;}
uint16_t music::channel() {return _streaminfo._channel;}
uint16_t music::spb() {return _streaminfo._spb;}
uint64_t music::samples() {return _streaminfo._samples;}

double music::totalsecs() {return _streaminfo._duration._totalsecs;}
uint16_t music::minutes() {return _streaminfo._duration._minutes;}
double music::seconds() {return _streaminfo._duration._seconds;}

// Setter
// Read from file & Set all
void music::setStreaminfo(vector<byte> block) {

    // Set streaminfo.sampleRate
    _streaminfo._sampleRate = (
    (static_cast<uint32_t>(block[10]) << 12) | 
    (static_cast<uint32_t>(block[11]) << 4) |
    (static_cast<uint32_t>(block[12]) >> 4));

    // Set streaminfo.channel
    _streaminfo._channel = ((static_cast<uint16_t>(block[12]) & 0x0F) >> 1) + 1;

    // Set SPB
    _streaminfo._spb = (
    (((static_cast<uint16_t>(block[12]) & 0x01) << 4) | (static_cast<uint16_t>(block[13]) >> 4)) + 1);

    // Set Samples
    _streaminfo._samples = (
    ((static_cast<uint64_t>(block[13]) & 0x0F) << 32) |
    (static_cast<uint64_t>(block[14]) << 24) |
    (static_cast<uint64_t>(block[15]) << 16) |
    (static_cast<uint64_t>(block[16]) << 8)  |
    (static_cast<uint64_t>(block[17])));

    // Set duration
    _streaminfo._duration._totalsecs = static_cast<double>(_streaminfo._samples) / static_cast<double>(_streaminfo._sampleRate); 
    _streaminfo._duration._minutes = static_cast<int>(_streaminfo._duration._totalsecs / 60); 
    _streaminfo._duration._seconds = fmod(_streaminfo._duration._totalsecs, 60.0);
}

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

    // for each block add to idk just print now idfc
    for (int i = 0; i < userCommentCount; i++) {
        vector<byte> curCommentHeader(userCommentBlock.begin(), userCommentBlock.begin() + 4);
        uint32_t curCommentLength = readLEHeader(curCommentHeader);
        vector<byte> curComment(userCommentBlock.begin() + 4, userCommentBlock.begin() + 4 + curCommentLength);
        string curCommentStr(reinterpret_cast<char*>(curComment.data()), curComment.size());
        userCommentBlock.erase(userCommentBlock.begin(), userCommentBlock.begin() + 4 + curCommentLength);

        cout << curCommentStr << '\n';
    }
}

void music::setInit() {
    vector<byte> magicBytes(4);
    vector<byte> metaHeader(4);
    vector<byte> metaBlock;
    fstream file(path(), ios::in);
    
    // Read magic (File Header) FUCK YOU NO MAGIC FOR NOW
    // file.read(reinterpret_cast<char*>(magicBytes.data()), 4);
    // magic() = string(reinterpret_cast<const char*>(magicBytes.data()), magicBytes.size()); 
    file.seekg(4, ios::cur);

    // set metadata block until reaches last
    do {
        file.read(reinterpret_cast<char*>(metaHeader.data()), 4);
        uint16_t blockSize = readBEHeader(metaHeader); 
        metaBlock.resize(blockSize);
        file.read(reinterpret_cast<char*>(metaBlock.data()), blockSize);

        switch (static_cast<int>(metaHeader[0])) {
            case block_t::STREAMINFO:
            _streaminfo._blockSize = blockSize;
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

void tagPrintMetadata() {
    for (music &i : files) {  
        i.setInit();
        
        cout 
            << setw(30) << "filename: " << i.filename() << '\n'
            // << setw(30) << "magic: " << i.magic() << '\n' 
            << setw(30) << "Sample Rate: " << dec << i.sampleRate() << '\n'
            << setw(30) << "Channel: " << dec << i.channel() << '\n'
            << setw(30) << "SPB: " << dec << i.spb() << '\n'
            << setw(30) << "Total Samples: " << dec << i.samples() << '\n'
            << setw(30) << "Duration: " << dec << i.totalsecs() << " (" << i.minutes() << ':' << i.seconds() << ')' << '\n';

        cout << endl;
    }   
}