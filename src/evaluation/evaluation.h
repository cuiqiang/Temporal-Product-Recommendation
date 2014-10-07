#include <vector>
#include <string>

class Evaluation{
    int total;
public:
    void evaluate(char*, int, char*, char*);
    double MAP(int, std::vector<std::string>, std::vector<std::string>);
    void getTotal();
};
