#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std;

mutex mu;

void shared_print(string msg, char delim, int id)
{
    lock_guard<mutex> guard(mu);
    cout << msg << id << delim;
}

void function_1(int t)
{
    for (int i = 0; i >= -1000; i--)
        shared_print(string("From t") + to_string(t) + string(": "), '\t', i);
}

int main()
{
    std::thread t1(function_1, 1);
    std::thread t2(function_1, 2);
    std::thread t3(function_1, 3);
    std::thread t4(function_1, 4);
    
    for (int i = 0; i < 100; i++)
        shared_print(string("From main: "), '\n', i);
        
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    return EXIT_SUCCESS;
}
