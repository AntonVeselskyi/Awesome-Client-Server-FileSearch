


#ifdef unix
#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h> //to use stat()
#include <netinet/in.h>
#include <unistd.h>	//to use close()
#include <dirent.h>
#include <errno.h> // to use errno
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1


void get_files_on_disk(const string &DATA_DIR, const string &to_find,bool &something_was_found,vector<string> &data);  //founds all files with needed file name

bool search_file(const string &to_find, vector<string> &data); //calls  get_files_on_disk() for each disk


#endif
