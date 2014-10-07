#include "collaborative_filtering.h"

int main(int argc, char* argv[]){

    CollaborativeFiltering cf;
    cf.predict(argv[1], argv[2]);
}
