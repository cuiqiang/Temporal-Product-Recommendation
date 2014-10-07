#include "matrix_factorization.h"

int main(int argc, char* argv[]){

    MatrixFactorization mf;
    mf.train(argv[1]);
}
