/*
 *  g++ -Wall -O2 -std=c++11  -I /usr/lib/arm-linux-gnueabihf/openmpi/include -L/usr/lib/mpich/lib -o scatter scatter.cpp -lmpi -lboost_mpi -lboost_serialization
 */
#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <vector>

namespace mpi = boost::mpi;

using namespace std;
int main(int argc, char* argv[])
{
    mpi::environment env(argc, argv);
    mpi::communicator world;

    srand(time(0) + world.rank());
    vector<int> all;
    int mine = -1;
    if (world.rank() == 0) {
        all.resize(world.size());
        generate(all.begin(), all.end(), rand);
    }
    mpi::scatter(world, all, mine, 0);
    for (int r = 0; r < world.size(); ++r) {
        world.barrier();

        if (r == world.rank()) {
            cout << "Rank " << setfill('0') << setw(2) << r << " got " << setw(10) << mine << '\n';
        }
    }
    return 0;
}
