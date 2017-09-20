#pragma once
#ifdef _WIN32

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;
void get_files_on_disk(const string &DATA_DIR, const string &to_find, bool &something_was_found, vector<string> &data);  //founds all files with needed file name

bool search_file(const string &to_find, vector<string> &data); //calls  get_files_on_disk() for each disk


#endif
