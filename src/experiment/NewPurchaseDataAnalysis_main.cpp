//
//  main.cpp
//  new_purchase_data_anaysis
//
//  Created by Eagle on 13-7-24.
//  Copyright (c) 2013年 Eagle. All rights reserved.
//

#include "NewPurchaseDataAnalysis.h"
#include <iostream>

int main(int argc, const char * argv[])
{
    NewPurchaseDataAnalysis analysis("new","../data/one_hop/purchase-40-55-1year-top7.5w-sort-id-uniq-test","../data/one_hop/purchase-40-55-1year-top7.5w-sort-id-uniq-train.model");
    analysis.loadTransferModel();
    for (int i=0; i<=10; i++) {
        analysis.setControlThreshold(i);
        cout << i << ":" << endl;
        analysis.predictFromHistoryAnalysis();
        analysis.resetValues();
    }
    
    return 0;
}

