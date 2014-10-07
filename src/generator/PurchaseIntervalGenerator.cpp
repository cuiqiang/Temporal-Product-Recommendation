//
//  PurchaseIntervalGenerator.cpp
//  purchase_interval_data
//
//  Created by Eagle on 13-7-13.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#include "PurchaseIntervalGenerator.h"
#include "../common/constants.h"
#include "../common/util.h"
//#include "constants.h"
//#include "util.h"
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <stdlib.h>
#include <math.h>

using namespace std;

PurchaseIntervalGenerator::PurchaseIntervalGenerator(string file){
    productPersonalIntervalResult = new multimap<long, map<long, multimap<long, int> > >;//user{sourceproduct{targetproduct,time}}
    categoryPersonalIntervalResult = new multimap<long, map<long, multimap<long, int> > >;
    
    productIntervalResult = new map<long,multimap<long,string> >;
    
    productTransResult = new map<long,map<long,int> >;
    categoryTransResult = new map<long,map<long,int> >;
    
    productCoocResult = new map<long,map<long,int> >;
    
    //definition of files
    inputFileName = file;
    //cout << inputFileName << endl;
    productPersonalIntervalResultFileName = inputFileName + "-personal-interval-product";
    categoryPersonalIntervalResultFileName = inputFileName + "-personal-interval-category";
    productIntervalResultFileName = inputFileName + "-interval-product";
    productTransResultFileName = inputFileName + "-trans-product";
    categoryTransResultFileName = inputFileName + "-trans-category";
    productCoocResultFileName = inputFileName + "-cooc-product";
}

PurchaseIntervalGenerator::~PurchaseIntervalGenerator(){
    if (productPersonalIntervalResult) {
        delete productPersonalIntervalResult;
    }
    if (categoryPersonalIntervalResult) {
        delete categoryPersonalIntervalResult;
    }
    if (productIntervalResult) {
        delete productIntervalResult;
    }
    if (productTransResult) {
        delete productTransResult;
    }
    if (categoryTransResult) {
        delete categoryTransResult;
    }
    if (productCoocResult) {
        delete productCoocResult;
    }
}

void PurchaseIntervalGenerator::generatePersonalProductInterval(){
    cout << "generate personal-product-interval..." << endl;
    long current_user = 0;//the current user
    vector<struct productPurchaseTime> userPurchase;//a temp vetcor for containing productPurchaseTime of each user
    map<long, int> productTimeMap;//a temp pair for product and time,it helps the product find the nearest influenceing product
    map<long, multimap<long, int> > sProduct;
    multimap<long,int> temp;
    
    //open the input-files
    ifstream inputFile;
    inputFile.open(inputFileName.c_str());
    
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);//read a line
        if(line.size() == 0)
            continue;
        
        vector<string> items = Util::split(line, SPACE);
        int time = atoi(items[0].c_str());
        long userId = atol(items[1].c_str());
        long productId = atol(items[2].c_str());
        
        struct productPurchaseTime productTime;
        productTime.productid = productId;
        productTime.time = time;
        
        if (current_user != userId || inputFile.eof()) {
            if (inputFile.eof()) {
                userPurchase.push_back(productTime);
            }
            for (vector<struct productPurchaseTime>::reverse_iterator i=userPurchase.rbegin(); i!=userPurchase.rend(); i++) {
                struct productPurchaseTime targetProduct = *i;
                for (vector<struct productPurchaseTime>::reverse_iterator j=i+1; j!=userPurchase.rend(); j++) {
                    struct productPurchaseTime sourceProduct = *j;
                    if (sourceProduct.time == targetProduct.time) {//if product i and product j are in one basket
                        continue;
                    }
                    if (productTimeMap.find(sourceProduct.productid) == productTimeMap.end()) {
                         productTimeMap.insert(make_pair(sourceProduct.productid, (targetProduct.time - sourceProduct.time)));
                    }
                }
                for (map<long,int>::iterator it=productTimeMap.begin(); it != productTimeMap.end(); it++) {
                    if (sProduct.find(it->first) == sProduct.end()) {
                        temp.insert(make_pair(targetProduct.productid,it->second));
                        sProduct.insert(make_pair(it->first, temp));
                        temp.clear();
                    }
                    else{
                        sProduct[it->first].insert(make_pair(targetProduct.productid,it->second));
                    }
                    
                }
                productTimeMap.clear();
            }
            if (current_user != 0) {
                productPersonalIntervalResult->insert(make_pair(current_user, sProduct));
            }
            sProduct.clear();
            userPurchase.clear();
            current_user = userId;//reset the current-userid
            cout << current_user << endl;
        }
        userPurchase.push_back(productTime);
    }
    inputFile.close();
}

void PurchaseIntervalGenerator::outputPersonalProductIntervalFile(){
    cout << "Wtriting the personal-product-interval file..." << endl;
    ofstream productPersonalIntervalResultFile;
    productPersonalIntervalResultFile.open(productPersonalIntervalResultFileName.c_str());
    int coCount;//the times a pair-product oc-appearance
    double totalDate;//the sum of date of each pair-product
    string times;//times of pair-product(t1,t2,t3...)
    double avg;
    double variance;
    
    multimap<long, map<long, multimap<long, int> > >::iterator user,userEnd;
    userEnd = productPersonalIntervalResult->end();//calculate once
    for (user=productPersonalIntervalResult->begin(); user!=userEnd; user++) {//for each user
        map<long, multimap<long,int> > sProducts = user->second;
        map<long, multimap<long,int> >::iterator sProduct;
        for (sProduct=sProducts.begin(); sProduct!=sProducts.end(); sProduct++) {//for each source-product
            set<long> keys = getAllKeys(sProduct->second);
            set<long>::iterator key;
            for (key=keys.begin(); key!=keys.end(); key++) {
                multimap<long, int>::size_type entries = sProduct->second.count(*key);
                multimap<long, int>::iterator tProduct = sProduct->second.find(*key);
                coCount=0;
                totalDate=0;
                times="";
                avg=0;
                variance=0;
                for (multimap<int, int>::size_type cnt = 0; cnt != entries; ++cnt,++coCount,tProduct++) {
                    int day;
                    day = tProduct->second;
                    totalDate += day;
                    times += Util::itos(day)+",";
                }
                times = times.substr(0,times.length()-1);
                avg = totalDate/coCount;
                entries = sProduct->second.count(*key);
                tProduct = sProduct->second.find(*key);
                for (multimap<int, int>::size_type cnt = 0; cnt != entries; ++cnt,tProduct++) {
                    int day;
                    day = tProduct->second;
                    variance += pow((day-avg), 2);
                }
                productPersonalIntervalResultFile << user->first << " " << sProduct->first << " " << *key << " " << times << " " << coCount << " " << avg << " " << sqrt(variance/coCount) << endl;
            }
        }
    }
    
    productPersonalIntervalResultFile.close();
    cout << "Writing the personal-product-interval file ok!" <<endl;
    
}


void PurchaseIntervalGenerator::outputPersonalCategoryIntervalFile(){

}

void PurchaseIntervalGenerator::generatePersonalCategoryInterval(){
    
}

void PurchaseIntervalGenerator::generatePersonalInterval(){
    
}


void PurchaseIntervalGenerator::generateProductInterval(){
    cout << "generate product-interval..." << endl;
    multimap<long,string> temp;
    
    //open the files
    ifstream inputFile;
    inputFile.open(productPersonalIntervalResultFileName.c_str());
    //if the personal-product-interval file is not exsit,first generate the personal-product-interval file
    if (!inputFile) {
        cout<< "the personal product interval file is not exsit,first generate the file..." << endl;
        inputFile.close();
        generatePersonalProductInterval();
        outputPersonalProductIntervalFile();
        inputFile.open(productPersonalIntervalResultFileName.c_str());
    }
    
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);
        if(line.size() == 0)
            continue;
        
        vector<string> items = Util::split(line, SPACE);
        int sourceProduct = atoi(items[1].c_str());
        int targetProduct = atoi(items[2].c_str());
        string times = items[3];
        
        if (productIntervalResult->find(sourceProduct) == productIntervalResult->end()) {
            temp.insert(make_pair(targetProduct, times));
            productIntervalResult->insert(make_pair(sourceProduct, temp));
            temp.clear();
        }
        else{
            (*productIntervalResult)[sourceProduct].insert(make_pair(targetProduct, times));
        }
    }
    
    inputFile.close();
}

void PurchaseIntervalGenerator::outputProductIntervalFile(){
    cout << "Wtriting the product-interval file..." << endl;
    ofstream productIntervalResultFile;
    productIntervalResultFile.open(productIntervalResultFileName.c_str());
    string times;
    
    map<long, multimap<long,string> >::iterator sourceProduct;
    for (sourceProduct=productIntervalResult->begin(); sourceProduct!=productIntervalResult->end(); sourceProduct++) {
        set<long> keys = getAllKeys(sourceProduct->second);
        set<long>::iterator key;
        for (key=keys.begin(); key!=keys.end(); key++) {
            multimap<long, string>::size_type entries = sourceProduct->second.count(*key);
            multimap<long, string>::iterator targetProduct = sourceProduct->second.find(*key);
            times="";
            for (multimap<int, string>::size_type cnt = 0; cnt != entries; ++cnt,targetProduct++) {
                string time;
                time = targetProduct->second;
                times += time + ",";
            }
            times = times.substr(0,times.length()-1);
            vector <string> time_item = Util::split(times, COMMA);
            vector<string>::iterator time_it;
            int time_sum = 0;
            int num = 0;
            double avg = 0;
            double variance = 0;
            for (time_it=time_item.begin(); time_it!=time_item.end(); time_it++,num++) {
                time_sum += atoi((*time_it).c_str());
            }
            avg = time_sum / num;
            for (time_it=time_item.begin(); time_it!=time_item.end(); time_it++) {
                variance += pow((atoi((*time_it).c_str()) - avg),2);
            }
            productIntervalResultFile << sourceProduct->first << " " << *key << " " << times << " " << avg << " " << sqrt(variance/num) << endl;
        }
    }
    cout << "Writing the product-interval file ok!" << endl;
}


void PurchaseIntervalGenerator::generateProductTrans(){
    cout << "generate product-transfer..." << endl;
    long current_user = 0;//the current user
    vector<struct productPurchaseTime> userPurchase;//a temp vetcor for containing productPurchaseTime of each user
    map<long, int> productTimeMap;//a temp pair for product and time,it helps the product find the nearest influenceing product
    
    //open the input-files
    ifstream inputFile;
    inputFile.open(inputFileName.c_str());
    
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);//read a line
        if(line.size() == 0)
            continue;
        
        vector<string> items = Util::split(line, SPACE);
        int time = atoi(items[0].c_str());
        long userId = atol(items[1].c_str());
        long productId = atol(items[2].c_str());
        
        struct productPurchaseTime productTime;
        productTime.productid = productId;
        productTime.time = time;
        
        if (current_user != userId || inputFile.eof()) {
            if (inputFile.eof()) {
                userPurchase.push_back(productTime);
            }
            for (vector<struct productPurchaseTime>::reverse_iterator i=userPurchase.rbegin(); i!=userPurchase.rend(); i++) {
                struct productPurchaseTime targetProduct = *i;
                for (vector<struct productPurchaseTime>::reverse_iterator j=i+1; j!=userPurchase.rend(); j++) {
                    struct productPurchaseTime sourceProduct = *j;
                    if (sourceProduct.time == targetProduct.time) {//if product i and product j are in one basket
                        continue;
                    }
                    if (productTimeMap.find(sourceProduct.productid) == productTimeMap.end()) {
                        productTimeMap.insert(make_pair(sourceProduct.productid, (targetProduct.time - sourceProduct.time)));
                    }
                }
                for (map<long,int>::iterator it=productTimeMap.begin(); it != productTimeMap.end(); it++) {
                    if (productTransResult->find(it->first) == productTransResult->end()) {
                        (*productTransResult)[it->first][targetProduct.productid] = 0;
                    }
                    (*productTransResult)[it->first][targetProduct.productid] += 1;
                }
                productTimeMap.clear();
            }
            userPurchase.clear();
            current_user = userId;//reset the current-userid
            cout << current_user << endl;
        }
        userPurchase.push_back(productTime);
    }
    inputFile.close();
   
}

void PurchaseIntervalGenerator::outputProductTransFile(){
    cout << "Wtriting the product-transfer file..." << endl;
    ofstream productTransResultFile;
    productTransResultFile.open(productTransResultFileName.c_str());
    
    map<long, map<long, int> >::iterator source,sourceEnd;
    sourceEnd = productTransResult->end();//calculate once
    for (source=productTransResult->begin(); source!=sourceEnd; source++) {
        map<long,int> target = source->second;
        map<long,int>::iterator it,itEnd;
        itEnd = target.end();
        double sum = 0;
        for (it=target.begin(); it!=itEnd; it++) {
            sum += it->second;
        }
        for (it=target.begin(); it!=itEnd; it++) {
            productTransResultFile << source->first << " " << it->first << " " << (it->second) / sum << endl;
        }
    }
    
    productTransResultFile.close();
    cout << "Writing the product-transfer file ok!" << endl;
}

void PurchaseIntervalGenerator::generateProductCooccurrence(){
    cout << "generate product-coocurrence..." << endl;
    long current_user = 0;//the current user
    vector<struct productPurchaseTime> userPurchase;//a temp vetcor for containing productPurchaseTime of each user
    map<long, int> productTimeMap;//a temp pair for product and time,it helps the product find the nearest influenceing product
    
    //open the input-files
    ifstream inputFile;
    inputFile.open(inputFileName.c_str());
    
    while (!inputFile.eof()) {
        string line;
        getline(inputFile,line);//read a line
        if(line.size() == 0)
            continue;
        
        vector<string> items = Util::split(line, SPACE);
        int time = atoi(items[0].c_str());
        long userId = atol(items[1].c_str());
        long productId = atol(items[2].c_str());
        
        struct productPurchaseTime productTime;
        productTime.productid = productId;
        productTime.time = time;
        
        if (current_user != userId || inputFile.eof()) {
            if (inputFile.eof()) {
                userPurchase.push_back(productTime);
            }
            for (vector<struct productPurchaseTime>::reverse_iterator i=userPurchase.rbegin(); i!=userPurchase.rend(); i++) {
                struct productPurchaseTime targetProduct = *i;
                for (vector<struct productPurchaseTime>::reverse_iterator j=i+1; j!=userPurchase.rend(); j++) {
                    struct productPurchaseTime sourceProduct = *j;
                    /*if (sourceProduct.time == targetProduct.time) {//if product i and product j are in one basket
                        continue;
                    }*/
                    if (productTimeMap.find(sourceProduct.productid) == productTimeMap.end()) {
                        productTimeMap.insert(make_pair(sourceProduct.productid, (targetProduct.time - sourceProduct.time)));
                    }
                }
                for (map<long,int>::iterator it=productTimeMap.begin(); it != productTimeMap.end(); it++) {
                    if (productCoocResult->find(it->first) == productCoocResult->end()) {
                        (*productCoocResult)[it->first][targetProduct.productid] = 0;
                    }
                    (*productCoocResult)[it->first][targetProduct.productid] += 1;
                    if (productCoocResult->find(targetProduct.productid) == productCoocResult->end()) {
                        (*productCoocResult)[targetProduct.productid][it->first] = 0;
                    }
                    (*productCoocResult)[targetProduct.productid][it->first] += 1;
                }
                productTimeMap.clear();
            }
            userPurchase.clear();
            current_user = userId;//reset the current-userid
            cout << current_user << endl;
        }
        userPurchase.push_back(productTime);
    }
    inputFile.close();
    
}

void PurchaseIntervalGenerator::outputProductCooccurrenceFile(){
    cout << "Wtriting the product-coocurrence file..." << endl;
    ofstream productCoocResultFile;
    productCoocResultFile.open(productCoocResultFileName.c_str());
    
    map<long, map<long, int> >::iterator source,sourceEnd;
    sourceEnd = productCoocResult->end();//calculate once
    for (source=productCoocResult->begin(); source!=sourceEnd; source++) {
        map<long,int> target = source->second;
        map<long,int>::iterator it,itEnd;
        itEnd = target.end();
        double sum = 0;
        for (it=target.begin(); it!=itEnd; it++) {
            sum += it->second;
        }
        for (it=target.begin(); it!=itEnd; it++) {
            productCoocResultFile << source->first << " " << it->first << " " << (it->second) / sum << endl;
        }
    }
    
    productCoocResultFile.close();
    cout << "Writing the product-coocurrence file ok!" << endl;
}


set<long> PurchaseIntervalGenerator::getAllKeys(multimap<long, int> p){
    multimap<long, int>::iterator tProduct;
    set<long> keys;
    for (tProduct = p.begin(); tProduct != p.end(); tProduct++) {
        keys.insert(tProduct->first);
    }
    return keys;
}

set<long> PurchaseIntervalGenerator::getAllKeys(multimap<long, string> p){
    multimap<long, string>::iterator tProduct;
    set<long> keys;
    for (tProduct = p.begin(); tProduct != p.end(); tProduct++) {
        keys.insert(tProduct->first);
    }
    return keys;
}
