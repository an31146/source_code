#include <sstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

vector<int> parseInts(string str) {
	// Complete this function
    vector<int> v;
    unsigned i = 0;
    int nextComma = 0;
	while (nextComma > -1)
    {
        nextComma = str.find(',');
        string strInt = str.substr(i, nextComma);
		str = str.substr(nextComma + 1);
        
        cout << "parseInt: " << strInt << "\nnextComma: " << nextComma << endl;
        v.push_back(atoi(strInt.c_str()));
        
        cout << "str: " << str << endl << endl;
        //i = str.find(',', nextComma + 1);
		//if (i > 0)
			// nextComma = i;
        //cout << "i: " << i << endl;
        
		/*
        if (i == string::npos)
        {
            i++;
            break;
        }*/
    }
    return v;
}

int main(int argc, char**argv) {
    string str = " 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97";
    //cin >> str;
    vector<int> integers = parseInts(str);
    for(unsigned i = 0; i < integers.size(); i++) {
        cout << "integers[" << i << "]: " << integers[i] << endl;
    }
    
    return 0;
}
