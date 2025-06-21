#include "define.h"

// variable declaration
int userInputAction;
fs::path currentPath;
string selectedPath;
string selectedSong;
vector<music> files;

void osClear(string USER_OS = USER_OS) {
    if (USER_OS == "Windows") {system("clr");}
    else {system("clear");}
    return;
}

// Action Enum
enum Action {   
                ACTION_END_TASK,
                ACTION_CHANGE_DIR,
                ACTION_PRINT_FILES,
                ACTION_PRINT_TAGS,
            };

int main() {

    // Initialize Path
    cout << USER_OS << " detected." << endl;
    currentPath = dirChange();
    cout << "Current Path: " << currentPath << endl;
    if (!dirScan(currentPath)) {
        cout << "No FLAC in this directory! Please change directory." << endl;
    }

    do {
        cout << "Select Action:\n"
                "(1) Change Directory\n"
                "(2) Print all music file in this folder\n"
                "(3) Print all Tags\n"
                "(0) End Task"
        << endl;
    	cin >> userInputAction;

        switch (userInputAction) {
            case ACTION_CHANGE_DIR:
                currentPath = dirChange();
                break;
            case ACTION_PRINT_FILES:
                osClear();
                dirPrint(currentPath);
                break;
            case ACTION_PRINT_TAGS:
                osClear();
                tagPrintMetadata();
                break;
            case ACTION_END_TASK:
                osClear();
                cout << "Have a nice day!" << endl;
                break;
            default:
                cerr << "Invaild Action." << endl;
                break;
        }
        
    } while (userInputAction != ACTION_END_TASK);
    
    return 0;
}