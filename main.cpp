#include <iostream>
#include <filesystem>
#include <string>
#include "tama_dir.cpp"
#include "tama_usr.cpp"

using namespace std;

// variable declaration
int userInputAction;

int main()
{
	while (true) {
		cout << "Enter Action: ";
		cin >> userInputAction;
		userAction(userInputAction);
	}		
}