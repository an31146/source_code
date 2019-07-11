/*
 *  g++ -Wall -O2 -std=c++11 -I /usr/lib/arm-linux-gnueabihf/openmpi/include -o hello_world hello_world.cpp -lmpi -lboost_mpi
 */
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iomanip>
#include <iostream>

using namespace boost::mpi;
using namespace std;

int main()
{
    environment env;
    communicator world;
    cout << "I am process " << setw(3) << world.rank() << " of " << setw(3) << world.size() << "." << endl;
    return 0;
}
