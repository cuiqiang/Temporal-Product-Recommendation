//
//  NewPurchaseDataAnalysis.cpp
//  new_purchase_data_anaysis
//
//  Created by Eagle on 13-7-24.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#include "NewPurchaseDataAnalysis.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../common/constants.h"
#include "../common/util.h"
#include <string>
#include <algorithm>
//#include "constants.h"
//#include "util.h"


using namespace std;

NewPurchaseDataAnalysis::NewPurchaseDataAnalysis(string state, string testFileName, string transferModelFileName){
    this->state = state;
    this->testFileName = testFileName;
    this->transferModelFileName = transferModelFileName;
    transferModel = new map<long, vector<long> >;
    predictFromHistory = new map<long, struct predictProductInfo>;
    preditctProductSize = 0;
}

NewPurchaseDataAnalysis::~NewPurchaseDataAnalysis(){
    if (transferModel) {
        delete transferModel;
    }
    if (predictFromHistory) {
        delete predictFromHistory;
    }
}

void NewPurchaseDataAnalysis::loadTransferModel(){
    vector<long> temp;
    
    cout << "Loading the transfer model file ..." << endl;
    
    //open file
    ifstream transferModelFile;
    transferModelFile.open(transferModelFileName.c_str());
    string line;
    getline(transferModelFile,line);
    
    while (!transferModelFile.eof()) {
        getline(transferModelFile,line);
        //cout << line << endl;

        if (line.size() == 0) {
            continue;
        }
        
        vector<string> items = Util::split(line, SPACE);
        long sourceProduct = atol(items[0].c_str());
        long targetProduct = atol(items[1].c_str());
        
        if (transferModel -> find(targetProduct) == transferModel -> end()) {
            temp.insert(temp.end(), sourceProduct);
            (*transferModel)[targetProduct] = temp;
            temp.clear();
        }
        else{
            (*transferModel)[targetProduct].insert((*transferModel)[targetProduct].end(), sourceProduct);
        }
    }
}

void NewPurchaseDataAnalysis::predictFromHistoryAnalysis(){
    //loadTransferModel();
    
    vector<string>::iterator pit, sit, pend, send;
    struct predictProductInfo temp;
    long predictedProduct;
    long historyProduct;
    vector<long> predictProductTransferVector;//contains all source products influncing the predicted product
    int influencedCount;
    
    //cout << "Analysising the test file ..." << endl;
    
    //open file
    ifstream testFile;
    testFile.open(testFileName.c_str());
    
    while (!testFile.eof()) {
        string line;
        getline(testFile,line);
        if (line.size() == 0) {
            continue;
        }
        
        vector<string> items = Util::split(line, TAB);
        line = items[1];
        items = Util::split(line, COMMA);
        vector<string> historyProductsString = Util::split(items[1], SPACE);
        vector<string> predictedProductsString = Util::split(items[2], SPACE);
        vector<string> historyProducts,predictedProducts;
        
        send = historyProductsString.end();
        for (sit = historyProductsString.begin(); sit != send; sit++) {
            items = Util::split((*sit), SIMICOLON);
            historyProducts.insert(historyProducts.end(), items[0]);
        }
        
        pend = predictedProductsString.end();
        for (pit = predictedProductsString.begin(); pit != pend; pit++) {
            items = Util::split((*pit), SIMICOLON);
            predictedProducts.insert(predictedProducts.end(), items[0]);
        }
        
        pend = predictedProducts.end();
        for (pit = predictedProducts.begin(); pit != pend; pit++) {
            influencedCount = 0;
            if (*pit=="") {
                continue;
            }
            
            if (state == "new") {
                if (find(historyProducts.begin(), historyProducts.end(), *pit) != historyProducts.end()) {
                    continue;
                }
            }
            else if (state == "re"){
                if (find(historyProducts.begin(), historyProducts.end(), *pit) == historyProducts.end()) {
                    continue;
                }
            }
            
            predictedProduct = atol((*pit).c_str());
            preditctProductSize++;
            if (predictFromHistory->find(predictedProduct) == predictFromHistory->end()) {
                temp.appearTimes = 1;
                for (int i=0; i<=10; i++) {
                    temp.influencedTimes[i] = 0;
                }
                temp.influenceProductList = "";
                temp.count = 0;
                (*predictFromHistory)[predictedProduct] = temp;
            }
            else{
                (*predictFromHistory)[predictedProduct].appearTimes += 1;
            }
            if (transferModel->find(predictedProduct) == transferModel->end()) {//if the predict product out of model
                (*predictFromHistory)[predictedProduct].influencedTimes[0] += 1;
                continue;
            }
            predictProductTransferVector = (*transferModel)[predictedProduct];
            send = historyProducts.end();
            for (sit = historyProducts.begin(); sit != send; sit++) {
                historyProduct = atol((*sit).c_str());
                if (find(predictProductTransferVector.begin(), predictProductTransferVector.end(), historyProduct) != predictProductTransferVector.end()) {//if historyProduct in the vector
                    influencedCount += 1;
                    (*predictFromHistory)[predictedProduct].count += 1;
                    (*predictFromHistory)[predictedProduct].influenceProductList += Util::ltos(historyProduct) + ",";
                }
            }
            if (influencedCount>10) {
                influencedCount=10;
            }
            (*predictFromHistory)[predictedProduct].influencedTimes[influencedCount] += 1;
        }
    }
    testFile.close();
    outputPredictFromHistoryAnalysis();
}

void NewPurchaseDataAnalysis::outputPredictFromHistoryAnalysis(){
    //cout << "The analysis results:" << endl;
    double influenceTimes = 0;
    map<long,struct predictProductInfo>::iterator it,itend;
    itend = predictFromHistory->end();
    for (it = predictFromHistory->begin(); it != itend; it++) {
        for (int i=controlThreshold; i<=10; i++) {
            influenceTimes += it->second.influencedTimes[i];
        }
    }
    cout<<"preditctProductSize="<<preditctProductSize<<","<<"influenceTimes="<<influenceTimes<<","<<"influenceRate="<<influenceTimes/preditctProductSize*100<<"%"<<endl;
}

void NewPurchaseDataAnalysis::resetValues(){
    preditctProductSize = 0;
    predictFromHistory->clear();
}
