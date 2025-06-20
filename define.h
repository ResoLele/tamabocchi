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

class streaminfo
{
    private:
    uint16_t _length;
    uint32_t _sampleRate;
    uint16_t _channel;
    uint16_t _spb;
    uint64_t _samples;
    double _duration; 

    public:
    void setLength(const vector<byte>);
    void setSampleRate(const vector<byte>);
    void setChannel(const vector<byte>);
    void setSpb(const vector<byte>);
    void setSamples(const vector<byte>);
    void setDuration();

    uint16_t length();
    uint32_t sampleRate();
    uint16_t channel();
    uint16_t spb();
    uint64_t samples();

    double duration();
    int minute();
    double second();
};

class music
{
    private:
    string _filename;
    string _path;
    
    streaminfo _info;

    string _title;
    string _album;
    string _date;
    
    vector<string> _composer;
    vector<string> _type;
    vector<string> _genre;
    vector<string> _artist;
        
    public:
    void setFilename(const string);
    void setPath(const fs::path);
    
    void setInfo(const streaminfo);

    void setTitle(const string);
    void setAlbum(const string);
    void setDate(const string);
    
    string filename();
    string path();

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