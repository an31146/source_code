/*
 *  g++ -Wall -O2 -std=c++11  -I /usr/lib/arm-linux-gnueabihf/openmpi/include -o point-to-point point-to-point.cpp -lmpi -lboost_mpi -lboost_serialization
 */
#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>

using namespace boost::mpi;
using namespace std;

int main()
{
    environment env;
    communicator world;

    if (world.rank() == 0) {
        cout << "world.size(): " << world.size() << endl;
	
        string msg;
	for (int r=1; r<world.size(); r++)
	{
	    string str_rank = string("[") + to_string(r) + string("] Hello");
	    world.send(r, 0, str_rank);
            world.recv(r, 1, msg);
            cout << msg << endl;
	}
    } else {
        string msg;
        world.recv(0, 0, msg);
        cout << msg;
        cout.flush();
        world.send(0, 1, string(" world!"));
    }

    return 0;
}
