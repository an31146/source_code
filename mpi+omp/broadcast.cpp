/*
 * g++ -Wall -O2 -std=c++11  -I /usr/lib/arm-linux-gnueabihf/openmpi/include -L/usr/lib/mpich/lib -o broadcast broadcast.cpp -lmpi -lboost_mpi -lboost_serialization
 */ 
#include <boost/mpi.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;

using namespace std;
int main()
{
    mpi::environment env;
    mpi::communicator world;

    string value;
    if (world.rank() == 0) {
        value = "\"Hello, World!\"";
    }

    broadcast(world, value, 0);

    cout << "Process #" << setfill('0') << setw(2) << world.rank() << " says " << value << endl;
    return 0;
}
