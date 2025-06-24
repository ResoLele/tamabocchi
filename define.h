#ifndef DEFINE_H
#define DEFINE_H

#if defined(_WIN32) || defined(_WIN64)
#define OS "win"
#elif defined(__APPLE__) 
#define OS "mac"
#elif defined(__linux__)
#define OS "linux"
#endif // OS_CHECK

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <filesystem>

#include "tama_file.h"
#include "tama_tag.h"
#include "tama_user.h"

using namespace std;
namespace fs = std::filesystem;

void clearConsole();
Song& promptSelectSong(User&);

#endif // DEFINE_H