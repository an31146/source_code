#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <iostream>

using namespace boost::mpi;
using namespace std;

int main(int argc, char **argv)
{
    //environment env;
    environment env(argc, argv);
    communicator world;
    cout << "I am process " << world.rank() << " of " << world.size()
              << "." << endl;
    return 0;
}
