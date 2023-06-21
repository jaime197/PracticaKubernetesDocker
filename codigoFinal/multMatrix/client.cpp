/*	JAIME VILLAR	*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "multmatrix_stub.h"

void freeMatrix(matrix_t *m)
{
    delete[] m->data;
    delete[] m;
}
// int main()
//{
//     multMatrix* mmatrix=new multMatrix();
//
//
//     matrix_t* m1= mmatrix->createRandMatrix(5,5);
//     matrix_t* m2= mmatrix->createIdentity(5,5);
//     matrix_t* mres=mmatrix->multMatrices(m1,m2);
//     mmatrix->writeMatrix(mres,"resultado.txt");
//     matrix_t* m3=mmatrix->readMatrix("resultado.txt");
//     matrix_t* mres2=mmatrix->multMatrices(m1,m3);
//     mmatrix->writeMatrix(mres2,"resultado2.txt");
//
//
//
//     freeMatrix(m1);
//     freeMatrix(m2);
//     freeMatrix(mres);
//     freeMatrix(m3);
//     freeMatrix(mres2);
//     delete mmatrix;
//
// }

int main()
{
    multmatrix_stub *mmatrix = new multmatrix_stub();

    matrix_t *m1 = mmatrix->createRandMatrix(5, 5);
    matrix_t *m2 = mmatrix->createIdentity(5, 5);
    matrix_t *mres = mmatrix->multMatrices(m1, m2);
    mmatrix->writeMatrix(mres, "resultado.txt"); // tiene que ser un nuevo archivo, no sobreescribe archivos existentes
    matrix_t *m3 = mmatrix->readMatrix("resultado.txt");
    matrix_t *mres2 = mmatrix->multMatrices(m1, m3);
    mmatrix->writeMatrix(mres2, "resultado2.txt"); // tiene que ser un nuevo archivo, no sobreescribe archivos existentes

    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(mres);
    freeMatrix(m3);
    freeMatrix(mres2);
    delete mmatrix;
}
