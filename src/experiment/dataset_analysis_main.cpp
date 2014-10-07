#include "dataset_analysis.h"
#include <iostream>

int main(int argc, char* argv[]){
    if(argc < 2)
        std::cout << "input the file path" << std::endl;

    DataSetAnalysis dsa(argv[1]);
    dsa.countDataSet();
}
