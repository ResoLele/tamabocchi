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
string music::magic() {return _info._magic;}
uint16_t music::length() {return _info._length;}
uint32_t music::sampleRate() {return _info._sampleRate;}
uint16_t music::channel() {return _info._channel;}
uint16_t music::spb() {return _info._spb;}
uint64_t music::samples() {return _info._samples;}

double music::totalsecs() {return _info._duration._totalsecs;}
uint16_t music::minutes() {return _info._duration._minutes;}
double music::seconds() {return _info._duration._seconds;}

// Setter
// Read from file & Set streaminfo
void music::readStreaminfo(fstream &file) {
    vector<byte> magicBytes(4);
    vector<byte> head(4);
    vector<byte> block;
    
    // Read magic (File Header)
    file.read(reinterpret_cast<char*>(magicBytes.data()), 4);
    _info._magic = string(reinterpret_cast<const char*>(magicBytes.data()), magicBytes.size());

    // Read Header & Set streaminfo.length
    file.read(reinterpret_cast<char*>(head.data()), 4);
    _info._length = readBEHeader(head);
    block.resize(_info._length);
    file.read(reinterpret_cast<char*>(block.data()), _info._length);

    // Set streaminfo.sampleRate
    _info._sampleRate = (
    (static_cast<uint32_t>(block[10]) << 12) | 
    (static_cast<uint32_t>(block[11]) << 4) |
    (static_cast<uint32_t>(block[12]) >> 4));

    // Set streaminfo.channel
    _info._channel = ((static_cast<uint16_t>(block[12]) & 0x0F) >> 1) + 1;

    // Set SPB
    _info._spb = (
    (((static_cast<uint16_t>(block[12]) & 0x01) << 4) | (static_cast<uint16_t>(block[13]) >> 4)) + 1);

    // Set Samples
    _info._samples = (
    ((static_cast<uint64_t>(block[13]) & 0x0F) << 32) |
    (static_cast<uint64_t>(block[14]) << 24) |
    (static_cast<uint64_t>(block[15]) << 16) |
    (static_cast<uint64_t>(block[16]) << 8)  |
    (static_cast<uint64_t>(block[17])));

    // Set duration
    _info._duration._totalsecs = static_cast<double>(_info._samples) / static_cast<double>(_info._sampleRate); 
    _info._duration._minutes = static_cast<int>(_info._duration._totalsecs / 60); 
    _info._duration._seconds = fmod(_info._duration._seconds, 60.0);
}

void tagPrintStreamInfo() {
    for (music &i : files) {  
        fstream file(i.path(), ios::in);
    
        i.readStreaminfo(file);
        
        cout 
            << setw(30) << "filename: " << i.filename() << '\n'
            << setw(30) << "magic: " << i.magic() << '\n' 
            << setw(30) << "Sample Rate: " << dec << i.sampleRate() << '\n'
            << setw(30) << "Channel: " << dec << i.channel() << '\n'
            << setw(30) << "SPB: " << dec << i.spb() << '\n'
            << setw(30) << "Total Samples: " << dec << i.samples() << '\n'
            << setw(30) << "Duration: " << dec << i.totalsecs() << " (" << i.minutes() << ':' << i.seconds() << ')' << '\n';

        cout << endl;

        file.close();
    }   
}

void tagReadVorbis() {
    for (music i : files) { 

        fstream file(i.path(), ios::in);
        
        byte VORBIUS_TYPE = byte{0x04};
        uint32_t vorbiusLength;
        vector<byte> vorbiusHeader(4);

        file.seekg(42, ios::beg);

        file.read(reinterpret_cast<char*>(vorbiusHeader.data()), 4);
        
        for (int i = 0; i < 4; i++) {
            cout << hex << static_cast<int>(vorbiusHeader[i]);
        }

        cout << dec;

        if (vorbiusHeader[0] == VORBIUS_TYPE) {
            cout << "Vorbius Type (04) Detected!" << endl; 
            vorbiusLength = readBEHeader(vorbiusHeader);
        }
        
        cout << "Vorbius comment Length: " << vorbiusLength << '\n';
        
        // Vendor String

        vector<byte> byteLength(4);
        file.read(reinterpret_cast<char*>(byteLength.data()), 4);
        uint16_t vendorLength = readLEHeader(byteLength);

        vector<byte> vendor(vendorLength);
        file.read(reinterpret_cast<char*>(vendor.data()), vendorLength);
        for (int i = 0; i < vendorLength; i++) {
            cout << static_cast<char>(vendor[i]);
        }
        cout << endl;
        
        vector<byte> byteCommentCount(4);
        file.read(reinterpret_cast<char*>(byteCommentCount.data()), 4);
        uint32_t commentCount = readLEHeader(byteCommentCount);
        
        cout << "Number of comments: " << commentCount << '\n';    
            // Comments
        for (int i = 0; i < commentCount; i++) {
            vector<byte> byteCommentSize(4);
            file.read(reinterpret_cast<char*>(byteCommentSize.data()), 4);
            uint32_t commentSize = readLEHeader(byteCommentSize);
            vector<byte> commentText(commentSize);
            file.read(reinterpret_cast<char*>(commentText.data()), commentSize);
            
            for (int j = 0; j < commentSize; j++) {
                cout << static_cast<char>(commentText[j]);
            }
            cout << '\n';
        }
        
        cout << endl;
        
        file.close();
    }
}

void tagPrintTags() {
    for (music i : files) {
        cout << "Title:\t\t" << i.title() << endl;
        cout << "Album:\t\t" << i.album() << endl;
        cout << endl;
    }
    cout << endl;
}