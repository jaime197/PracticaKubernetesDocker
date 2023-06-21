/*	JAIME VILLAR	*/

#include "utils.h"
#include <vector>
#include <string>

using namespace std;

#define OP_FIN -1
#define OP_LISTFICHERO 0
#define OP_WRITEFICHERO 1
#define OP_READFICHERO 2
#define OP_FREELISTED 3

class filemanager_stub
{
    char *dirPath;
    connection_t conn;

public:
    filemanager_stub(char *dirPath);
    vector<string *> *listFichero();
    char *writeFichero(char *file, char *content);
    char *leeFichero(char *nombre);
    char *freeListedFiles(vector<string *> *fileList);
    ~filemanager_stub();
};