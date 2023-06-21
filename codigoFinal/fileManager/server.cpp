/*	JAIME VILLAR	*/

#include "utils.h"
#include <iostream>
#include <string>
#include "filemanager_imp.h"
#include <thread>
#include <list>

std::list<std::thread *> threadList;
void clientManager(int clientID)
{
    filemanager_imp *fm = new filemanager_imp(clientID);

    while (!fm->salir)
    {
        fm->recvOP();
    }
    delete fm;
}

int main(int argc, char **argv)
{
    int server_fd = initServer(32443);
    int lastClientID = -1;
    while (1)
    {
        while (!checkClient())
        {
            usleep(1000);
        }
        int clientID = getLastClientID();

        threadList.push_back(new std::thread(clientManager, clientID));
    }

    closeConnection(server_fd);
    return 0;
}
