#include "top_pop_generator.h"
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

void TopPopGenerator::generateTrainTest(char* file){
    
    // file open
    ifstream in;
    in.open(file);

    char trainFile[200];
    char testFile[200];// user with at least one purchase in the training data

    ofstream trainOut, testOut;
    
    strcpy(trainFile, file);
    strcpy(testFile, file);
    
    trainOut.open(strcat(trainFile,"-train"));
    testOut.open(strcat(testFile,"-test"));
    

    //statistic
    data_size = 0;
    map<string, int> userDic;
    map<string, int> productDic;
    map<string, int> timeSplit;
    
    vector<purchase_record> buffer;
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

    trainOut << userDic.size() << SPACE << productDic.size() << endl;

    int train_size = 0;
    map<string,int>::iterator itr;
    for(itr=timeSplit.begin();itr!=timeSplit.end();itr++){
       if(itr->second > data_size*9/10){
           train_size = itr->second;
           break;
       }
    }

    //generate training and testing data
    map<string, map<string,int> > matrix;
    map<string, map<string,int> > purchased;//test set
    map<string, int> trainUser;
    map<string, int> trainProduct;

    for(int index=0; index<buffer.size(); index++){

        string time = buffer[index].t;
        string userId = buffer[index].u;
        string productId = buffer[index].i;
                
        if(index < train_size){//generate training data
            
            trainUser[userId] = 1;
            trainProduct[productId] = 1;
  
            if(matrix.find(userId) != matrix.end() && matrix[userId].find(productId) != matrix[userId].end()){
                matrix[userId][productId] += 1;
            }else{
                matrix[userId][productId] = 1;
                
            }
            trainOut << time << SPACE << userId << SPACE << productId << endl;
        }else{//generate test data
            purchased[userId][productId] = 1;
        } 
    }

    map<string, map<string,int> >::iterator itr1;
    map<string, int>::iterator itr2;
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

void TopPopGenerator::generate(char* file){
    generateTrainTest(file);
}
