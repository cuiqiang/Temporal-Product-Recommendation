#include "cross_selling_factorization.h"
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

void CrossSellingFactorization::train(char* trainData){
    load_purchase(trainData);
    SVD_cross();
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void CrossSellingFactorization::initIJ(){
    
    //initial I & J
    I = new double*[column];
    for(int i=0; i<column; i++){
        I[i] = new double[K];
    }
    
    J = new double*[K];
    for(int i=0; i<K; i++){
        J[i] = new double[column];
    }
    
    for(int i=0; i<column; i++){
        for(int k=0; k<K; k++){
            I[i][k] = ((double)(rand()%K+1)/K);
        }
    }
    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            J[k][j] = ((double)(rand()%K+1)/K);
        }
    }

    cout << "init IJ!" << endl;
} 

void CrossSellingFactorization::load_purchase(char* trainData){
   
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
    cout << "load finish!" << endl;
}

double CrossSellingFactorization::dot(double** I, int i, double** J, int j){
    double dot_result = 0.0;
    for(int k=0; k<K; k++){
        dot_result += I[i][k]*J[k][j];
    }
    return dot_result;
}

void CrossSellingFactorization::SVD_cross(int step_count, double lambda1, double lambda2, double beta1, double beta2, double gamma0, double threshold){
    initIJ();

    for(int step=0; step<step_count; step++){
       
        cout << "iteration: " << step << "\t";

        for(int index=0; index<H.size(); index++){
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int Hu_size = H[index].Hu_size;
            int* Hu = H[index].Hu;
            int ruit = H[index].ruit;
            
            double vuit = 0.0;// ci = 0
            for(int j=0; j<Hu_size; j++){
                vuit += dot(I,Hu[j],J,i);
            }
            double guit = exp(0-vuit*ruit)/(1+exp(0-vuit*ruit))*(0-ruit);
           
            //cout << u << SPACE << i << ":" << vuit << " " << guit << " " << endl;
            for(int h=0; h<Hu_size; h++){
                int j = Hu[h];
                for(int k=0; k<K; k++){
                    double Ijk = I[j][k];
                    double Jki = J[k][i];
                    I[j][k] = Ijk - beta1 * (guit*Jki+lambda1*Ijk);
                    J[k][i] = Jki - beta1 * (guit*Ijk+lambda1*Jki);
                    //cout << P[u][k] << " " << Q[k][i] << endl;
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
                 vuit += dot(I,Hu[j],J,i);
            }
            e += log(1+exp(0-vuit*ruit));
        }
        for(int i=0; i<column; i++)
            for(int k=0; k<K; k++){
                e += (lambda1/2)*pow(I[i][k],2);
            }

        for(int j=0; j<column; j++){
            for(int k=0; k<K; k++){
                e += (lambda1/2)*pow(J[k][j],2);
            }
        }
        cout << "error: " << e << endl;
        if(e - last_e < threshold)
            break;
        last_e = e;
    }

}
void CrossSellingFactorization::model_output(char* modelFile){

    ofstream modelOut;
    modelOut.open(modelFile);

    modelOut << column << SPACE << K << SPACE;
    for(int i=0; i<column; i++){
        for(int k=0; k<K; k++){
            modelOut << I[i][k] << SPACE;
        }
    }
    modelOut << endl;
    modelOut << K << SPACE << column << SPACE;
    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            modelOut << J[k][j] << SPACE;
        }
    }
    modelOut << endl;
}
void CrossSellingFactorization::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

        
    model >> column >> K;
        
    I = new double*[column];
    for(int i=0; i<column; i++){
        I[i] = new double[K];
    }

    cout << "1111111" << endl;
    
    for(int i=0; i<column; i++){
        for(int k=0; k<K; k++){
            model >> I[i][k];
        }
    }


    cout << "load I finish" << endl;

    model >> K >> column;
    J = new double*[K];
    for(int i=0; i<K; i++){
        J[i] = new double[column];
    }

    cout << "22222222222" << endl;

    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            model >> J[k][j];
        }
    }

    cout << "load J finish" << endl;
    
}
void CrossSellingFactorization::predict(char* modelFile, char* testData){

    cout << "predict" << endl; 

    load_model(modelFile);
    
    ifstream in;
    in.open(testData);

    ofstream predictOut;
    char predictFile[200];
    strcpy(predictFile, testData);
    predictOut.open(strcat(predictFile, ".predict"));
    int heap_size = 5;


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

        SortHeap<purchaseProb> topK(heap_size);
        for(int i=0; i<column; i++){
            purchaseProb pp;
            pp.i = i;
            
            double vuit = 0.0;
            map<string,int>::iterator itr;
            for(itr=x.begin(); itr!=x.end(); itr++){
                int j = atoi(itr->first.c_str())-1;
                vuit += dot(I,j,J,i);
            }
            pp.prob =  1.0/(1+exp(0-vuit));
            
            topK.Insert(pp); 
        }
        
        //predict output 
        predictOut << line << COMMA;
        struct purchaseProb* heap = topK.Output();
        for(int k=1; k<heap_size+1; k++){
            predictOut << (heap[k].i+1) << SPACE;
        }
        predictOut << endl;
    }
}
