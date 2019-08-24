/*
 *  g++ -Wall -O2 -std=c++11  -I /usr/lib/arm-linux-gnueabihf/openmpi/include -L/usr/lib/mpich/lib -o gather gather.cpp -lmpi -lboost_mpi -lboost_serialization
 */
#include <boost/mpi.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdlib>
namespace mpi = boost::mpi;

using namespace std;
int main()
{
    mpi::environment env;
    mpi::communicator world;

    srand(time(0) + world.rank());
    int32_t my_number = rand() & 0xff;
    if (world.rank() == 0) {
        vector<int> all_numbers;
        gather(world, my_number, all_numbers, 0);
        for (int proc = 0; proc < world.size(); ++proc)
            cout << "Process #" << setfill('0') << setw(2) << proc << " thought of " << setw(3) << all_numbers[proc] << endl;
    } else {
            gather(world, my_number, 0);
    }

    return 0;
}
