#include "matrix_factorization.h"
#include "purchase_prob.h"
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

void MatrixFactorization::init(){
    
    //init P(u|z) P(v|z)
    Pu_z = new double*[row];
    for(int u=0; u<row; u++){
        Pu_z[u] = new double[K];
        double norm = 0.0;
        for(int z=0; z<K; z++){
        	double val = (double)rand();
        	Pu_z[u][z] = val;
        	norm += val; 
        }
        for(int z=0; z<K; z++){
        	Pu_z[u][z] /= norm;
        	//cout << Pu_z[u][z] << SPACE;
        }
        //cout << endl;
    }
    
    Qv_z = new double*[column];
    for(int v=0; v<column; v++){
        Qv_z[v] = new double[K];
		double norm = 0.0;
        for(int z=0; z<K; z++){
        	double val = (double)rand();
        	Qv_z[v][z] = val;
        	norm += val; 
        }
        for(int z=0; z<K; z++){
        	Qv_z[v][z] /= norm;
        	//cout << Qv_z[v][z] << SPACE;
        }
        //cout << endl;
    }

    cout << "init p(u|z), p(v|z)" << endl;

}


void MatrixFactorization::matrix_factorization(int step_count, double alpha, double beta, double threshold)
{
    init();
    for(int step=0; step<step_count; step++){
    
        cout << "iteration: " << step << "\t";

        for(int index=0; index < R.size(); index++){
            int i = R[index].i;
            int j = R[index].j;
            double eij = R[index].rate - dot(i,j);
            for(int k=0; k<K; k++){
                double Pik = Pu_z[i][k];
                double Qkj = Qv_z[j][k];
                Pu_z[i][k] = Pik + alpha * (2*eij*Qkj-beta*Pik);
                Qv_z[j][k] = Qkj + alpha * (2*eij*Pik-beta*Qkj);
            }
        }
        double e = 0.0;
        for(int index=0; index < R.size(); index++){
            int i = R[index].i;
            int j = R[index].j;
            e += pow(R[index].rate - dot(i,j), 2);
        }
        for(int i=0; i<row; i++){
            for(int k=0; k<K; k++){
                e += (beta/2)*(pow(Pu_z[i][k],2));
            }
        }
        for(int j=0; j<column; j++){
            for(int k=0; k<K; k++){
                e += (beta/2)*(pow(Qv_z[j][k],2));
            }
        }
        cout << "error: " << e << endl;
        if(e < threshold)
            break;
    }
}

double MatrixFactorization::dot(int i, int j){
    double dot_result = 0.0;
    for(int k=0; k<K; k++){
        dot_result += Pu_z[i][k]*Qv_z[j][k];
    }
    return dot_result;
}

void MatrixFactorization::debug_model_para(){
	for(int i=0; i<row; i++){
		for(int k=0; k<K; k++){
			cout << Pu_z[i][k] << SPACE;
		}
		cout << endl;
	}
	for(int j=0; j<column; j++){
		for(int k=0; k<K; k++){
			cout << Qv_z[j][k] << SPACE;
		}
		cout << endl;
	}

	for(int i=0; i<row; i++){
		for(int j=0; j<column; j++){
			cout << dot(i,j) << SPACE;
		}
		cout << endl;
	}
}

void MatrixFactorization::model_output(char* modelFile){
    
    ofstream modelOut;
    modelOut.open(modelFile);

    modelOut << row << SPACE << K << SPACE;
    for(int i=0; i<row; i++){
        for(int k=0; k<K; k++){
            modelOut << Pu_z[i][k] << SPACE;
        }
    }
    modelOut << endl;
    modelOut << K << SPACE << column << SPACE;
    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            modelOut << Qv_z[k][j] << SPACE;
        }
    }
    modelOut << endl;

	for(int i=0; i<row; i++){
		for(int j=0; j<column; j++){
			cout << dot(i,j) << SPACE;
		}
		cout << endl;
	}
}

void MatrixFactorization::load_model(char* modelFile){

    ifstream model;
    model.open(modelFile);

    model >> row >> K;
    Pu_z = new double*[row];
    for(int i=0; i<row; i++){
        Pu_z[i] = new double[K];
    }
    
    for(int i=0; i<row; i++){
        for(int k=0; k<K; k++){
            model >> Pu_z[i][k];
        }
    }
    
    model >> K >> column;
    Qv_z = new double*[K];
    for(int i=0; i<K; i++){
        Qv_z[i] = new double[column];
    }

    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            model >> Qv_z[k][j];
        }
    }

}

void MatrixFactorization::train(char* trainData){
    load_matrix(trainData);
    matrix_factorization();
    char modelFile[200];
    strcpy(modelFile,trainData);
    model_output(strcat(modelFile,".model"));
}

void MatrixFactorization::load_matrix(char* trainData){
   
    ifstream in;
    in.open(trainData);

    in >> row >> column >> K;

    while(!in.eof()){
        string line;
        getline(in, line);
        if (line.size() == 0)
            continue;
        
        vector<string> secs = Util::split(line, SPACE);
        element e;
        e.i = atoi(secs[0].c_str())-1;
        e.j = atoi(secs[1].c_str())-1;
        e.rate = atof(secs[2].c_str());
        R.push_back(e);
    }
    cout << "load finish!" << endl;
}

void MatrixFactorization::predict(char* modelFile, char* testData){

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

        vector<string> cols= Util::split(line, TAB); 
        vector<string> secs = Util::split(cols[1], COMMA);
        int u = atoi(secs[0].c_str())-1;


        SortHeap<purchaseProb> topK(HEAP_SIZE);
        for(int i=0; i<column; i++){
            purchaseProb pp;
            pp.i = i;
            pp.prob = dot(u,i);
            topK.Insert(pp); 
        }
        
        //output 
        predictOut << line << COMMA;
        struct purchaseProb* heap = topK.Output();
        for(int k=1; k<HEAP_SIZE+1; k++){
            predictOut << (heap[k].i+1) << SPACE;
        }
        predictOut << endl;
    }
}
