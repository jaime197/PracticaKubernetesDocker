/*	JAIME VILLAR	*/

#include "multmatrix_stub.h"

multmatrix_stub::multmatrix_stub()
{
    conn = initClient("127.0.0.1", 32444);
}

matrix_t *multmatrix_stub::readMatrix(const char *fileName)
{
    int bufferSize = 0;
    int *buffer = 0;

    int op = OP_READMATRIX;

    sendMSG(0, (void *)&op, sizeof(int)); // send op

    sendMSG(0, fileName, strlen(fileName)); // send fileName

    recvMSG(0, (void **)&buffer, &bufferSize); // receive dimension
    int size = ((int *)buffer)[0];

    matrix_t *result = new matrix_t[1];
    result->rows = size;
    result->cols = size;
    result->data = new int[size * size];

    for (int i = 0; i < size * size; i++)
    {
        recvMSG(0, (void **)&buffer, &bufferSize); // receive numbers individually
        cout << *buffer << endl;
        result->data[i] = *buffer;
    }

    delete[] buffer;
    return result;
}

matrix_t *multmatrix_stub::multMatrices(matrix_t *m1, matrix_t *m2)
{
    int bufferSize = 0;
    int *buffer = 0;

    int op = OP_MULTMATRIX;

    sendMSG(0, (void *)&op, sizeof(int)); // send op

    int size = m1->cols;

    sendMSG(0, (void *)&size, sizeof(int)); // send dimension

    for (int i = 0; i < size * size; i++)
    {
        int num = *(m1->data + i);
        sendMSG(0, (void *)&num, sizeof(int)); // send mat1
    }
    for (int i = 0; i < size * size; i++)
    {
        int num = *(m2->data + i);
        sendMSG(0, (void *)&num, sizeof(int)); // send mat2
    }

    matrix_t *result = new matrix_t[1];
    result->rows = size;
    result->cols = size;
    result->data = new int[size * size];

    for (int i = 0; i < size * size; i++)
    {
        recvMSG(0, (void **)&buffer, &bufferSize); // receive numbers individually
        result->data[i] = *buffer;
    }

    delete[] buffer;
    return result;
}

void multmatrix_stub::writeMatrix(matrix_t *m, const char *fileName)
{
    int bufferSize = 0;
    char *buffer = 0x00;

    int op = OP_WRITEMATRIX;

    sendMSG(0, (void *)&op, sizeof(int));

    int size = m->cols;
    sendMSG(0, (void *)&size, sizeof(int)); // send dimension

    for (int i = 0; i < size * size; i++)
    {
        int num = *(m->data + i);
        sendMSG(0, (void *)&num, sizeof(int)); // send mat
    }

    sendMSG(0, fileName, strlen(fileName)); // send fileName
}

matrix_t *multmatrix_stub::createIdentity(int rows, int cols)
{
    int bufferSize = 0;
    int *buffer = 0;

    matrix_t *m = new matrix_t[1];
    m->rows = rows;
    m->cols = cols;
    m->data = new int[rows * cols];

    int op = OP_IDENTITY;

    sendMSG(0, (void *)&op, sizeof(int)); // send op

    sendMSG(0, (void *)&rows, sizeof(int)); // send rows
    sendMSG(0, (void *)&cols, sizeof(int)); // send cols

    for (int i = 0; i < rows * cols; i++)
    {
        recvMSG(0, (void **)&buffer, &bufferSize); // receive numbers individually
        m->data[i] = *buffer;
    }

    delete[] buffer;
    return m;
}

matrix_t *multmatrix_stub::createRandMatrix(int rows, int cols)
{
    int bufferSize = 0;
    int *buffer = 0;

    matrix_t *m = new matrix_t[1];
    m->rows = rows;
    m->cols = cols;
    m->data = new int[rows * cols];

    int op = OP_RAND;

    sendMSG(0, (void *)&op, sizeof(int)); // send op

    sendMSG(0, (void *)&rows, sizeof(int)); // send rows
    sendMSG(0, (void *)&cols, sizeof(int)); // send cols

    for (int i = 0; i < rows * cols; i++)
    {
        recvMSG(0, (void **)&buffer, &bufferSize); // receive numbers individually
        m->data[i] = *buffer;
    }

    delete[] buffer;
    return m;
}

multmatrix_stub::~multmatrix_stub()
{
    int op = OP_FIN;
    sendMSG(0, &op, sizeof(int));

    closeConnection(0);
}