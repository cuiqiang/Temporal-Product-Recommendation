#include "collaborative_filtering_generator.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){

    CollaborativeFilteringGenerator cf_gen;
    cf_gen.generate(argv[1]);
}
