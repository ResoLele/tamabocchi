#include <iostream>
#include <filesystem>
#include <string>
#include "define.h"

using namespace std;

// variable declaration
int userInputAction;
string selectedPath;
string selectedSong;

#if defined _WIN32 || defined _WIN64
#define USER_OS "Windows"
#elif __APPLE__ 
#define USER_OS "macOS"
#elif __linux__
#define USER_OS "Linux"
#endif

void osClear(string OS) {
    if (OS == "Windows") {system("clr");}
    else {system("clear");}
    return;
}

// Action Enum
enum Action {   
                ACTION_END_TASK,
                ACTION_CHANGE_DIR,
                ACTION_LIST_DIR,
                ACTION_SELECT_FILE,
                ACTION_PRINT_TAGS,
            };

int main() {

    // Initialize
    fileInit(selectedPath, USER_OS);
    cout << USER_OS << " detected." << endl;

    do {

        cout << "Select Action:\n"
                "(1) Change Directory\n"
                "(2) List Directory\n"
                "(3) Select File\n"
                "(4) Print Tags\n"
                "(0) End Task"
        << endl;
    	cin >> userInputAction;

        switch (userInputAction) {
            case ACTION_CHANGE_DIR:
                osClear(USER_OS);
                fileChangeDir(selectedPath);
                break;
            case ACTION_LIST_DIR:
                osClear(USER_OS);
                fileListDir(selectedPath);
                break;
            case ACTION_SELECT_FILE:
                osClear(USER_OS);
                fileSelectFile(selectedPath, selectedSong);
                break;
            case ACTION_PRINT_TAGS:
                osClear(USER_OS);    
                tagPrintTags(selectedPath, selectedSong);
                break;
            case ACTION_END_TASK:
                osClear(USER_OS);
                cout << "Have a nice day!" << endl;
                break;
            default:
                cerr << "Invaild Action." << endl;
                break;
        }
        
    } while (userInputAction != ACTION_END_TASK);
    
    return 0;
}