#include "evaluation.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    Evaluation e;
    for(int k=1; k<11; k++){
        e.evaluate(argv[1], k, argv[2], argv[3]);
    }
    e.getTotal();
}
