#ifndef DEFINE_H
#define DEFINE_H

#include <string>
#include <vector>

using namespace std;

// tama_file.cpp
void fileInit();
void fileChangeDir();
void fileListDir();

// tama_tag.cpp
void tagPrintTitle();

class Music
{
    private:
        string title;
        string album;
        string date;
        string path;

        vector<string> artist;
        vector<string> composer;
        vector<string> type;
        vector<string> genre;
        
    public:
        
};

#endif // define.h