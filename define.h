#ifndef DEFINE_H
#define DEFINE_H

#if defined(_WIN32) || defined(_WIN64)
#define OS "win"
#elif defined(__APPLE__) 
#define OS "mac"
#elif defined(__linux__)
#define OS "linux"
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>

#include "tama_file.h"
#include "tama_tag.h"

using namespace std;
namespace fs = std::filesystem;

const string USER_OS = OS;

void clearConsole();

#endif // DEFINE_H