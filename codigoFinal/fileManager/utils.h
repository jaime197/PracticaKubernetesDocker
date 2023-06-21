/*	JAIME VILLAR	*/

#ifndef _UTILS_H_
#define _UTILS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <list>
#include <map>
#define ERROR -1
#define SYNC 0
#define SYNC_ACK 1
#define ACK 2
#define END 3

#define DEBUG

#ifdef DEBUG

#define DEBUG_MSG(...) printf(__VA_ARGS__);
#endif

typedef struct msg_t
{
    int size;
    char *data;
} msg_t;

typedef struct connection_t
{
    unsigned int id;
    int socket;
    std::list<msg_t *> *buffer;
    bool alive;
} connection_t;

int initServer(int port);
connection_t initClient(char *host, int port);
int waitForConnections(int sock_fd);
void sendMSG(int clientID, const void *data, int dataLen);
void recvMSG(int clientID, void **data, int *dataLen);
void closeConnection(int clientID);
void getMSG(int clientID, void **data, int *dataLen);

bool checkPendingMessages(int clientID);
void recvMSGAsync(connection_t connection);
void waitForConnectionsAsync(int server_fd);

bool checkClient();
int getNumClients();
int getClientID(int numClient);
int getLastClientID();

#endif
