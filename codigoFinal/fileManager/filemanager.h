/*	JAIME VILLAR	*/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <dirent.h>

using namespace std;

class FileManager
{
    string dirPath;
    map<string, string *> files;

public:
    FileManager(string path);
    vector<string *> *listFiles();
    char *readFile(char *fileName, unsigned long int &dataLength);
    char *writeFile(char *fileName, char *data, unsigned long int dataLength);
    char *freeListedFiles(vector<string *> *fileList);

    ~FileManager();
};

#endif // FILEMANAGER_H
