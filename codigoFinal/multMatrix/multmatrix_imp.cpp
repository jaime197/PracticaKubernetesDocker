/*	JAIME VILLAR	*/

#include "multmatrix_imp.h"
#include "multmatrix_stub.h"
#include "utils.h"

void printMatrix(matrix_t *m)
{
    for (int i = 0; i < m->rows; ++i)
    {
        for (int j = 0; j < m->cols; ++j)
        {
            int index = i * m->cols + j;
            std::cout << m->data[index] << " ";
        }
        std::cout << std::endl;
    }
}

multmatrix_imp::multmatrix_imp(int clientID)
{
    this->clientID = clientID; // waitForConnections(server_fd);
    multMatrix *mmatrix = new multMatrix();
}

void multmatrix_imp::recvOP()
{
    int op = -1;
    char *buff;
    int bufLen;
    recvMSG(clientID, (void **)&buff, &bufLen);
    op = ((int *)buff)[0];
    delete[] buff;

    switch (op)
    {
    case OP_READMATRIX:
    {
        recvMSG(clientID, (void **)&buff, &bufLen); // receive fileName
        char *file = buff;

        matrix_t *m = mmatrix->readMatrix(file);
        int dim = m->rows;
        sendMSG(clientID, (void *)&dim, sizeof(int)); // send dimension

        for (int i = 0; i < dim * dim; i++)
        {
            int num = *(m->data + i);
            cout << num << endl;
            sendMSG(clientID, (void *)&num, sizeof(int)); // send read matrix
        }
        cout << "enviado" << endl;
        break;
    }
    case OP_MULTMATRIX:
    {
        recvMSG(clientID, (void **)&buff, &bufLen); // receive size
        int size = ((int *)buff)[0];
        matrix_t *m1 = new matrix_t[1];
        m1->rows = size;
        m1->cols = size;
        m1->data = new int[size * size];

        matrix_t *m2 = new matrix_t[1];
        m2->rows = size;
        m2->cols = size;
        m2->data = new int[size * size];
        cout << "mat1 " << endl;
        for (int i = 0; i < size * size; i++)
        {
            recvMSG(clientID, (void **)&buff, &bufLen); // receive numbers for mat1
            m1->data[i] = *buff;
        }
        printMatrix(m1);
        cout << "mat2 " << endl;
        for (int i = 0; i < size * size; i++)
        {
            recvMSG(clientID, (void **)&buff, &bufLen); // receive numbers for mat2
            m2->data[i] = *buff;
        }
        printMatrix(m2);

        matrix_t *m3 = mmatrix->multMatrices(m1, m2);

        for (int i = 0; i < size * size; i++)
        {
            int num = *(m3->data + i);
            sendMSG(clientID, (void *)&num, sizeof(int));
        }

        break;
    }
    case OP_WRITEMATRIX:
    {
        recvMSG(clientID, (void **)&buff, &bufLen); // receive dimension
        int size = ((int *)buff)[0];

        matrix_t *m = new matrix_t[1];
        m->rows = size;
        m->cols = size;
        m->data = new int[size * size];

        for (int i = 0; i < size * size; i++)
        {
            recvMSG(clientID, (void **)&buff, &bufLen); // receive numbers for mat1
            m->data[i] = *buff;
        }
        printMatrix(m);

        recvMSG(clientID, (void **)&buff, &bufLen); // receive fileName
        char *file = buff;
        mmatrix->writeMatrix(m, file);

        break;
    }
    case OP_IDENTITY:
    {
        int rows = 0;
        int cols = 0;
        recvMSG(clientID, (void **)&buff, &bufLen); // receive rows

        rows = ((int *)buff)[0];
        delete[] buff;

        recvMSG(clientID, (void **)&buff, &bufLen); // receive cols
        cols = ((int *)buff)[0];
        delete[] buff;

        matrix_t *m1 = mmatrix->createIdentity(rows, cols);

        for (int i = 0; i < rows * cols; i++)
        {
            int num = *(m1->data + i);
            sendMSG(clientID, (void *)&num, sizeof(int));
        }

        break;
    }
    case OP_RAND:
    {
        int rows = 0;
        int cols = 0;
        recvMSG(clientID, (void **)&buff, &bufLen); // receive rows

        rows = ((int *)buff)[0];
        delete[] buff;

        recvMSG(clientID, (void **)&buff, &bufLen); // receive cols
        cols = ((int *)buff)[0];
        delete[] buff;

        matrix_t *m1 = mmatrix->createRandMatrix(rows, cols);

        for (int i = 0; i < rows * cols; i++)
        {
            int num = *(m1->data + i);
            sendMSG(clientID, (void *)&num, sizeof(int));
        }

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

multmatrix_imp::~multmatrix_imp()
{
    closeConnection(clientID);
}