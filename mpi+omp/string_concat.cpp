/*
 *  g++ -Wall -O2 -std=c++11  -I /usr/lib/arm-linux-gnueabihf/openmpi/include -o string_concat string_concat.cpp -lmpi -lboost_mpi -lboost_serialization
 */
#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;

using namespace std;

int main()
{
    mpi::environment env;
    mpi::communicator world;

    string names[12] = { "zero ", "one ", "two ", "three ",
                         "four ", "five ", "six ", "seven ",
                         "eight ", "nine ", "ten ", "eleven " };

    string result;
    reduce(world, world.rank()<12 ? names[world.rank()] : string("many "),
           result, plus<string>(), 0);

    if (world.rank() == 0)
        cout << "The result is " << result << endl;

    return 0;
}
