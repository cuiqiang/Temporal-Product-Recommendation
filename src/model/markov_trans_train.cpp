#include "markov_trans.h"

int main(int argc, char* argv[]){
    MarkovTrans mt;
    mt.train(argv[1]);
}
