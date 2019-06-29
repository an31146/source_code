#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

vector<int> parseInts(string str) {
	// Complete this function
    vector<int> v;
    unsigned i = 0;
    while (i < str.size())
    {
        unsigned nextComma = str.find(',');
        string strInt = str.substr(i, nextComma);
        
        cout << "parseInt: " << strInt << "\t nextComma: " << nextComma << endl;
        v.push_back(atoi(strInt.c_str()));
        
        cout << "str: " << str << endl;
        i = str.find(2, ',');
        cout << "i: " << i << endl;
        
        if (i == string::npos)
        {
            i++;
            break;
        }
    }
    return v;
}

int main(int argc, char**argv) {
    string str;
    cin >> str;
    vector<int> integers = parseInts(str);
    for(unsigned i = 0; i < integers.size(); i++) {
        cout << integers[i] << "\n";
    }
    
    return 0;
}
