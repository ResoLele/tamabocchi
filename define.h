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

void tagPrintStreamInfo();
void tagReadVorbis();
void tagPrintTags();

class file
{
    private:
    string _filename;
    fs::path _path;

    public:
    void setFilename(const string);
    void setPath(const fs::path);

    string filename();
    fs::path path();
};

struct duration
{
    double _totalsecs;
    uint16_t _hours;
    uint16_t _minutes;
    double _seconds;
};

struct streaminfo
{
    string _magic;
    duration _duration;
    uint16_t _length;
    uint32_t _sampleRate;
    uint16_t _channel;
    uint16_t _spb;
    uint64_t _samples;
};

class music : public file
{
    private:
    streaminfo _info;

    string _title;
    string _album;
    string _date;
    
    vector<string> _composer;
    vector<string> _type;
    vector<string> _genre;
    vector<string> _artist;
        
    public:
    
    // STREAMINFO
    void readStreaminfo(fstream&);

    string magic();
    uint16_t length();
    uint32_t sampleRate();
    uint16_t channel();
    uint16_t spb();
    uint64_t samples();

    double totalsecs();
    uint16_t minutes();
    double seconds();

    // VORBIUS_COMMENT
    void setTitle(const string);
    void setAlbum(const string);
    void setDate(const string);

    streaminfo info();
    string title();
    string album();
    string date();
};

extern vector<music> files;

string readFileHeader(fstream&);
streaminfo readStreaminfo(fstream&);

void readVendorHeader(fstream);
void readVorbiusHeader(fstream);

#endif