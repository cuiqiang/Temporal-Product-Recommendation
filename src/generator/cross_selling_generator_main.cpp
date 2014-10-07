#include "cross_selling_generator.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){

    CrossSellingGenerator cross_gen;
    cross_gen.generate(argv[1],atoi(argv[2]));
}
