#include <iostream>
#include <filesystem>
#include <string>
#include "define.h"
#include "tama_file.cpp"
#include "tama_tag.cpp"

using namespace std;

// variable declaration
int userInputAction;

enum Action {   ACTION_CHANGE_DIR = 1,
                ACTION_LIST_DIR,
                ACTION_END_TASK,
            };

int main() {

    // Initialize
    fileInit();

    do {
        cout << "Enter Action: ";
    	cin >> userInputAction;

        switch (userInputAction) {
            case ACTION_CHANGE_DIR:
                fileChangeDir();
                break;
            case ACTION_LIST_DIR:
                fileListDir();
                break;
            case ACTION_END_TASK:
                cout << "Have a nice day!";
                break;
            default:
                cerr << "Invaild Action." << endl;
                break;
        }
    } while (userInputAction != ACTION_END_TASK);
    
    return 0;
}