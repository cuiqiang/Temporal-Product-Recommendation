#include "../common/common.h"
#include <map>
#include <vector>

struct productPurchaseTime{
    long productid;
    int time;
};

class MarkovTrans{
protected:
    int column;
    int row;
    std::vector<cross_purchase> H;
    std::map<int, std::map<int, double> > trans;    //for predict
    std::map<long,std::map<long,int> > * productTransResult; //for train
public:
    MarkovTrans();
    ~MarkovTrans();
    void train(char*);
    void load_purchase(char*);
    void load_model(char*);
    void markov_trans(char*);
    void model_output(char*);
    void predict(char*,char*);
};
