/*	JAIME VILLAR	*/

#include "utils.h"
#include <map>
#include <thread>
#include <mutex>
#include <stdio.h>

std::map<unsigned int, connection_t> clientList;
unsigned int contador = 0;
bool salir = false;
std::thread *waitForConnectionsThread;
int lastClientSize = 0;
std::list<unsigned int> waitingClients;

int initServer(int port)
{
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("Error creating socket\n");
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    int option = 1;
    setsockopt(sock_fd, SOL_SOCKET,
               (SO_REUSEPORT | SO_REUSEADDR),
               &option, sizeof(option));

    if (bind(sock_fd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        printf("ERROR on binding");
    listen(sock_fd, 5);

    waitForConnectionsThread = new std::thread(waitForConnectionsAsync, sock_fd);
    return sock_fd;
}

connection_t initClient(char *host, int port)
{
    int sock_out = 0;
    struct sockaddr_in serv_addr;
    connection_t connection;
    if ((sock_out = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        connection.socket = -1;
        connection.alive = false;
        return connection;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        connection.socket = -1;
        connection.alive = false;
        return connection;
    }

    if (connect(sock_out, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        connection.socket = -1;
        connection.alive = false;
        return connection;
    }

    unsigned int localID = -1;

    read(sock_out, &localID, sizeof(unsigned int));

    connection.id = localID;
    connection.socket = sock_out;
    connection.buffer = new std::list<msg_t *>();
    clientList[0] = connection;
    return connection;
}

void waitForConnectionsAsync(int server_fd)
{
    while (!salir)
    {
        int newSocket = waitForConnections(server_fd);
    }
}

int waitForConnections(int sock_fd)
{
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    int newsock_fd = accept(sock_fd,
                            (struct sockaddr *)&cli_addr,
                            &clilen);

    connection_t client;
    client.id = contador;
    contador++;
    client.alive = true;
    client.socket = newsock_fd;
    client.buffer = new std::list<msg_t *>();
    clientList[client.id] = client;

    waitingClients.push_back(client.id);

    write(newsock_fd, &client.id, sizeof(unsigned int));

    return newsock_fd;
}

void closeConnection(int clientID)
{
    connection_t connection = clientList[clientID];
    close(connection.socket);
    connection.alive = false;

    if (checkPendingMessages(clientID))
    {
        printf("ERROR: Mensajes sin recibir de %d\n", connection.id);
        for (std::list<msg_t *>::iterator t = connection.buffer->begin();
             t != connection.buffer->end(); t++)
        {
            msg_t *msg = *t;
            delete[] msg->data;
            delete[] msg;
        }
        delete connection.buffer;
    }
    clientList.erase(clientID);
}

/** funciones asíncronas **/

void recvMSGAsync(connection_t connection)
{

    while (connection.alive)
    {
        msg_t *msg = new msg_t[1];
        recvMSG(connection.socket, (void **)&(msg->data), &(msg->size));
        connection.buffer->push_back(msg);
    }
}

bool checkPendingMessages(int clientID)
{
    return clientList[clientID].buffer->size() > 0;
}

void getMSG(int clientID, void **data, int *dataLen)
{
    if (!checkPendingMessages(clientID))
    {
        *data = 0x00;
        *dataLen = 0x00;
    }
    else
    {
        msg_t *msg = clientList[clientID].buffer->front();
        clientList[clientID].buffer->pop_front();
        *data = msg->data;
        *dataLen = msg->size;

        delete[] msg;
    }
}

/** precondiciones, variables, etc...**/

void sendMSG(int clientID, const void *data, int dataLen)
{
    connection_t connection = clientList[clientID];

    int socket = connection.socket;

    char msg = 0;
    int bufferRecv = 0;
    // send sync
    msg = SYNC;
    write(socket, &msg, sizeof(char));
    // receive sync ack
    read(socket, &msg, sizeof(char));
    if (msg != SYNC_ACK)
    {
        printf("ERROR: sendMSG -- line : %d error fallo conexion\n", __LINE__);
        exit(0); // return error code
    }
    // send ack
    msg = ACK;
    write(socket, &msg, sizeof(char));

    // enviar tamaño buffer
    write(socket, &dataLen, sizeof(int));
    // recv ack
    read(socket, &msg, sizeof(char));
    if (msg != ACK)
    {
        printf("ERROR: sendMSG -- line : %d error fallo conexion\n", __LINE__);
        exit(0); // return error code
    }

    // enviar buffer

    write(socket, data, dataLen);
    // recv buffersize
    read(socket, &bufferRecv, sizeof(int));
    if (dataLen != bufferRecv)
    {
        printf("ERROR: sendMSG -- line : %d error fallo conexion\n", __LINE__);
        exit(0); // return error code
    }

    // send end
    msg = END;
    write(socket, &msg, sizeof(char));
}

void recvMSG(int clientID, void **data, int *dataLen)
{

    connection_t connection = clientList[clientID];

    int socket = connection.socket;

    char msg = 0;
    // recv sync
    read(socket, &msg, sizeof(char));
    if (msg != SYNC)
    {
        printf("ERROR: recvMSG -- line : %d error fallo conexion\n", __LINE__);
        exit(0); // return error code
    }
    // send sync ack

    msg = SYNC_ACK;
    write(socket, &msg, sizeof(char));

    // recv ack
    read(socket, &msg, sizeof(char));
    if (msg != ACK)
    {
        printf("ERROR: recvMSG -- line : %d error fallo conexion\n", __LINE__);
        exit(0); // return error code
    }

    int bufferSize = 0;
    read(socket, &bufferSize, sizeof(int));
    DEBUG_MSG("DatosLeidos : %d\n", bufferSize);

    msg = ACK;
    write(socket, &msg, sizeof(char));

    char *buffer = new char[bufferSize];
    bufferSize = read(socket, buffer, bufferSize);

    write(socket, &bufferSize, sizeof(int));

    read(socket, &msg, sizeof(char));
    if (msg != END)
    {
        printf("ERROR: recvMSG -- line : %d error fallo conexion\n", __LINE__);
        exit(0); // return error code
    }
    *dataLen = bufferSize;
    *data = buffer;
}

bool checkClient()
{
    return waitingClients.size() > 0;
}

int getNumClients()
{
    return clientList.size();
}

int getClientID(int numClient)
{
    return clientList[numClient].id;
}

int getLastClientID()
{
    int id = waitingClients.back();
    waitingClients.pop_back();
    return id;
}
