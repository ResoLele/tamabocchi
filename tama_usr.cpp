#include <iostream>
using namespace std;

enum Action {ACTION_CHANGE_DIR,
             ACTION_END_TASK,
            }; 

void userAction (const int inputAction) {
    switch (inputAction) {
    case ACTION_CHANGE_DIR:
        dirAccess();
        break;
    default:
        break;
    }
}