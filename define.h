#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

#if defined(_WIN32) || defined(_WIN64)
#define OS "win"
#elif defined(__APPLE__) 
#define OS "mac"
#elif defined(__linux__)
#define OS "linux"
#endif

const std::string USER_OS = OS;

// tama_file.cpp
fs::path dirChange();
bool dirScan(fs::path);
void dirPrint(fs::path);

// tama_tag.cpp
uint32_t readBEHeader(vector<byte>);
uint32_t readLEHeader(vector<byte>);

void tagPrintMetadata();
void tagReadVorbis();
void tagPrintTags();

class file
{
    private:
    string _filename;
    string _magic;
    fs::path _path;

    public:
    void setFilename(const string);
    void setPath(const fs::path);

    string filename();
    string magic();
    fs::path path();
};

struct duration
{
    double _totalsecs;
    uint16_t _hours;
    uint16_t _minutes;
    double _seconds;
};

enum block_t {
    STREAMINFO = 0x00,
    PADDING = 0x01,
    APPLICATION = 0x02,
    SEEKTABLE = 0x03,
    VORBIUS_COMMENT = 0x04,
    CUESHEET = 0x05,
    PICTURE = 0x06,
    INVALID = 0x07,
};

struct metadata
{
    block_t _block_type;
    uint16_t _blockSize;
};

struct streaminfo : metadata
{
    duration _duration;
    uint16_t _length;
    uint32_t _sampleRate;
    uint16_t _channel;
    uint16_t _spb;
    uint64_t _samples;
};

struct vorbiusComment : metadata
{
    string _vendor;

    uint8_t _size;
    uint8_t _tracknumber;
    uint8_t _totaltrack;
    uint8_t _discnumber;
    uint8_t _totaldiscs;
    
    string _title;
    string _album;
    string _albumartist;
    string _date;

    string _catalog;
    string _label;

    vector<string> _artist;
    vector<string> _composer;
    vector<string> _type;
    vector<string> _genre;
};

class music : public file
{
    private:
    streaminfo _streaminfo;
    vorbiusComment _vorbiusComment;
    
    public:
    byte VORBIUS_TYPE = byte{0x04};

    void setInit();
    void setStreaminfo(vector<byte>);
    void setVorbiusComment(vector<byte>);

    string magic();
    uint16_t length();
    uint32_t sampleRate();
    uint16_t channel();
    uint16_t spb();
    uint64_t samples();

    double totalsecs();
    uint16_t minutes();
    double seconds();

    streaminfo info();
    vorbiusComment comment();
    string title();
    string album();
    string date();
};

extern vector<music> files;

#endif