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
    //lock_guard<mutex> lock(mutey);
    mutey.try_lock();
    this_thread::sleep_for(chrono::seconds(1));
    cout << endl << "foo's thread id: " << this_thread::get_id() << endl;
    //cout << thread::get_id() << endl;
    for (int i=0; i<100; i++)
    {
        //cout << "foo thread. " << i << "\r";
        //cout.flush();
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    cout << endl;
    mutey.unlock();
} 

void bar(int x) 
{
    // do stuff...
    
    mutey.try_lock();
    this_thread::sleep_for(chrono::seconds(1));
    cout << endl << "bar's thread id:  " << this_thread::get_id() << endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout << "bar thread exit." << endl;
    // unlock mutey!
    mutey.unlock();
} 

int main() 
{ 
    lock_guard<mutex> lock(mutey);
    thread first (foo);         // spawn new thread that calls foo() 
    thread second (bar, 0);      // spawn new thread that calls bar(0) 
    std::thread::id t1_id = first.get_id();
    std::thread::id t2_id = second.get_id();
    
    cout << "first's thread id: " << t1_id << endl;
    cout << "second's thread id: " << t2_id << endl;
    cout << "main, foo and bar now execute concurrently...\n"; 
    mutey.unlock();
    
    // synchronize threads: 
    first.join();                    // pauses until first finishes 
    second.join();                   // pauses until second finishes 
    
    cout << "foo and bar completed.\n"; 
    return 0; 
}