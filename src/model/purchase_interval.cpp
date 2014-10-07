#include "purchase_interval.h"
#include "util.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace std;

void PurchaseInterval::svdpi_evaluation(){
    
    /*for(int i=0; i<row; i++){
        for(int k=0; k<K; k++){
            cout << P[i][k] << " ";
        }
        cout << endl;
    }

    cout << "===============" << endl;

    for(int j=0; j<column; j++){
        for(int k=0; k<K; k++){
            cout << Q[k][j] << " ";
        }
        cout << endl;
    }

    cout << "===============" << endl;
    */
    for(int i=0; i<row; i++){
        for(int j=0; j<column; j++){
            cout << dot(i,j) << " ";
        }
        cout << endl;
    }

    cout << "================" << endl;
    
    for(int i=0; i<column; i++){
        cout << gamma[i] << " ";
    }
    cout << endl << "================" << endl;

    for(int i=0; i<column; i++){
        cout << mu[i] << " ";
    }
    cout << endl << "================" << endl;

    now = 20;
    for(int i=0; i<row; i++){
        for(int j=0; j<column; j++){
            double max_pi = maxPI(i,j,now);
            double PIuji = pow(1+max_pi, mu[j]);
            double vuit = dot(i,j)*PIuji*(pow(X[i][j]+1,gamma[j])-pow(X[i][j],gamma[j]));
            
            //cout << i << j << ": "<< "dot:" << dot(i,j) << " " << "maxPI:" << max_pi << " "<< "PI:" <<  PIuji << " " << "d:" << (pow(X[i][j]+1,gamma[j])-pow(X[i][j],gamma[j])) << " " << vuit;
            cout << 1.0/(1+exp(0-vuit)) << " ";
        }
        cout << endl;
    }
}

void PurchaseInterval::load_purchase_interval(){
    
    ifstream in;
    in.open("../../data/purchase_interval");

    d = new int**[row];
    for(int i=0; i<row; i++){
        d[i] = new int*[column];
        for(int j=0; j<column; j++){
            d[i][j] = new int[column];
        }
    }

    for(int i=0; i<row; i++)
        for(int j=0; j<column; j++)
            for(int k=0; k<column; k++){
                d[i][j][k] = -1;
            }

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, " ");
        int u = atoi(secs[0].c_str())-1;
        int i = atoi(secs[1].c_str())-1;
        int j = atoi(secs[2].c_str())-1;
        int duij = atoi(secs[3].c_str());

        d[u][i][j] = duij;
    }
}

void PurchaseInterval::initT(){
    
    T = new int*[row];
    for(int i=0; i<row; i++){
        T[i] = new int[column];
    }

    for(int i=0; i<row; i++)
        for(int j=0; j<column; j++){
            T[i][j] = -1;
        }
}

void PurchaseInterval::initMu(){
    
    mu = new double[column];
    for(int i=0; i<column; i++){
        mu[i] = (double)(rand()%column+1)/column;
    }
}

double PurchaseInterval::maxPI(int u, int i, int t){
    double max_pi = 0.0;
    for(int j=0; j<column; j++){
        int tj = T[u][j];

        if(tj == -1 || d[u][j][i] == -1)
            continue;
    
        double pi = 1.0/(log(abs(t-tj-d[u][j][i])+2)/log(2));

        //cout << t << " " << tj << " " << d[u][j][i] << " " <<  1.0/(log(abs(t-tj-d[u][j][i])+2)/log(2)) << " " << endl;

        if(pi > max_pi)
            max_pi = pi;
    }
    return max_pi;
}

void PurchaseInterval::SVD_pi(int step_count, double lambda1, double lambda2, double lambda3, double beta1, double beta2, double beta3, double gamma0, double mu0, double threshold){
    load_purchase();
    load_purchase_interval();
    initPQ();
    initX();
    initT();
    initGamma();
    initMu();


    for(int step=0; step<step_count; step++){
        for(int index=0; index<H.size(); index++){
            int t = H[index].t;
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int ruit = H[index].ruit;
            if(ruit == 1)
                T[u][i] = t;
            double duit = pow(Xuit+1,gamma[i])-pow(Xuit,gamma[i]);
            double max_pi =  maxPI(u,i,t);
            double PIuji = pow(1+max_pi, mu[i]);
            double vuit = dot(u,i)*duit*PIuji-0;// ci = 0
            double guit = exp(0-vuit*ruit)/(1+exp(0-vuit*ruit))*(0-ruit);

            X[u][i] = Xuit;
            //cout << u << i << ":" << duit << " " << max_pi << " " << PIuji << " " << vuit << " " << guit << endl;

            for(int k=0; k<K; k++){
                double Puk = P[u][k];
                double Qki = Q[k][i];
                P[u][k] = Puk - beta1 * (guit*duit*PIuji*Qki+lambda1*Puk);
                Q[k][i] = Qki - beta1 * (guit*duit*PIuji*Puk+lambda1*Qki);
            }
            gamma[i] = gamma[i] - beta2 * (guit*dot(u,i)*PIuji*(pow(Xuit+1, gamma[i])*log(Xuit+1)-pow(Xuit, gamma[i])*log(Xuit))+lambda2*gamma[i]);
            mu[i] = mu[i] - beta3 * (guit*dot(u,i)*duit*pow(1+max_pi,mu[i])*log(1+max_pi)+lambda3*mu[i]);


       }

        double e = 0.0;
        for(int index=0; index<H.size(); index++){
            int t = H[index].t;
            int u = H[index].u;
            int i = H[index].i;
            int Xuit = H[index].Xuit;
            int ruit = H[index].ruit;
            double duit = pow(Xuit+1,gamma[i])-pow(Xuit,gamma[i]);
            double max_pi =  maxPI(u,i,t);
            double PIuji = pow(1+max_pi, mu[i]);
            double vuit = dot(u,i)*duit*PIuji-0;// ci = 0
            e += log(1+exp(0-vuit*ruit));
            
        }
        for(int i=0; i<column; i++){
            for(int u=0; u<row; u++)
                for(int k=0; k<K; k++){
                    e += (lambda1/2)*(pow(P[u][k],2)+pow(Q[k][i],2));
                }
            e += (lambda2/2)*pow(gamma[i]-gamma0,2);
            e += (lambda3/2)*pow(mu[i]-mu0,2);
        }
        if(e < threshold)
            break;
    }

}
