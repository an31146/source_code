/*
 *  g++ -Wall -O2 -std=c++11 -I/usr/include/mpi -o boost_mpi_communicator boost_mpi_communicator.cpp  -lboost_mpi
 */
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>

using namespace boost::mpi;
using namespace std;

int main(int argc, char **argv)
{
    environment env;
    communicator world;
    cout << "I am process " << world.rank() << " of " << world.size()
              << "." << endl;
    return 0;
}
