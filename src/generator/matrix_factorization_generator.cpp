#include "matrix_factorization_generator.h"
#include "../common/constants.h"
#include "../common/util.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <ctime>
#include <sys/timeb.h>

using namespace std;

void MatrixFactorizationGenerator::dataStat(char* file, int k){
    
    ifstream in;
    in.open(file);
    
    K = k;
    data_size = 0;
    map<string, int> userDic;
    map<string, int> productDic;
    map<string, int> timeSplit;
    
    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, SPACE);
        string time = secs[0];
        string userId = secs[1];
        string productId = secs[2];
        
        purchase_record pr;
        pr.t =time;
        pr.u = userId;
        pr.i = productId;
        buffer.push_back(pr); 
        
        userDic[userId] = 1;
        productDic[productId] = 1;
        
        data_size++;
        timeSplit[time] = data_size;
    }
    in.close();

    user_size = userDic.size();
    product_size = productDic.size();

    train_size = 0;
    map<string,int>::iterator itr;
    for(itr=timeSplit.begin();itr!=timeSplit.end();itr++){
       if(itr->second > data_size*9/10){
           train_size = itr->second;
           break;
       }
    }

}

void MatrixFactorizationGenerator::generateTrainTest(char* file, int k){
    
    // train and test file
    char trainFile[200];
    char testFile[200];

    ofstream trainOut, testOut;
    
    strcpy(trainFile, file);
    strcpy(testFile, file);
    
    trainOut.open(strcat(trainFile,"-train"));
    testOut.open(strcat(testFile,"-test"));
    
    // data statistic
    dataStat(file, k);

    // generate training and testing data
    map<string, map<string,int> > matrix;
    map<string, map<string,int> > purchased;//test set
    map<string, int> trainUser;
    map<string, int> trainProduct;
   
    trainOut << user_size << SPACE << product_size << SPACE << K << endl;  
    
    for(int index=0; index<buffer.size(); index++){

        string time = buffer[index].t;
        string userId = buffer[index].u;
        string productId = buffer[index].i;

        if(index < train_size){//generate training data
            
            if(matrix.find(userId) != matrix.end() && matrix[userId].find(productId) != matrix[userId].end()){
                matrix[userId][productId] += 1;
            }else{
                matrix[userId][productId] = 1;
                trainUser[userId] = 1;
                trainProduct[productId] = 1;
            }

        }else{//generate test data
            purchased[userId][productId] = 1;
        } 
    }

    map<string, map<string,int> >::iterator itr1;
    map<string, int>::iterator itr2;

    for(itr1=matrix.begin();itr1!=matrix.end();itr1++){
        for(itr2=matrix[itr1->first].begin();itr2!=matrix[itr1->first].end();itr2++){
            trainOut << itr1->first << SPACE << itr2->first << SPACE << itr2->second << endl;
        }
    }
    
    int random_scale = 1000;
    for(int i=0; i<user_size; i++){
        for(int j=0; j<product_size; j++){

            if(matrix.find(Util::itos(i)) == matrix.end() || matrix[Util::itos(i)].find(Util::itos(j)) == matrix[Util::itos(i)].end()){
                timeb timebuffer;
                ftime(&timebuffer);
                srand(timebuffer.millitm);

                int randNum = rand()%random_scale;
                
                if(randNum == 0){
                    trainOut << i << SPACE << j << SPACE << "0" << endl;
                }
            }
        }  
    }

    cout << "train finish!" << endl;

    for(itr1=purchased.begin();itr1!=purchased.end();itr1++){
        
        map<string, int> p;//identify re-purchase 
        int userInTrain = 0;
        int re_purchase = 0;
        
        if(trainUser.find(itr1->first) != trainUser.end()){
            userInTrain = 1;
        }
        
        string xuit = "";
        for(itr2=matrix[itr1->first].begin();itr2!=matrix[itr1->first].end();itr2++){
            xuit += itr2->first + SIMICOLON + Util::itos(itr2->second) + SPACE;
            p[itr2->first] = 1;//already purchase list
        }

        string purchased_list = "";
        for(itr2=purchased[itr1->first].begin();itr2!=purchased[itr1->first].end();itr2++){
            if(trainProduct.find(itr2->first) != trainProduct.end()){
                purchased_list += itr2->first + SIMICOLON + "1" + SPACE;
            }else{
                purchased_list += itr2->first + SIMICOLON + "0" + SPACE;
            }
            if(p.find(itr2->first) != p.end()){
                re_purchase = 1;
            }
        }
        if(purchased_list.size() != 0){
            testOut << userInTrain <<  re_purchase << TAB << itr1->first << COMMA << xuit << COMMA << purchased_list << endl;
        }
    }
    trainOut.close();
    testOut.close();
}

void MatrixFactorizationGenerator::generate(char* file, int k){
    generateTrainTest(file, k);
}
