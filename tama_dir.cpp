#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

string userInputPath;
	
void dirAccess () {
	cout << "Please enter the directory: ";
	while (true) {
		getline(cin, userInputPath);
		fs::directory_entry USER_PATH{userInputPath};
		if (USER_PATH.exists()) {
			cout << "Loaded successfully! Directory existed." << endl;
			break;
		} else {
			cout << "Failed! Please input a valid directory: " << endl;
		} 
	}
}