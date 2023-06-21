/*	JAIME VILLAR	*/

#include "utils.h"
#include "multmatrix.h"

using namespace std;

#define OP_FIN -1
#define OP_READMATRIX 0
#define OP_MULTMATRIX 1
#define OP_WRITEMATRIX 2
#define OP_IDENTITY 3
#define OP_RAND 4

class multmatrix_stub
{
    connection_t conn;

public:
    multmatrix_stub();
    matrix_t *readMatrix(const char *fileName);
    matrix_t *multMatrices(matrix_t *m1, matrix_t *m2);
    void writeMatrix(matrix_t *m, const char *fileName);
    ~multmatrix_stub();
    matrix_t *createIdentity(int rows, int cols);
    matrix_t *createRandMatrix(int rows, int cols);
};