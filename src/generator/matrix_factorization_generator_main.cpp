#include "matrix_factorization_generator.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){

    MatrixFactorizationGenerator mf_gen;
    mf_gen.generate(argv[1],atoi(argv[2]));
}
