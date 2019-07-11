#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <math.h>

using namespace std;

unsigned isprime(unsigned long n) {
    if (n<2 || ((n != 2) && (n&1) == 0) )
        return 0;
    else 
        if (n == 2 || n==3 || n==5 || n==7 || n==11)
            return 1;
        
    for (unsigned i=3; i<=sqrt(n); i+=2) {
        if (n%i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    string input;
    unsigned long cases, i, j, n, count = 0;
    char *st;

    getline (cin, input);
    cases = strtoul(input.c_str(), NULL, 0);

    for (unsigned c=0; c<cases; c++)
    {
        getline (cin, input);
        i = strtoul(input.c_str(), &st, 0);
        j = strtoul(st, NULL, 0);
        
        if (j>0 && j>i)
            for (n=i; n<=j; n++)
	        if (isprime(n))
		{
                    cout << setw(12) << n;
		    count++;
		}
                
        cout << endl << count << endl;
    }
    return 0;
}
