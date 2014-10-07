#include "marginal_net_utility_generator.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){

    MarginalNetUtilityGenerator util_gen;
    util_gen.generate(argv[1],atoi(argv[2]));
}
