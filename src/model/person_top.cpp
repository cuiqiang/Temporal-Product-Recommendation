#include "person_top.h"
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
#include <map>

using namespace std;

void PersonTop::predict(char* trainData, char* testData){

    ifstream train;
    train.open(trainData);
    train >> row >> column >> K;
    train.close();

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

        map<string, int> x;
        
        vector<string> cols = Util::split(line, TAB);
        vector<string> secs = Util::split(cols[1], COMMA);
      
        if(secs[1].size() > 0){
            vector<string> v = Util::split(Util::trim(secs[1]), SPACE);
            for(int i=0;i<v.size();i++){
                vector<string> item_freq = Util::split(v[i], SIMICOLON);
                x[item_freq[0]] = atoi(item_freq[1].c_str());
            }
        }

        SortHeap<purchaseProb> topK(HEAP_SIZE);
        for(int i=0; i<column; i++){
            purchaseProb pp;
            pp.i = i;
           
            int freq = 0;
            if(x.find(Util::itos(i+1)) != x.end()){
                freq = x[Util::itos(i+1)];
            }
            
            pp.prob = 1.0*freq;
            
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
