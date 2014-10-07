#include "top_pop.h"
#include "../common/util.h"
#include "../common/constants.h"
#include "../common/SortHeap.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <math.h>
#include <sstream>

using namespace std;

void TopPop::top_pop(char* trainData){
    
    ifstream in;
    in.open(trainData);

    map<string, int> productCount;

    while(!in.eof()){
        string line;
        getline(in, line);
        if (line.size() == 0)
            continue;
        
        vector<string> secs = Util::split(line, SPACE);
        string productId = secs[2];
        if(productCount.find(productId) != productCount.end()){
            productCount[productId] += 1;
        }else{
            productCount[productId] = 1;
        }
    }
    
    SortHeap<purchaseProb> topK(HEAP_SIZE);
    map<string, int>::iterator itr;
    for(itr=productCount.begin();itr!=productCount.end();itr++){
        purchaseProb pp;
        pp.i = atoi(itr->first.c_str());
        pp.prob = 1.0*itr->second;
        topK.Insert(pp);
    }
    
    struct purchaseProb* heap = topK.Output();
    for(int k=1; k<HEAP_SIZE+1; k++){
        topProduct.push_back(heap[k].i);
    }
}

void TopPop::model_output(char* modelFile){
    
    ofstream modelOut;
    modelOut.open(modelFile);

    modelOut << topProduct.size() << SPACE;  

    for(int i=0; i<topProduct.size(); i++){
        modelOut << topProduct[i] << SPACE;
    }
    
}

void TopPop::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

    int size;
    model >> size;
    
    for(int i=0; i<size; i++){
        int productId;
        model >> productId;
        topProduct.push_back(productId);
    }
}

void TopPop::train(char* trainData){
    top_pop(trainData);
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void TopPop::predict(char* modelFile, char* testData){

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
        
        //output 
        predictOut << line << COMMA;
        for(int i=0; i<topProduct.size(); i++){
            predictOut << Util::itos(topProduct[i]) << SPACE;
        }
        predictOut << endl;
    }
}
