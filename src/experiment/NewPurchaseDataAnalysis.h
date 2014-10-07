//
//  NewPurchaseDataAnalysis.h
//  new_purchase_data_anaysis
//
//  Created by Eagle on 13-7-24.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#ifndef new_purchase_data_anaysis_NewPurchaseDataAnalysis_h
#define new_purchase_data_anaysis_NewPurchaseDataAnalysis_h

#include <string>
#include <map>
#include <vector>

using namespace std;

struct predictProductInfo{
    int appearTimes;//how many times the product appear in test set
    int influencedTimes[11];//how many(0~10) products in the history influence the predicted products
    string influenceProductList;//in form of "sourceProduct1,sourceProduct2,..."(repeatable)
    int count;//the total size of influenceProductList
};

class NewPurchaseDataAnalysis{
private:
    string state;//"new","re" or "all"
    int controlThreshold;
    string testFileName;//the test file for analysising
    string transferModelFileName;//the transfer model file assists for test file analysising
    int preditctProductSize;//the total count of predicted products(repeatable)
    map <long, vector<long> >* transferModel;//{targetProduct:<sourceProduct1,sourceProduct2,...>, ...}
    map<long, struct predictProductInfo>* predictFromHistory;//the result of predicted product influenced from history
    
public:
    NewPurchaseDataAnalysis(string state, string testFileName, string transferModelFileName);
    ~NewPurchaseDataAnalysis();
    void predictFromHistoryAnalysis();//the fuction of analysising the degree that history products influece predicted products.Including
    void loadTransferModel();//load transfer model from transfer model file into transferModel
    void outputPredictFromHistoryAnalysis();//output the results of predictFromHistory
    void setControlThreshold(int controlThreshold){this->controlThreshold = controlThreshold;}
    void resetValues();
};

#endif
