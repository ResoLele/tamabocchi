#include <iostream>
#include <filesystem>
#include <string>
#include "define.h"

using namespace std;

// variable declaration
int userInputAction;
string selectedPath;
string selectedSong;
string USER_OS;

// Check OS
#ifdef _WIN32 
#define USER_OS "Windows"
#elif __APPLE__ 
#define USER_OS "macOS"
#elif __linux__
#define USER_OS "Linux"
#endif

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
    fileInit(selectedPath);
    cout << USER_OS << " detected." << endl;

    do {
        cout << "Select Action: ";
    	cin >> userInputAction;

        switch (userInputAction) {
            case ACTION_CHANGE_DIR:
                fileChangeDir(selectedPath);
                break;
            case ACTION_LIST_DIR:
                fileListDir();
                break;
            case ACTION_SELECT_FILE:
                fileSelectFile(selectedPath, selectedSong);
                break;
            case ACTION_PRINT_TAGS:
                tagPrintTags(selectedPath, selectedSong);
                break;
            case ACTION_END_TASK:
                cout << "Have a nice day!" << endl;
                break;
            default:
                cerr << "Invaild Action." << endl;
                break;
        }
    } while (userInputAction != ACTION_END_TASK);
    
    return 0;
}