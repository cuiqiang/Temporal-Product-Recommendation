#include "dataset_analysis.h"
#include "../common/util.h"
#include "../common/constants.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;


DataSetAnalysis::DataSetAnalysis(char* f){
    file = f;
}

void DataSetAnalysis::countDataSet(){
    map<string, int> userIndex;
    map<string, int> productIndex;
    map<string, int> userProductIndex;

    ifstream in;
    in.open(file);

    
    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, SPACE);
        string userId = secs[1];
        string productId = secs[2];

        if(userIndex.find(userId) == userIndex.end()){
            userIndex[userId] = 1;
        }
        if(productIndex.find(productId) == productIndex.end()){
            productIndex[productId] = 1;
        }
        if(userProductIndex.find(userId+","+productId) == userProductIndex.end()){
            userProductIndex[userId+","+productId] = 1;
        }
    }
    cout << "user size: " << userIndex.size() << endl;
    cout << "product size: " << productIndex.size() << endl;
    cout << "user-product pair: " << userProductIndex.size() << endl;
}
