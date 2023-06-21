/*	JAIME VILLAR	*/

#include "multmatrix.h"

class multmatrix_imp
{
    int clientID;
    multMatrix *mmatrix;

public:
    bool salir = false;
    multmatrix_imp(int clientID);
    void recvOP();
    ~multmatrix_imp();
};
