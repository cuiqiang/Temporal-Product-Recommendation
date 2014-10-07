#include "util.h"
#include <sstream>

using namespace std;

vector<string> Util::split (string str, string del){

    string::size_type pos;
    vector<string> result;
    str+=del;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(del,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+del.size()-1;
        }
    }
    return result;
}
string Util::trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}
string Util::itos(int i){
    stringstream sstream;
    sstream << i;
    return sstream.str();
}

string Util::ltos(long i){
    stringstream sstream;
    sstream << i;
    return sstream.str();
}
string Util::ftos(double i){
    stringstream sstream;
    sstream << i;
    return sstream.str();
}
