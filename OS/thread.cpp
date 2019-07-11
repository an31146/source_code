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
    mutey.try_lock();
    lock_guard<mutex> lock(mutey);
    //this_thread::sleep_for(chrono::milliseconds(10));
    for (int i=0; i<100; i++)
    {
        cout << "\t\t\t\tfoo thread. " << i << "\r";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    cout << endl << endl;
    //mutey.unlock();
} 

void bar(int x) 
{
    // do stuff...
    mutey.try_lock();
    cout << "bar thread.  \n";
    this_thread::sleep_for(chrono::seconds(2));
    cout << "bar thread exit.\n";
    // unlock mutey!
    mutey.unlock();
} 

int main() 
{ 
    mutey.lock();
    thread first (foo);         // spawn new thread that calls foo() 
    thread second (bar,0);      // spawn new thread that calls bar(0) 
    
    cout << "main, foo and bar now execute concurrently...\n"; 
    mutey.unlock();
    
    // synchronize threads: 
    first.join();                    // pauses until first finishes 
    second.join();                   // pauses until second finishes 
    
    cout << "foo and bar completed.\n"; 
    return 0; 
}
