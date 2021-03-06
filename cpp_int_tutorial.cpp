/*
 * g++ -Wall -O2 -std=c++11 -IC:\Tools\boost_1_69_0 -o cpp_int_tutorial.exe cpp_int_tutorial.cpp
 */
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <string>

using namespace boost::multiprecision;
using namespace std;

void do_factorial(unsigned n)
{
    // Repeat at arbitrary precision:
    cpp_int u = 1;
    for(unsigned i = 1; i <= n; ++i)
      u *= i;

    ostringstream oss;
    oss << u;
    string fact = u.str();

    if (fact.length() > 205) {
        cout << n << "! = " << fact.substr(0, 100)
                            << "..." << fact.substr(fact.length()-100, 100)
                            << endl << endl;
    } 
    else {
        cout << n << "! = " << fact << endl << endl;
    }
    cout << "Press Enter: ";
    cout.flush();
    getc(stdin);
    cout << endl;
}

void do_double_factorial(unsigned n)
{
    // Repeat at arbitrary precision:
    cpp_int u = 1;
    if (n % 2 == 0)
    {
        for(unsigned i = 2; i <= n; i += 2)
            u *= i;
    }
    else 
    {
        for(unsigned i = 1; i <= n; i += 2)
            u *= i;
    }

    ostringstream oss;
    oss << u;
    string double_fact = u.str();
    
    if (double_fact.length() > 205) {
        cout << n << "!! = "    << double_fact.substr(0, 100)
                                << "..." << double_fact.substr(double_fact.length()-100, 100)
                                << endl << endl;
    } 
    else {
        cout << n << "!! = "  << double_fact << endl << endl;
    }
    cout << "Press Enter: ";
    cout.flush();
    getc(stdin);
    cout << endl;
}

int main()
{
    int128_t v = 1;

    // Do some fixed precision arithmetic:
    for(unsigned i = 1; i <= 20; ++i)
        v *= i;
    // prints 2432902008176640000 (i.e. 20!)
    cout << "20! = " << v << endl << endl;

    // prints 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000 (i.e. 100!)
    do_factorial(100);

    // prints 122013682599111006870123878542304692625357434280319284219241358838584537315388199760549644750220328186301361647714820358
    //        416337872207817720048078520515932928547790757193933060377296085908627042917454788242491272634430567017327076946106280231
    //        045264421887878946575477714986349436778103764427403382736539747138647787849543848959553753799042324106127132698432774571
    //        554630997720278101456108118837370953101635632443298702956389662891165897476957208792692887128178007026517450776841071962
    //        439039432253642260523494585012991857150124870696156814162535905669342381300885624924689156412677565448188650659384795177
    //        536089400574523894033579847636394490531306232374906644504882466507594673586207463792518420045936969298102226397195259719
    //        094521782333175693458150855233282076282002340262690789834245171200620771464097945611612762914595123722991334016955236385
    //        094288559201872743379517301458635757082835578015873543276888868012039988238470215146760544540766353598417443048012893831
    //        389688163948746965881750450692636533817505547812864000000000000000000000000000000000000000000000000000000000000000000000
    //        0000000000000000000000000000000000000000000000000000000 (i.e. 500!)
    do_factorial(500);
    do_double_factorial(500);
    do_double_factorial(501);

    do_factorial(5000);
    do_double_factorial(5000);
    do_double_factorial(5001);

    do_factorial(100000);
    do_double_factorial(100000);
    do_double_factorial(100001);

    do_factorial(200000);
    do_double_factorial(200000);
    do_double_factorial(200001);

    return 0;
}
