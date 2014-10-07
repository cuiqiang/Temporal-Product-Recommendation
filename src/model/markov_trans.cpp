#include "markov_trans.h"
#include "purchase_prob.h"
#include "../common/util.h"
#include "../common/constants.h"
#include "../common/SortHeap.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

MarkovTrans::MarkovTrans(){
    productTransResult = new map<long,map<long,int> >;
}

MarkovTrans::~MarkovTrans(){
    if (productTransResult) {
        delete productTransResult;
    }
}

void MarkovTrans::train(char* trainData){
    //load_purchase(trainData);
    markov_trans(trainData);
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void MarkovTrans::load_purchase(char* trainData){
   
    ifstream in;
    in.open(trainData);

    in >> row >> column;
    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, SPACE);
        cross_purchase cp;
        cp.t = atoi(secs[0].c_str());
        cp.u = atoi(secs[1].c_str())-1;
        cp.i = atoi(secs[2].c_str())-1;
        cp.Xuit = atoi(secs[3].c_str());
        cp.Hu_size = atoi(secs[4].c_str());
        cp.Hu = new int[cp.Hu_size];
        vector<string> v = Util::split(secs[5], COMMA);
        for(int i=0; i<cp.Hu_size; i++){
            cp.Hu[i] = atoi(v[i].c_str())-1;
        }
        cp.ruit = atoi(secs[6].c_str());
        H.push_back(cp);
    }
    cout << "load finish!" << endl;
    in.close();
}

void MarkovTrans::markov_trans(char* trainData){
    cout << "generate product-transfer..." << endl;
    long current_user = 0;//the current user
    vector<struct productPurchaseTime> userPurchase;//a temp vetcor for containing productPurchaseTime of each user
    map<long, int> productTimeMap;//a temp pair for product and time,it helps the product find the nearest influenceing product
    
    //open the input-files
    ifstream inputFile;
    inputFile.open(trainData);
    
    inputFile >> row >> column;
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);//read a line
        if(line.size() == 0)
            continue;
        
        vector<string> items = Util::split(line, SPACE);
        int time = atoi(items[0].c_str());
        long userId = atol(items[1].c_str());
        long productId = atol(items[2].c_str());
        
        struct productPurchaseTime productTime;
        productTime.productid = productId;
        productTime.time = time;
        
        if (current_user != userId || inputFile.eof()) {
            if (inputFile.eof()) {
                userPurchase.push_back(productTime);
            }
            for (vector<struct productPurchaseTime>::reverse_iterator i=userPurchase.rbegin(); i!=userPurchase.rend(); i++) {
                struct productPurchaseTime targetProduct = *i;
                for (vector<struct productPurchaseTime>::reverse_iterator j=i+1; j!=userPurchase.rend(); j++) {
                    struct productPurchaseTime sourceProduct = *j;
                    if (sourceProduct.time == targetProduct.time) {//if product i and product j are in one basket
                        continue;
                    }
                    if (productTimeMap.find(sourceProduct.productid) == productTimeMap.end()) {
                        productTimeMap.insert(make_pair(sourceProduct.productid, (targetProduct.time - sourceProduct.time)));
                    }
                }
                for (map<long,int>::iterator it=productTimeMap.begin(); it != productTimeMap.end(); it++) {
                    if (productTransResult->find(it->first) == productTransResult->end()) {
                        (*productTransResult)[it->first][targetProduct.productid] = 0;
                    }
                    (*productTransResult)[it->first][targetProduct.productid] += 1;
                }
                productTimeMap.clear();
            }
            userPurchase.clear();
            current_user = userId;//reset the current-userid
            cout << current_user << endl;
        }
        userPurchase.push_back(productTime);
    }
    inputFile.close();
    return;
}

void MarkovTrans::model_output(char* modelFile){

    cout << "Wtriting the product-transfer file..." << endl;
    ofstream productTransResultFile;
    productTransResultFile.open(modelFile);
    
    productTransResultFile << column << endl;
    
    map<long, map<long, int> >::iterator source,sourceEnd;
    sourceEnd = productTransResult->end();//calculate once
    for (source=productTransResult->begin(); source!=sourceEnd; source++) {
        map<long,int> target = source->second;
        map<long,int>::iterator it,itEnd;
        itEnd = target.end();
        double sum = 0;
        for (it=target.begin(); it!=itEnd; it++) {
            sum += it->second;
        }
        for (it=target.begin(); it!=itEnd; it++) {
            productTransResultFile << source->first << " " << it->first << " " << (it->second) / sum << endl;
            //productTransResultFile << source->first << " " << it->first << " " << (it->second)<< endl;
        }
    }
    
    productTransResultFile.close();
    cout << "Writing the product-transfer file ok!" << endl;

}
void MarkovTrans::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

    model >> column;

    int i,j;
    double p;
    while(!model.eof()){
        model >> i >> j >> p;
        trans[i-1][j-1] = p;
    }

}
void MarkovTrans::predict(char* modelFile, char* testData){

    load_model(modelFile);

    ifstream in;
    in.open(testData);

    ofstream predictOut;
    char predictFile[200];
    strcpy(predictFile, testData);
    predictOut.open(strcat(predictFile, ".predict"));

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;


        vector<string> cols = Util::split(line, TAB);
        vector<string> secs = Util::split(cols[1], COMMA);

        vector<int> purchase_list;
        if(secs[1].size() > 0){
            vector<string> v = Util::split(Util::trim(secs[1]), SPACE);
            for(int i=0;i<v.size();i++){
                vector<string> item_freq = Util::split(v[i], SIMICOLON);
                purchase_list.push_back(atoi(item_freq[0].c_str())-1);
            }
        }

        /*vector<int> add;
        for(int hop=0; hop<1; hop++){
            add.clear();
            for(int j=0; j<column; j++){
                double prob = 0.0;
                for(int i=0; i<purchase_list.size(); i++){
                    if(trans.find(purchase_list[i]) != trans.end() && trans[purchase_list[i]].find(j) != trans[purchase_list[i]].end()){
                        prob += trans[purchase_list[i]][j];
                    }
                }
                if(prob > 0.0){
                    add.push_back(j);
                }
            }
            for(int i=0; i<add.size(); i++){
                purchase_list.push_back(add[i]);
            }
        }*/

        SortHeap<purchaseProb> topK(HEAP_SIZE);
        for(int j=0; j<column; j++){
            purchaseProb pp;
            pp.i = j;
            pp.prob = 0.0;
            for(int i=0; i<purchase_list.size(); i++){
                if(trans.find(purchase_list[i]) != trans.end() && trans[purchase_list[i]].find(j) != trans[purchase_list[i]].end()){
                    pp.prob += trans[purchase_list[i]][j];
                }
            }
            
            pp.prob = pp.prob/purchase_list.size();

            topK.Insert(pp); 
        }
        
        //predict output 
        predictOut << line << COMMA;
        struct purchaseProb* heap = topK.Output();
        for(int k=1; k<HEAP_SIZE+1; k++){
            predictOut << (heap[k].i+1) << SPACE;
        }
        predictOut << endl;
    }
}
