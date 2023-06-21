/*	JAIME VILLAR	*/

#include <iostream>
#include "filemanager.h"
#include "filemanager_stub.h"

/*int main(int argc,char** argv)
{
    FileManager *fm=new FileManager("./dirprueba/");
    vector<string*>* vfiles=fm->listFiles();
    cout<<"Lista de ficheros en el directorio de prueba:\n";
    for(unsigned int i=0;i<vfiles->size();++i)
    {
        cout<<"Fichero: "<<vfiles->at(i)->c_str()<<endl;
    }
    cout<<"Leyendo el primer fichero del directorio de prueba:\n";

    char* data=nullptr;
    unsigned long int fileLen=0;
    fm->readFile(&(*(vfiles->at(0)))[0],data,fileLen);
    cout<<"Escribiendo el primer fichero del directorio de prueba:\n";
    fm->writeFile(&(*(vfiles->at(0)))[0],data,fileLen);
    cout<<"Liberando lista de ficheros:\n";
    fm->freeListedFiles(vfiles);
    cout<<"Liberando datos de fichero leído:\n";

    delete[] data;
    return 0;
}*/

int main(int argc, char **argv)
{
    filemanager_stub *fm = new filemanager_stub("./pruebas"); // se recomienda usar la ruta entera desde la raiz
    vector<string *> *vfiles = fm->listFichero();
    cout << "Lista de ficheros en el directorio de prueba:\n";
    for (unsigned int i = 0; i < vfiles->size(); ++i)
    {
        cout << "Fichero: " << vfiles->at(i)->c_str() << endl;
    }
    cout << "Leyendo el primer fichero del directorio de prueba:\n";

    char *data = nullptr;
    char *res = nullptr;
    unsigned long int fileLen = 0;
    data = fm->leeFichero(&(*(vfiles->at(0)))[0]);
    cout << "Escribiendo el segundo fichero del directorio de prueba:\n";
    res = fm->writeFichero(&(*(vfiles->at(1)))[0], data);
    // cout << res << endl; // res = "Success" o "Error"
    cout << "Liberando lista de ficheros:\n";
    res = fm->freeListedFiles(vfiles);
    cout << res << endl;

    fm->~filemanager_stub();
    delete[] data;
    return 0;
}
