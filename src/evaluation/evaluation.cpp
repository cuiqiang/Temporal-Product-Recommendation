#include "evaluation.h"
#include "../common/util.h"
#include "../common/constants.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <algorithm>

using namespace std;

void Evaluation::evaluate(char*file, int K, char* uflag, char* pflag){

    ifstream in;
    in.open(file);
    
    // variable init
    double precision = 0.0;
    double recall = 0.0;
    int conv = 0;
    total = 0;
    double map_k = 0.0;
    int uflag_size = 2;
    int pflag_size = 1;

    while(!in.eof()){
        string line;
        getline(in, line);
        if(line.size() == 0)
            continue;

        vector<string> cols = Util::split(line, TAB);
        vector<string> secs = Util::split(cols[1], COMMA);
        vector<string> target = Util::split(Util::trim(secs[2]), SPACE);
        vector<string> predict = Util::split(Util::trim(secs[3]), SPACE);
        
        map<string,int> target_map;
        for(int i=0;i<target.size();i++){
            vector<string> v = Util::split(target[i], SIMICOLON);
            if(pflag[0] == STAR || strcmp(v[1].c_str(),pflag) == 0){
                target_map[v[0]] = 1;
            }
        }
        bool u_match = true;
        for(int i=0; i<uflag_size; i++){
            if(uflag[i] != STAR && cols[0].c_str()[i] != uflag[i]){
                u_match = false;
            }
        }
        if(target_map.size() == 0 || !u_match){
            continue;
        }

        total++;
        int common = 0;
        for(int j=0;j<K;j++){
            if(target_map.find(predict[j]) != target_map.end()){
                common++;
            }
        }
        if(common > 0){
            conv++;
            precision += common*1.0/K;
            recall += common*1.0/target_map.size();
        }
        map_k += MAP(K, target, predict);

    }

    cout << K << SIMICOLON << conv*1.0/total << TAB << precision/total << TAB << recall/total << TAB << map_k/total << endl; 
}
void Evaluation::getTotal(){
    cout << "variable data size: " << total << endl;
}
double Evaluation::MAP(int k, vector<string> t, vector<string> p){
    double count = 0.0;
    double map = 0.0;
    int real_count = t.size();
    vector<string> target,temp;
    vector<string>::iterator result;
    for(int i=0;i<t.size();i++){
        temp = Util::split(t[i], SIMICOLON);
        target.push_back(temp[0]);
    }
    if(k>p.size())
        k = p.size();
    for(int i=1;i<=k;i++){
        result = find(target.begin(),target.end(),p[i-1]);
        if(result != target.end()){
            count += 1;
            map += count/i;
        }
    }
    map = map/real_count;
    return map;
}
