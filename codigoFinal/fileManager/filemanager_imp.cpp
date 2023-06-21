/*	JAIME VILLAR	*/

#include "filemanager_imp.h"
#include "filemanager_stub.h"
#include "utils.h"
#include <string.h>

filemanager_imp::filemanager_imp(int clientID)
{
    this->clientID = clientID; // waitForConnections(server_fd);
    // fm = new FileManager(); Esperamos a crear el filemanager una vez hayamos recibido el dirPath del cliente.
}

void filemanager_imp::recvOP()
{
    int op = -1;
    char *buff;
    int bufLen;
    recvMSG(clientID, (void **)&buff, &bufLen);
    op = ((int *)buff)[0];
    delete[] buff;

    switch (op)
    {
    case OP_LISTFICHERO:
    {
        recvMSG(clientID, (void **)&buff, &bufLen); // recibe dirPath para crear FileManager
        char *path = buff;

        std::string pathString(path);         // LOGGING PARA DEBUGEAR
        std::cout << pathString << std::endl; // LOGGING PARA DEBUGEAR

        FileManager fm = FileManager(path); // creamos FileManager

        vector<string *> *res = fm.listFiles(); // almacenamos el resultado de listFiles
                                                // Calculate the total length of the concatenated string
        int size = res->size();
        sendMSG(clientID, (void *)&size, sizeof(int)); // mandamos la cantidad de archivos que va a recibir el cliente

        for (const string *strPtr : *res)
        {
            sendMSG(clientID, strPtr->c_str(), strPtr->length()); // devolvemos dicho resultado
        }

        // fm.~FileManager();
        break;
    }
    case OP_WRITEFICHERO:
    {
        recvMSG(clientID, (void **)&buff, &bufLen); // recibe dirPath para crear FileManager
        char *path = buff;
        FileManager fm = FileManager(path);

        std::cout << "path:" << path << std::endl;
        recvMSG(clientID, (void **)&buff, &bufLen); // recibe nombre del fichero
        char *file = buff;
        std::cout << "file:" << file << std::endl;
        // delete[] buff;
        recvMSG(clientID, (void **)&buff, &bufLen); // recibe contenido que escribir en fichero
        char *content = buff;
        std::cout << "content:" << content << std::endl;
        int len = bufLen;
        // delete[] buff;

        // delete[] buff;

        char *res = fm.writeFile(file, content, len); // writeFile escribe el content en un file. Si no existe file, lo crea.
                                                      // devuelve un error o el success y lo guarda en data
        sendMSG(clientID, res, strlen(res));

        // fm.~FileManager();
        break;
    }
    case OP_READFICHERO:
    {
        recvMSG(clientID, (void **)&buff, &bufLen); // recibe dirPath para crear FileManager
        FileManager fm = FileManager(buff);

        recvMSG(clientID, (void **)&buff, &bufLen); // recibe nombre del fichero

        unsigned long int fileLen = 0;
        char *data = fm.readFile(buff, fileLen); // readFile devuelve el contenido del archivo
                                                 // devuelve un error o el contenido y lo guarda en data
        sendMSG(clientID, data, fileLen);

        // fm.~FileManager();
        break;
    }
    case OP_FREELISTED:
    {
        int size = 0;
        std::string *strPtr = new string();
        vector<string *> *vec = new vector<string *>();

        recvMSG(0, (void **)&buff, &bufLen); // receive amount of files
        size = ((int *)buff)[0];

        for (int i = 0; i < size; i++)
        {
            recvMSG(0, (void **)&buff, &bufLen); // receive files
            strPtr = new std::string(buff);
            vec->push_back(strPtr);
        }

        recvMSG(clientID, (void **)&buff, &bufLen); // recibe dirPath para crear FileManager
        char *path = buff;
        FileManager fm = FileManager(path);

        char *res = fm.freeListedFiles(vec);

        sendMSG(clientID, res, strlen(res));

        // fm.~FileManager();
        break;
    }
    case OP_FIN:
    {
        salir = true;
        break;
    }
    default:
    {
        printf("ERROR OP NO DEFINIDA: %d\n", op);
        break;
    }
    }
}

filemanager_imp::~filemanager_imp()
{
    closeConnection(clientID);
}
