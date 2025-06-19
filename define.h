#ifndef DEFINE_H
#define DEFINE_H

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
void tagAddTags();
void tagPrintTags();

class file
{
    private:
    string _filename;
    string _path;
    
    string _title;
    string _album;
    string _date;
    
    vector<string> composer;
    vector<string> type;
    vector<string> genre;
    vector<string> artist;
        
    public:
    void setFilename(const string);
    void setPath(const fs::path);
    
    void setTitle(const string);
    void setAlbum(const string);
    void setDate(const string);
    
    
    string filename();
    string title();
    string album();
    string date();
    string path();
};

extern vector<file> files;

#endif