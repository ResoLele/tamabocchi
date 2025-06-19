#ifndef DEFINE_H
#define DEFINE_H

#include <string>
#include <vector>

using namespace std;

string get_OS();

void fileInit(string &selectedPath);
void fileChangeDir(string &selectedPath);
void fileListDir();
void fileSelectFile(string &selectedPath, string &selectedSong);

void tagPrintTags(string &selectedPath, string &selectedSong);

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