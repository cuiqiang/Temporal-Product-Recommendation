#include "data_preprocessing.h"
#include "../common/util.h"
#include "../common/constants.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;

void DataPreprocessing::indexUserProduct(char* file){
    int user_index = 1;
    int product_index = 1;
    map<string,int> userIndex;
    map<string,int> productIndex;
    long begin_time = 0;

    ifstream in;
    in.open(file);

    ofstream indexOut,userDic,productDic;
    
    char indexFile[200];
    char userDicFile[200];
    char productDicFile[200];

    strcpy(indexFile, file);
    strcpy(userDicFile, file);
    strcpy(productDicFile, file);
   
    indexOut.open(strcat(indexFile,"-id"));
    userDic.open(strcat(userDicFile,"-ud"));
    productDic.open(strcat(productDicFile,"-pd"));

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, CONTROL_A);
        long time = atol(secs[0].c_str());
        string userId = secs[1];
        string productId = secs[2];

        if(begin_time == 0)
            begin_time = time;

        time = (time-begin_time)/(24*60*60*1000);
        
        if(userIndex.find(userId) == userIndex.end()){
            userIndex[userId] = user_index;
            user_index++;
        }
        if(productIndex.find(productId) == productIndex.end()){
            productIndex[productId] = product_index;
            product_index++;
        }

        indexOut << time << SPACE << userIndex[userId] << SPACE << productIndex[productId] << SPACE << secs[4] << endl;
    }
    in.close();
    indexOut.close();

    map<string,int>::iterator it;
    for(it=userIndex.begin();it!=userIndex.end();++it){
        userDic << it->first << TAB << it->second << endl;
    }
    userDic.close();
    for(it = productIndex.begin();it!=productIndex.end();++it){
        productDic << it->first << TAB << it->second << endl;
    }
    productDic.close();
}
void DataPreprocessing::uniq(char* file){
    
    map<string,int> uniqDic;

    ifstream in;
    in.open(file);

    char outFile[200];
    ofstream out;
    strcpy(outFile, file);
    out.open(strcat(outFile,"-uniq"));

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, SPACE);
        string key = secs[0] + SPACE + secs[1] + SPACE + secs[2];
        if(uniqDic.find(key) == uniqDic.end()){
            out << line << endl;
            uniqDic[key] = 1;
        }
    }
    in.close();
    out.close();
}
void DataPreprocessing::dataPreprocessing(char* file){
    indexUserProduct(file);
    char idFile[200];
    strcpy(idFile, file);
    uniq(strcat(idFile, "-id"));
}
