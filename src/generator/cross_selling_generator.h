#include "matrix_factorization_generator.h"

class CrossSellingGenerator : public MatrixFactorizationGenerator {
    
public:
    void generateTrainTest(char*, int);
    void generate(char*, int);
};
