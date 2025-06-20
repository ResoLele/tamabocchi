#include "define.h"

#include <taglib/fileref.h>
#include <taglib/tstring.h>
#include <taglib/tag.h>


void tagReadHex() {
    for (file &i : files) {  
        fstream file(i.path(), ios::in);
        
        vector<byte> header(4);
        vector<byte> meta_header(3);

        file.read(reinterpret_cast<char*>(header.data()), 4);
        
        for (int i = 0; i < 4; i++) {cout << static_cast<char>(header[i]);}
        cout << endl;

        file.seekg(1, ios::cur); // skip end check first
        // Read STREAMINFO ONLY I DON'T WANT TO THINK
        
        file.read(reinterpret_cast<char*>(meta_header.data()), 3);
        int length = (static_cast<int>(meta_header[0]) << 16) | (static_cast<int>(meta_header[1]) << 8) | (static_cast<int>(meta_header[2]));
        
        vector<byte> meta_block(length);
        file.read(reinterpret_cast<char*>(meta_block.data()), length);
        cout << setw(30) << "Metadata length: " << dec << length << '\n';

        uint32_t SAMPLE_RATE = 
            (static_cast<uint32_t>(meta_block[10]) << 12) | 
            (static_cast<uint32_t>(meta_block[11]) << 4) |
            (static_cast<uint32_t>(meta_block[12]) >> 4);

        uint8_t CHANNEL =
            ((static_cast<uint8_t>(meta_block[12]) & 0x0F) >> 1) + 1;

        uint8_t SPB = 
            (((static_cast<uint8_t>(meta_block[12]) & 0x01) << 4) |
            (static_cast<uint8_t>(meta_block[13]) >> 4)) + 1;

        uint64_t SAMPLES = (
            ((static_cast<uint64_t>(meta_block[13]) & 0x0F) << 32) |
            (static_cast<uint64_t>(meta_block[14]) << 24) |
            (static_cast<uint64_t>(meta_block[15]) << 16) |
            (static_cast<uint64_t>(meta_block[16]) << 8)  |
            (static_cast<uint64_t>(meta_block[17])));

        i.setSampleRate(SAMPLE_RATE);
        i.setChannel(CHANNEL);
        i.setSPB(SPB);
        i.setSamples(SAMPLES);

        double LENGTH_TOTAL = (static_cast<double>(SAMPLES) / static_cast<double>(SAMPLE_RATE));
        int LENGTH_MIN = static_cast<int>(LENGTH_TOTAL) / 60;
        double LENGTH_SEC = fmod(LENGTH_TOTAL, 60.0);

        cout 
            << setw(30) << "Sample Rate: " << dec << i.sampleRate() << '\n'
            << setw(30) << "Channel: " << dec << i.channel() << '\n'
            << setw(30) << "SPB: " << dec << i.SPB() << '\n'
            << setw(30) << "Total Samples: " << dec << i.samples() << " (" << LENGTH_MIN << ':' << LENGTH_SEC << ')' << '\n';

        cout << endl;

        file.close();
    }   
}

void tagAddTags() {
    for (file &i : files) {
        TagLib::FileRef scan(i.path().c_str());
        i.setTitle(scan.tag()->title().to8Bit(true));
        i.setAlbum(scan.tag()->album().to8Bit(true));
    }
}

void tagPrintTags() {
    for (file i : files) {
        cout << "Title:\t\t" << i.title() << endl;
        cout << "Album:\t\t" << i.album() << endl;
        cout << endl;
    }
    cout << endl;
}