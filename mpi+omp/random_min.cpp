/*
 * g++ -Wall -O2 -std=c++11  -I /usr/lib/arm-linux-gnueabihf/openmpi/include -o random_min random_min.cpp -lmpi -lboost_mpi
 */
#include <boost/mpi.hpp>
#include <iostream>
#include <cstdlib>
namespace mpi = boost::mpi;

using namespace std;

int main()
{
    mpi::environment env;
    mpi::communicator world;
    int rank = world.rank();

    srand(time(0) + rank);
    int my_number = rand() % 1000 * (rank+1);

    cout << my_number << endl;

    if (rank == 0) {
        int minimum;
        reduce(world, my_number, minimum, mpi::minimum<int>(), 0);
        cout << endl << "The minimum value is " << minimum << endl;
    }
    else
    {
        reduce(world, my_number, mpi::minimum<int>(), 0);
    }
    if (rank == 1)
    {
        int maximum;
        reduce(world, my_number, maximum, mpi::maximum<int>(), 1);
        cout << "The maximum value is " << maximum << endl;
    }
    else
    {
        reduce(world, my_number, mpi::maximum<int>(), 1);
    }
    return 0;
}
