#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <fstream>
// #include <bitset>
// #include <map>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

void osClear(string);

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

void printMetadata();
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
    block_t _blockType;
    uint16_t _blockSize;
};

// Note-to-self: uint32_t is exactly 4 bytes (Header) 

using userComment_t = pair<string, string>;

struct vorbiusComment : metadata
{
    uint32_t _vendorHeader; 
    string _vendor;

    vector<userComment_t> _userComments;
};


class music : public file
{
    private:
    vector<byte> _streaminfo;
    vorbiusComment _vorbiusComment;

    public:

    void setInit();
    void setStreaminfo(vector<byte>);
    void setVorbiusComment(vector<byte>);

    // STREAMINFO
    uint32_t sampleRate();
    uint16_t channel();
    uint16_t bps();
    uint64_t samples();

    double totalsecs();
    uint16_t minutes();
    double seconds();

    string userCommentField(unsigned int);
    unsigned int userCommentCount();
    string userComment(string);
    string title();
    string album();
    string date();
};

extern vector<music> files;

#endif // DEFINE_H