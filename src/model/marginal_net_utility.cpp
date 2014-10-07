#include "marginal_net_utility.h"
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

void MarginalNetUtility::train(char* trainData){
    load_purchase(trainData);
    SVD_util();
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void MarginalNetUtility::load_purchase(char* trainData){
   
    ifstream in;
    in.open(trainData);

    in >> row >> column >> K;

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, SPACE);
        purchase p;
        p.t = atoi(secs[0].c_str());
        p.u = atoi(secs[1].c_str())-1;
        p.i = atoi(secs[2].c_str())-1;
        p.Xuit = atoi(secs[3].c_str());
        p.ruit = atoi(secs[4].c_str());
        H.push_back(p);
    }
    cout << "load finish!" << endl;
}

void MarginalNetUtility::initGamma(){
    
    gamma = new double[column];
    for(int i=0; i<column; i++){
        gamma[i] = (double)(rand()%column+1)/column;
    }
    cout << "init Gamma!" << endl;
}

void MarginalNetUtility::SVD_util(int step_count, double lambda1, double lambda2, double beta1, double beta2, double gamma0, double threshold){
    initPQ();
    //initX();
    initGamma();

    for(int step=0; step<step_count; step++){
       
        cout << "iteration: " << step << "\t";

        for(int index=0; index<H.size(); index++){
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int ruit = H[index].ruit;
            double duit = pow(Xuit+1,gamma[i])-pow(Xuit,gamma[i]);
            double vuit = dot(u,i)*duit-0;// ci = 0
            double guit = exp(0-vuit*ruit)/(1+exp(0-vuit*ruit))*(0-ruit);
           
            //X[u][i] = Xuit;
           //cout << u << i << ":" << duit << " " << vuit << " " << guit << " " << Xuit << " " << pow(Xuit,gamma[i]) << endl;

            for(int k=0; k<K; k++){
                double Puk = P[u][k];
                double Qki = Q[k][i];
                P[u][k] = Puk - beta1 * (guit*duit*Qki+lambda1*Puk);
                Q[k][i] = Qki - beta1 * (guit*duit*Puk+lambda1*Qki);
                //cout << P[u][k] << " " << Q[k][i] << endl;
            }
            gamma[i] = gamma[i] - beta2 * (guit*dot(u,i)*(pow(Xuit+1, gamma[i])*log(Xuit+1)-pow(Xuit, gamma[i])*log(Xuit))+lambda2*gamma[i]);
       }
        
        
        double e = 0.0;
        double last_e = 0.0;
        for(int index=0; index < H.size(); index++){
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int ruit = H[index].ruit;
            double duit = pow(Xuit+1,gamma[i])-pow(Xuit,gamma[i]);
            double vuit = dot(u,i)*duit-0;// ci = 0
            e += log(1+exp(0-vuit*ruit));
        }
        for(int u=0; u<row; u++)
            for(int k=0; k<K; k++){
                e += (lambda1/2)*pow(P[u][k],2);
            }

        for(int i=0; i<column; i++){
            for(int k=0; k<K; k++){
                    e += (lambda1/2)*pow(Q[k][i],2);
            }
            e += (lambda2/2)*pow(gamma[i]-gamma0,2);
        }
        cout << "error: " << e << endl;
        if(e - last_e < threshold)
            break;
        last_e = e;
    }

}
void MarginalNetUtility::model_output(char* modelFile){

    ofstream modelOut;
    modelOut.open(modelFile);

    modelOut << row << SPACE << K << SPACE;
    for(int i=0; i<row; i++){
        for(int k=0; k<K; k++){
            modelOut << P[i][k] << SPACE;
        }
    }
    modelOut << endl;
    modelOut << K << SPACE << column << SPACE;
    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            modelOut << Q[k][j] << SPACE;
        }
    }
    modelOut << endl;
    modelOut << column << SPACE;
    for(int i=0; i<column; i++){
        modelOut << gamma[i] << SPACE;
    }
    modelOut << endl;

}
void MarginalNetUtility::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

    model >> row >> K;
    P = new double*[row];
    for(int i=0; i<row; i++){
        P[i] = new double[K];
    }
    
    for(int i=0; i<row; i++){
        for(int k=0; k<K; k++){
            model >> P[i][k];
        }
    }
    
    model >> K >> column;
    Q = new double*[K];
    for(int i=0; i<K; i++){
        Q[i] = new double[column];
    }

    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            model >> Q[k][j];
        }
    }

    model >> column;
    gamma = new double[column];
    for(int i=0; i<column; i++){
        model >> gamma[i];
    }

}
void MarginalNetUtility::predict(char* modelFile, char* testData){

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
        if(secs[1].size() != 0){
            vector<string> v = Util::split(secs[1], SPACE);    
            for(int i=0;i<v.size();i++){
                vector<string> item_freq = Util::split(v[i], SIMICOLON);
                x[item_freq[0]] = atoi(item_freq[1].c_str());
            }
        }

        SortHeap<purchaseProb> topK(HEAP_SIZE);
        for(int i=0; i<column; i++){
            purchaseProb pp;
            pp.i = i;
            int xuit = 0;
            string key = Util::itos(i+1);
            if(x.find(key) != x.end()){
                xuit = x[key];
            }
            
            double vuit = dot(u,i)*(pow(xuit+1,gamma[i])-pow(xuit,gamma[i]));
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
