#include <iomanip>
#include <iostream>
#include <chrono>
#include <mutex>
#include <random>
#include <thread>
#include <vector>
#include <array>

using namespace std;

vector<thread> threads;
mutex mutex1;
int i = 0;
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(10, 50);
    array<int, 100> arr;

void worker(int id)
{
    //srand(time(nullptr));
    chrono::milliseconds delay{dis(gen)};
    mutex1.lock();
	cout << "Worker thread #" << id << ", " << dis(gen) << "\t";
    mutex1.unlock();
    auto start = chrono::high_resolution_clock::now();
    for (int i=0; i<1000; i++)
    {
        arr[id]++;
        //cout << arr[id] << " ";
        this_thread::sleep_for(delay);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> diff = end-start;
    //i++;
    mutex1.lock();
    cout << "Worker thread #" << id;
    cout << " exited after " << diff.count() << " secs." << endl;
    mutex1.unlock();
}

int main(int argc, char *argv[]) {
	// your code goes here

    cout << "The name used to start the program: " << argv[ 0 ]
         << "\nArguments are:\n";
    for (int n = 1; n < argc; n++)
        cout << setw( 2 ) << n << ": " << argv[ n ] << '\n';
 	
	for (int t=0; t<100; t++)
	{
        arr[t] = 0;
	    threads.push_back(thread(worker, t));
	}
	//threads.push_back(thread(worker, 1));
	
    for (auto& th : threads)
    {
        cout << "Thread ID: " << th.get_id() << "\t";
        th.join();
    }
    //cout << "i: " << i << endl;

	return 0;
}
