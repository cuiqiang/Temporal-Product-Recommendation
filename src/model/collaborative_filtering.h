#include <iostream>
#include <vector>
#include <map>
#include <string>


class CollaborativeFiltering{
    std::map<int, std::map<int, int> > matrix;
public:
    double sim(std::map<int,int>, std::map<int,int>);
    void load_model(char*);
    void CF(char*);
    void model_output(char*);
    void train(char*);
    void predict(char*,char*);
};
