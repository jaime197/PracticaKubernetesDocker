/*	JAIME VILLAR	*/

#ifndef OPERACIONES_IMP_H
#define OPERACIONES_IMP_H
#include "filemanager.h"

class filemanager_imp
{
    int clientID;
    FileManager *fm;

public:
    bool salir = false;
    filemanager_imp(int clientID);
    void recvOP();
    ~filemanager_imp();
};

#endif // OPERACIONES_IMP_H
