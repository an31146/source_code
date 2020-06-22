/*
 * g++ -Wall -O2 -std=c++11 -IC:\Tools\boost_1_69_0
 *                          -IC:\Tools\libtommath-1.1.0 
 *                          -o tommath_factorial.exe tommath_factorial.cpp 
 *                          -ltommath
 */
#include <boost/multiprecision/tommath.hpp>
#include <iostream>

using namespace std;

int main()
{
    boost::multiprecision::tom_int v = 1;

    // Do some arithmetic:
    for(unsigned i = 1; i <= 1000; ++i)
        v *= i;

    cout << v << endl << endl; // prints 1000!
    cout << hex << v << endl << endl; // prints 1000! in hex format
	
	cout << "Press Enter: ";
	getchar();
	
	// Do more arithmetic
	for(unsigned i = 1001; i <= 2000; i++)
		v *= i;

    cout << endl << dec << v << endl << endl; // prints 2000!
    cout << hex << v << endl << endl; // prints 2000! in hex format

    try {
        cout << hex << -v << endl; // Ooops! can't print a negative value in hex format!
    }
    catch(const runtime_error& e)
    {
        cout << e.what() << endl;
    }

    try {
        // v is not a 2's complement type, bitwise operations are only supported
        // on positive values:
        v = -v & 2;
    }
    catch(const runtime_error& e)
    {
        cout << e.what() << endl;
        cout << "Okay, then." << endl;
    }

    return 0;
}
