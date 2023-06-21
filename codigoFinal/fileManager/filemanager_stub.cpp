/*	JAIME VILLAR	*/

#include "filemanager_stub.h"
#include "utils.h"

filemanager_stub::filemanager_stub(char *_dirPath)
{
    dirPath = _dirPath;
    conn = initClient("192.168.49.2", 32443);
}

vector<string *> *filemanager_stub::listFichero()
{
    int bufferSize = 0;
    int size = 0;
    char *buffer = 0x00;
    std::string *strPtr = new string();
    vector<string *> *vec = new vector<string *>();

    int op = OP_LISTFICHERO;

    sendMSG(0, (void *)&op, sizeof(int));

    sendMSG(0, dirPath, strlen(dirPath)); // second message

    recvMSG(0, (void **)&buffer, &bufferSize); // receive amount of files
    size = ((int *)buffer)[0];

    for (int i = 0; i < size; i++)
    {
        recvMSG(0, (void **)&buffer, &bufferSize); // receive files
        strPtr = new std::string(buffer);
        vec->push_back(strPtr);
    }

    return vec;
}

char *filemanager_stub::writeFichero(char *file, char *content)
{
    int bufferSize = 0;
    char *buffer = 0x00;

    int op = OP_WRITEFICHERO;
    sendMSG(0, (void *)&op, sizeof(int));

    sendMSG(0, dirPath, strlen(dirPath)); // second message
    sendMSG(0, file, strlen(file));       // third message
    sendMSG(0, content, strlen(content)); // fourth message

    recvMSG(0, (void **)&buffer, &bufferSize); // char* conteniendo error o success

    return buffer;
}

char *filemanager_stub::leeFichero(char *nombre)
{
    int bufferSize = 0;
    char *buffer = 0x00;

    int op = OP_READFICHERO;

    std::string truePath = std::string(dirPath) + nombre;

    sendMSG(0, (void *)&op, sizeof(int));

    sendMSG(0, dirPath, strlen(dirPath));      // second message
    sendMSG(0, nombre, strlen(nombre));        // third message
    recvMSG(0, (void **)&buffer, &bufferSize); // char* conteniendo error o contenido del fichero

    return buffer;
}

char *filemanager_stub::freeListedFiles(vector<string *> *fileList)
{
    int bufferSize = 0;
    char *buffer = 0x00;
    int size = fileList->size();

    int op = OP_FREELISTED;

    sendMSG(0, (void *)&op, sizeof(int));

    sendMSG(0, (void *)&size, sizeof(int)); // mandamos la cantidad de archivos que va a recibir el servidor
    for (const string *strPtr : *fileList)
    {
        sendMSG(0, strPtr->c_str(), strPtr->length()); // devolvemos dicho resultado
    }

    sendMSG(0, dirPath, strlen(dirPath));
    recvMSG(0, (void **)&buffer, &bufferSize); // char* conteniendo error o success

    return buffer;
}

filemanager_stub::~filemanager_stub()
{
    int op = OP_FIN;

    sendMSG(0, &op, sizeof(int));

    closeConnection(0);
}
