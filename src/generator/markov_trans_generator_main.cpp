#include "markov_trans_generator.h"

using namespace std;

int main(int argc, char* argv[]){

    MarkovTransGenerator mt_gen;
    mt_gen.generate(argv[1]);
}
