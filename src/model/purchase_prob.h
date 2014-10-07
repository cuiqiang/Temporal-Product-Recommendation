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

