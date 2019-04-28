// thread example 
#include <iostream> // std::cout 
#include <thread> // std::thread 
#include <mutex> // std::mutex, std::lock 

using namespace std;
//using std::thread;
mutex mutey; 

void foo() 
{
    // do stuff... 
    //mutey.lock();
    lock_guard<mutex> lock(mutey);
    for (int i=0; i<100; i++)
    {
        cout << "foo thread. " << i << "\r";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << endl;
    //mutey.unlock();
} 

void bar(int x) 
{
    // do stuff...
    mutey.try_lock();
    cout << "bar thread.  " << endl;
    this_thread::sleep_for(chrono::seconds(3));
    cout << "bar thread exit." << endl;
    // unlock mutey!
    mutey.unlock();
} 

int main() 
{ 
    thread first (foo);         // spawn new thread that calls foo() 
    thread second (bar,0);      // spawn new thread that calls bar(0) 
    
    cout << "main, foo and bar now execute concurrently...\n"; 
    
    // synchronize threads: 
    first.join();                    // pauses until first finishes 
    second.join();                   // pauses until second finishes 
    
    cout << "foo and bar completed.\n"; 
    return 0; 
}