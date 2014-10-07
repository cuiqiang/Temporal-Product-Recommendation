#include "model_analysis.h"
#include "../common/util.h"
#include "../common/constants.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

void ModelAnalysis::compare(char* file1, char* file2, char* file3){
    cross_load(file1);
    markov_load(file2);
    load_purchase(file3);
    train_data();
    map<int, map<int,double> >::iterator itr1;
    map<int,double>::iterator itr2;
    for(itr1=model1.begin(); itr1!=model1.end(); itr1++){
        for(itr2=itr1->second.begin(); itr2!=itr1->second.end(); itr2++){
            cout << itr1->first << SPACE << itr2->first << SPACE << itr2->second << SPACE << model2[itr1->first][itr2->first] << SPACE << train[itr1->first][itr2->first] << endl;
        }
    }
}
void ModelAnalysis::cross_load(char* modelFile){

    ifstream model;
    model.open(modelFile);

    int i,j;
    double p;
    while(!model.eof()){
        model >> i >> j >> p;
        model2[i][j] = p;
    }

    cout << "load cross!" << endl;
}
void ModelAnalysis::markov_load(char* initFile){

    ifstream init;
    init.open(initFile);

    int init_column;
    init >> init_column;

    int i,j;
    double p;
    while(!init.eof()){
        init >> i >> j >> p;
        model1[i-1][j-1] = p;
    }

    cout << "load markov!" << endl;
}
void ModelAnalysis::load_purchase(char* trainData){
   
    ifstream in;
    in.open(trainData);

    int row,column,K;
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
    cout << "load finish!" << endl;
}
void ModelAnalysis::train_data(){
for(int i=0; i < 5 ; i++){
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
                    trans[Hu[j]][i] = trans[Hu[j]][i] - 0.015  * (guit/Hu_size+0.05*trans[Hu[j]][i]);
                    train[Hu[j]][i] += Util::ftos(trans[Hu[j]][i]) + SPACE;
                }else{
                    trans[Hu[j]][i] = 0.0 - 0.015 * (guit/Hu_size);
                    train[Hu[j]][i] = Util::ftos(trans[Hu[j]][i]) + SPACE;
                }
            }
       }

}
}
