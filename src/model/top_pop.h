#include <iostream>
#include <vector>
#include <map>
#include <string>

struct purchaseProb{
    int i;
    double prob;
    bool operator<(const purchaseProb &a)const{
        return prob < a.prob;
    }
    bool operator>(const purchaseProb &a)const{
        return prob > a.prob;
    }
    bool operator<=(const purchaseProb &a)const{
        return prob <= a.prob;
    }
    bool operator>=(const purchaseProb &a)const{
        return prob >= a.prob;
    }
};

class TopPop{
    std::vector<int> topProduct;
public:
    void load_model(char*);
    void top_pop(char*);
    void model_output(char*);
    void train(char*);
    void predict(char*,char*);
};
