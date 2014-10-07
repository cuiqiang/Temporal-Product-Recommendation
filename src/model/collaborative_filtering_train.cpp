#include "collaborative_filtering.h"

int main(int argc, char* argv[]){

    CollaborativeFiltering cf;
    cf.train(argv[1]);
}
