#include "cross_selling.h"
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

void CrossSelling::train(char* trainData){
    load_purchase(trainData);
    SVD_cross();
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void CrossSelling::load_purchase(char* trainData){
   
    ifstream in;
    in.open(trainData);

    in >> row >> column >> K;

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
    in.close();
    cout << "load finish!" << endl;
}

void CrossSelling::SVD_cross(int step_count, double lambda1, double lambda2, double beta1, double beta2, double gamma0, double threshold){

    for(int step=0; step<step_count; step++){
       
        cout << "iteration: " << step << "\t";

        for(int index=0; index<H.size(); index++){
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int Hu_size = H[index].Hu_size;
            int* Hu = H[index].Hu;
            int ruit = H[index].ruit;
            
            double vuit = 0.0;
            for(int j=0; j<Hu_size; j++){
                if(trans.find(Hu[j]) != trans.end() && trans[Hu[j]].find(i) != trans[Hu[j]].end()){
                    vuit += trans[Hu[j]][i];
                }
            }

            vuit = vuit/Hu_size;
            double guit = exp(0-vuit*ruit)/(1+exp(0-vuit*ruit))*(0-ruit);
           
            //cout << u << SPACE << i << ":" << vuit << " " << guit << " " << endl;
            for(int j=0; j<Hu_size; j++){
                if(trans.find(Hu[j]) != trans.end() && trans[Hu[j]].find(i) != trans[Hu[j]].end()){
                    trans[Hu[j]][i] = trans[Hu[j]][i] - beta1 * (guit/Hu_size+lambda1*trans[Hu[j]][i]);
                    //trans[Hu[j]][i] = trans[Hu[j]][i] - beta1 * (guit/Hu_size);
                }else{
                    trans[Hu[j]][i] = 0.0 - beta1 * (guit/Hu_size);
                }
            }
       }

        
        double e = 0.0;
        double last_e = 0.0;
        for(int index=0; index < H.size(); index++){
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int Hu_size = H[index].Hu_size;
            int* Hu = H[index].Hu;
            int ruit = H[index].ruit;
            double vuit = 0.0;// ci = 0
            for(int j=0; j<Hu_size; j++){
                if(trans.find(Hu[j]) != trans.end() && trans[Hu[j]].find(i) != trans[Hu[j]].end()){
                    vuit += trans[Hu[j]][i];
                }
            }
            vuit = vuit/Hu_size;
            e += log(1+exp(0-vuit*ruit));
        }
        map<int, map<int, double> >::iterator itr1;
        map<int, double>::iterator itr2;
        for(itr1=trans.begin(); itr1!=trans.end(); itr1++){
            for(itr2=itr1->second.begin(); itr2!=itr1->second.end(); itr2++){
                e += (lambda2/2)*pow(itr2->second,2);
            }
        }
        cout << "error: " << e << endl;
        if(e - last_e < threshold)
            break;
        last_e = e;
    }

}
void CrossSelling::model_output(char* modelFile){

    ofstream modelOut;
    modelOut.open(modelFile);
    
    map<int, map<int, double> >::iterator itr1;
    map<int, double>::iterator itr2;
    for(itr1=trans.begin(); itr1!=trans.end(); itr1++){
        for(itr2=itr1->second.begin(); itr2!=itr1->second.end(); itr2++){
            modelOut << itr1->first << SPACE << itr2->first << SPACE << itr2->second << SPACE;
        }
    }
    modelOut << endl;

}
void CrossSelling::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

    int i,j;
    double p;
    while(!model.eof()){
        model >> i >> j >> p;
        trans[i][j] = p;
    }

}
void CrossSelling::predict(char* modelFile, char* testData){

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

        map<string, int> x;

        vector<string> cols = Util::split(line, TAB);
        vector<string> secs = Util::split(cols[1], COMMA);
        int u = atoi(secs[0].c_str())-1;

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
            
            double vuit = 0.0;
            map<string,int>::iterator itr;
            for(itr=x.begin(); itr!=x.end(); itr++){
                int j = atoi(itr->first.c_str())-1;
                if(trans.find(j) != trans.end() && trans[j].find(i) != trans[j].end()){
                    vuit += trans[j][i];
                }
            }
            
            vuit = vuit/x.size();

            pp.prob =  1.0/(1+exp(0-vuit));
            
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
