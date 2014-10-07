#include <map>
#include <vector>
#include <string>
#include "../common/common.h"

class ModelAnalysis{
    std::vector<cross_purchase> H;
    std::map<int, std::map<int,double> > model1;
    std::map<int, std::map<int,double> > model2;
    std::map<int, std::map<int,double> > trans;
    std::map<int, std::map<int,std::string> > train;
public:
    void compare(char*, char*, char*);
    void cross_load(char*);
    void markov_load(char*);
    void load_purchase(char*);
    void train_data();
};
