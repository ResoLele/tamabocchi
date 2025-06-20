#include "define.h"

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

void streaminfo::setLength(const vector<byte> head) {
    _length = readBEHeader(head);
};
void streaminfo::setSampleRate(const vector<byte> block) {
    _sampleRate = (
    (static_cast<uint32_t>(block[10]) << 12) | 
    (static_cast<uint32_t>(block[11]) << 4) |
    (static_cast<uint32_t>(block[12]) >> 4));
};
void streaminfo::setChannel(const vector<byte> block) {
    _channel = ((static_cast<uint16_t>(block[12]) & 0x0F) >> 1) + 1;
};
void streaminfo::setSpb(const vector<byte> block) {
    _spb = (
    (((static_cast<uint16_t>(block[12]) & 0x01) << 4) | (static_cast<uint16_t>(block[13]) >> 4)) + 1);
};
void streaminfo::setSamples(const vector<byte> block) {
    _samples = (
    ((static_cast<uint64_t>(block[13]) & 0x0F) << 32) |
    (static_cast<uint64_t>(block[14]) << 24) |
    (static_cast<uint64_t>(block[15]) << 16) |
    (static_cast<uint64_t>(block[16]) << 8)  |
    (static_cast<uint64_t>(block[17])));
};
void streaminfo::setDuration() {
    _duration = (static_cast<double>(_samples) / static_cast<double>(_sampleRate));
}

uint16_t streaminfo::length() {return _length;}
uint32_t streaminfo::sampleRate() {return _sampleRate;}
uint16_t streaminfo::channel() {return _channel;}
uint16_t streaminfo::spb() {return _spb;}
uint64_t streaminfo::samples() {return _samples;}

double streaminfo::duration() {return _duration;}
int streaminfo::minute() {return static_cast<int>(_duration / 60);}
double streaminfo::second() {return fmod(_duration, 60.0);}

void music::setInfo(streaminfo info) {_info = info;}
streaminfo music::info() {return _info;}

string readFileHeader(fstream &file) {
    vector<byte> bytes(4);
    string str;

    file.read(reinterpret_cast<char*>(bytes.data()), 4);
    for (byte b : bytes) {str += static_cast<char>(b);}
    
    return str;
}

streaminfo readStreaminfo(fstream &file) {
    vector<byte> head(4);
    vector<byte> block;
    streaminfo info;

    file.read(reinterpret_cast<char*>(head.data()), 4);
    info.setLength(head);
    block.resize(info.length());
    file.read(reinterpret_cast<char*>(block.data()), info.length());

    info.setSampleRate(block);
    info.setChannel(block);
    info.setSpb(block);
    info.setSamples(block);
    info.setDuration();

    return info;
}

void tagPrintStreamInfo() {
    for (music &i : files) {  
        fstream file(i.path(), ios::in);
        
        string magic = readFileHeader(file);
        streaminfo info = readStreaminfo(file);
        
        i.setInfo(info);

        cout 
            << setw(30) << "filename: " << i.filename() << '\n'
            << setw(30) << "magic: " << magic << '\n' 
            << setw(30) << "Sample Rate: " << dec << i.info().sampleRate() << '\n'
            << setw(30) << "Channel: " << dec << i.info().channel() << '\n'
            << setw(30) << "SPB: " << dec << i.info().spb() << '\n'
            << setw(30) << "Total Samples: " << dec << i.info().samples() << '\n'
            << setw(30) << "Duration: " << dec << i.info().duration() << " (" << i.info().minute() << ':' << i.info().second() << ')' << '\n';

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