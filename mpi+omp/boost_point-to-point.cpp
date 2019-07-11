/*
 *  g++ -Wall -O2 -std=c++11 -I/usr/include/mpi -o boost_point-to-point boost_point-to-point.cpp  -lboost_mpi -lboost_serialization -lmpi
 *
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
        world.send(1, 0, string("Hello"));

        string msg;
	world.recv(1, 1, msg);

        cout << msg << "!" << endl;
    }
    else {
        string msg;
        
	world.recv(0, 0, msg);
        
	cout << msg << ", ";
        cout.flush();
        
	world.send(0, 1, string("world"));
    }

    return 0;
}
