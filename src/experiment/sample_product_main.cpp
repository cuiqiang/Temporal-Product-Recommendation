#include "sample_product.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){

    SampleProduct sp;
    if(strcmp(argv[1],"-top") == 0){
        sp.topProduct(argv[2],argv[3]);
    }else if(strcmp(argv[1], "-rand") == 0){
        sp.randomSample(argv[2],atoi(argv[3]));
    }
}
