#include "collaborative_filtering.h"
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

void CollaborativeFiltering::CF(char* trainData){
    
    ifstream in;
    in.open(trainData);

    while(!in.eof()){
        string line;
        getline(in, line);
        if (line.size() == 0)
            continue;
        
        vector<string> secs = Util::split(line, SPACE);
        int u = atoi(secs[1].c_str())-1;
        int i = atoi(secs[2].c_str())-1;
        if(matrix.find(u) != matrix.end() && matrix[u].find(i) != matrix[u].end()){
            matrix[u][i] += 1;
        }else{
            matrix[u][i] = 1;
        }
    }
    in.close();

    int neighbor_size = 5;
    map<int, map<int,int> >::iterator ui,uj;
    map<int,int>::iterator i;
    for(ui=matrix.begin();ui!=matrix.end();ui++){
        SortHeap<purchaseProb> knn(neighboor_size);
        for(uj=matrix.begin();uj!=matrix.end();uj++){
            purchaseProb pp;
            pp.i = uj->first;
            pp.prob = sim(ui->second,uj->second);

            knn.Insert(pp);
        }
        
        struct purchaseProb* heap = knn.Output();
        for(int k=1; k<neighbor_size+1; k++){
            cout << ui->first << SPACE << (heap[k].i+1) << endl;
        }
    }
    
}

double CollaborativeFiltering::sim(map<int,int> a, map<int,int> b){
    double sim_score = 0.0;
    map<int,int>::iterator i;
    for(i=a.begin();i!=a.end();i++){
        if(b.find(i->first) != b.end()){
            sim_score = sim_score + 1;
        }
    }
    return sim_score;
}
void CollaborativeFiltering::model_output(char* modelFile){
    
    ofstream modelOut;
    modelOut.open(modelFile);

    map<int, map<int,int> >::iterator itr1;
    map<int, int>::iterator itr2;
    for(itr1=matrix.begin(); itr1!=matrix.end();itr1++){
        for(itr2=itr1->second.begin(); itr2!=itr1->second.end(); itr2++){
            modelOut << itr1->first << SPACE << itr2->first << SPACE << itr2->second << SPACE;
        }
    }
    modelOut.close();
}

void CollaborativeFiltering::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

    while(!model.eof()){
        int u,i,freq;
        model >> u >> i >> freq;
        matrix[u][i] = freq;
    }
}

void CollaborativeFiltering::train(char* trainData){
    CF(trainData);
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void CollaborativeFiltering::predict(char* modelFile, char* testData){

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
        /*for(int i=0; i<topProduct.size(); i++){
            predictOut << Util::itos(topProduct[i]) << SPACE;
        }*/
        predictOut << endl;
    }
}
