#include <iostream>
#include <string>
#include "define.h"

#include <taglib/fileref.h>
#include <taglib/tstring.h>
#include <taglib/tag.h>

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