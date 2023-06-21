/*	JAIME VILLAR	*/

#include "filemanager.h"
/**
 * @brief FileManager::FileManager Constructor de la clase FileManager. Recibe por parámetros el directorio
 * que usará esta clase para indexar, almacenar y leer ficheros. Se aconseja usar una ruta completa al directorio,
 * desde la raiz del sistema de ficheros.
 *
 * @param path Ruta al directorio que se desa usar
 */

FileManager::FileManager(string path)
{
    this->dirPath = path;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != nullptr)
        {
            if (ent->d_type == DT_REG) // Store only regular files
            {
                string *f = new string(ent->d_name);
                this->files[*f] = f;
            }
        }
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        string *f = new string("ERROR: No existe el fichero o directorio");
        this->files[*f] = f;
        std::cout << "ERROR: No existe el fichero o directorio\n";
    }
}

/**
 * @brief FileManager::listFiles Sirve para acceder a la lista de ficheros almacenados en la ruta
 * que se usó en el contructor de la clase
 * @return Una copia de la lista de ficheros almacenada en "files". Esta copia hay que liberarla
 * después de haber sido usada. Para ello se ofrece la función "freeListedFiles"
 */
vector<string *> *FileManager::listFiles()
{
    vector<string *> *flist = new vector<string *>();
    for (map<string, string *>::iterator i = files.begin(); i != files.end(); ++i)
    {
        string fileName = i->first;
        flist->push_back(new string(fileName));
        cout << "File name: " << fileName << endl;
    }
    return flist;
}
/**
 * @brief FileManager::freeListedFiles Función de apoyo, libera una lista de ficheros devuelta por la función "listFiles"
 * @param fileList Puntero a una lista de strings devuelta por la función "listFiles"
 */
char *FileManager::freeListedFiles(vector<string *> *fileList)
{
    if (fileList == nullptr) // error handling
    {
        return "Error: fileList is null";
    }

    for (vector<string *>::iterator i = fileList->begin(); i != fileList->end(); ++i)
    {
        delete *i;
    }
    delete fileList;
    return "Success";
}
/**
 * @brief FileManager::readFile Dado el nombre de un fichero almacenado en el directorio que se usó en el contructor,
 * se inicializará la variable "data" con un puntero al contenido del fichero, y la variable dataLength con el
 * tamaño del mismo en bytes.
 *
 * @param fileName Nombre del fichero a leer
 * @param data Datos del fichero. Deberá de liberarse después de ser usado
 * @param dataLength Longitud del fichero en bytes
 */

char *FileManager::readFile(char *fileName, unsigned long int &dataLength)
{
    string path = this->dirPath + "/" + string(fileName);
    FILE *f = fopen(path.c_str(), "r");
    char *data = 0x00;
    // std::cout << fileName << std::endl;
    // std::cout << path << std::endl;
    if (!f) // handle error
    {
        char *res = "Error: Unable to open file for reading";
        return res;
    }

    fseek(f, 0L, SEEK_END);
    dataLength = ftell(f);
    fseek(f, 0L, SEEK_SET);
    data = new char[dataLength];

    fread(data, dataLength, 1, f);
    fclose(f);

    return data;
}

/**
 * @brief FileManager::readFile Dado un nuevo nombre de un fichero que se quiere almacenar  en el directorio que se usó en el contructor,
 * se escribirá el contenido del array de datos almacenado en "data", siendo dataLength el
 * tamaño del mismo en bytes. Sobreescribirá el fichero que hubiera en el directorio si tiene el mismo nombre.
 *
 * @param fileName Nombre del fichero a escribir
 * @param data Datos del fichero. Deberá de liberarse después de ser usado
 * @param dataLength Longitud del fichero en bytes
 */

char *FileManager::writeFile(char *fileName, char *data, unsigned long dataLength)
{
    string path = this->dirPath + "/" + string(fileName);
    FILE *f = fopen(path.c_str(), "w");

    if (!f) // handle error
    {
        char *res = "Error: Unable to open file for writing";
        return res;
    }

    fwrite(data, dataLength, 1, f);
    fclose(f);
    // añadir a la lista el nuevo fichero, si no existe ya
    if (files.find(string(fileName)) == files.end())
        files[string(fileName)] = new string(fileName);

    return "Success";
}

FileManager::~FileManager()
{
    // Free the memory allocated for each file in the map
    // for (auto &file : files)
    //{
    //    delete file.second;
    //}

    // Clear the map
    // files.clear();
}