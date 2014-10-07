#include "dataset_generator.h"
#include <string>
#include <vector>

struct purchase_record{
    std::string t;
    std::string u;
    std::string i;
};

class MatrixFactorizationGenerator{
protected:
    int data_size;
    int user_size;
    int product_size;
    int K;
    std::vector<purchase_record> buffer;
    int train_size;
public:
    void dataStat(char*, int);
    void generateTrainTest(char*, int);
    void generate(char*, int);
};
