//
//  main.cpp
//  purchase_interval_data
//
//  Created by Eagle on 13-7-13.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#include <iostream>
#include "PurchaseIntervalGenerator.h"

using namespace std;

int main(int argc, const char * argv[])
{
    PurchaseIntervalGenerator purchaseInterval("../data/purchase_interval_data/purchase-40-55-1year-top7.5w/purchase-40-55-1year-top7.5w-sort-id-uniq-train-sort");
    //purchaseInterval.generatePersonalProductInterval();
    //purchaseInterval.outputPersonalProductIntervalFile();
    //purchaseInterval.generateProductTrans();
    //purchaseInterval.outputProductTransFile();
    //purchaseInterval.generateProductInterval();
    //purchaseInterval.outputProductIntervalFile();
    purchaseInterval.generateProductCooccurrence();
    purchaseInterval.outputProductCooccurrenceFile();
}

