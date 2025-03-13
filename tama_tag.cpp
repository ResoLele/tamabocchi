#include <iostream>
#include <string>
#include "define.h"

#include <taglib/fileref.h>
#include <taglib/tstring.h>

// Print tags of the selected file
void tagPrintTags(std::string &selectedPath, std::string &selectedSong) {
    TagLib::FileName song = selectedSong.c_str();

    TagLib::FileRef test(song);
    TagLib::String title = test.tag() -> title();
    TagLib::String artist = test.tag() -> artist();

    std::cout << "Title:\t\t" << title << endl;
    std::cout << "Artist:\t\t" << artist << endl;
}