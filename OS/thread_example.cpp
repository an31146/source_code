/*
 * g++ -Wall -O2 -std=c++11 -o thread_example.exe thread_example.cpp -lpthread
 */
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
mutex g_io_mutex;

//int i = 0;

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(3, 10);
    array<int, 100> arr;

void shared_print(string msg, char delim, int id)
{
    lock_guard<mutex> guard(g_io_mutex);
    cout << msg << id << delim;
}

void worker(int id)
{
    //srand(time(nullptr));
    chrono::milliseconds delay{dis(gen)};

    lock_guard<mutex> guard(g_io_mutex);
	cout << "thread #" << setfill('0') << setw(2) << id << ", distribution: " << dis(gen) << endl;
    g_io_mutex.unlock();

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i<1000; i++)
    {
        arr[id]++;
        //cout << arr[id] << " ";
        this_thread::sleep_for(delay);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> diff = end - start;
    //i++;

    //lock_guard<mutex> guard(g_io_mutex);
    g_io_mutex.lock();
    cout << "thread #" << setfill('0') << setw(2) << id;
    cout << " exited after " << setprecision(4) << diff.count() << " secs." << endl;
    g_io_mutex.unlock();
}

int main(int argc, char *argv[]) {
	// your code goes here

    cout << "The name used to start the program: " << argv[ 0 ]
         << "\nArguments are:\n";
    for (int n = 1; n < argc; n++)
    {
        cout << setw( 2 ) << n << ": " << argv[ n ] << '\n';
    }	
    mutex1.lock();
    for (int t = 0; t < 100; t++)
    {
        arr[t] = 0;
        lock_guard<mutex> lock(g_io_mutex);
        threads.emplace_back(thread(worker, t));
        cout << "Thread ID: " << setfill('0') << setw(2) << threads.back().get_id() << endl;
    }
    cout << string(40, '-') << endl;
    //threads.push_back(thread(worker, 1));
	
    //lock_guard<mutex> lock(g_io_mutex);
    for (auto& th : threads)
    {
        th.join();
    }
    //g_io_mutex.unlock();
    //cout << "i: " << i << endl;

    return EXIT_SUCCESS;
}
