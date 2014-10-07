#include "sample_product.h"
#include "../common/constants.h"
#include "../common/util.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <stdlib.h>
#include <sstream>
#include <ctime>
#include <sys/timeb.h>

using namespace std;

void SampleProduct::topProduct(char* file, char* dic){
    map<string,int> productDic;

    ifstream in,pdic;
    in.open(file);
    pdic.open(dic);

    char outFile[200];
    ofstream out;
    strcpy(outFile, file);
    out.open(strcat(outFile,"-top7.5w"));

    while(!pdic.eof()){
        string line;
        getline(pdic, line);
        if(line.size() == 0)
            continue;

        productDic[Util::trim(line)]=1;
    }
    pdic.close();

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> secs = Util::split(line, CONTROL_A);
        string productId = secs[2];

        if(productDic.find(productId) != productDic.end())
            out << line << endl;
    }
    in.close();
    out.close();
}
void SampleProduct::randomSample(char* file, int random_scale){

    ifstream in;
    in.open(file);

    char outFile[200];
    ofstream out;
    strcpy(outFile, file);
    out.open(strcat(outFile,"-rand"));


    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;
        
        timeb timebuffer;
        ftime(&timebuffer);
        srand(timebuffer.millitm);
        int randNum = rand()%random_scale;
        if(randNum == 0){
            out << line << endl;
        }

    }
    in.close();
    out.close();
}
