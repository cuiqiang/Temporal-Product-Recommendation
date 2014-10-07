#include "matrix_factorization_generator.h"
#include <vector>

class MarginalNetUtilityGenerator : public MatrixFactorizationGenerator {
    
public:
    void generateTrainTest(char*, int);
    void generate(char*, int);
};
