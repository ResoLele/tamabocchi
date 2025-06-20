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

const string USER_OS = OS;

// tama_file.cpp
fs::path dirChange();
bool dirScan(fs::path);
void dirPrint(fs::path);

// tama_tag.cpp
void tagReadHex();
void tagAddTags();
void tagPrintTags();

class file
{
    private:
    string _filename;
    string _path;
    
    uint32_t _sampleRate;
    uint8_t _channel;
    uint8_t _SPB;
    uint64_t _samples;

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
    
    void setSampleRate(const uint32_t);
    void setChannel(const uint16_t);
    void setSPB(const uint16_t);
    void setSamples(const uint64_t);

    void setTitle(const string);
    void setAlbum(const string);
    void setDate(const string);
    
    string filename();
    string path();

    uint32_t sampleRate();
    uint16_t channel();
    uint16_t SPB();
    uint64_t samples();

    string title();
    string album();
    string date();
};

extern vector<file> files;

#endif