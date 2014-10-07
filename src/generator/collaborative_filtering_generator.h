#include "dataset_generator.h"
#include <string>

struct purchase_record{
    std::string t;
    std::string u;
    std::string i;
};

class CollaborativeFilteringGenerator{
    int data_size;
public:
    void generateTrainTest(char*);
    void generate(char*);
};
