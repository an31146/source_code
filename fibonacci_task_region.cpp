/*
 * $ g++ -Wall -O2 -std=c++11 -LC:\Tools\boost_1_69_0\stage\lib -o fibonacci_task_region.exe fibonacci_task_region.cpp -llibboost_thread-mgw81-mt-x64-1_69
 */
#include <iostream>

#define BOOST_THREAD_PROVIDES_FUTURE

#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/csbl/vector.hpp>
#include <boost/thread/experimental/task_region.hpp>

// Horribly recursive and huge memory consumption!
int fib_task_region(int n)
{
    using boost::experimental::parallel::task_region;
    using boost::experimental::parallel::task_region_handle;

    if (n == 0) return 0;
    if (n == 1) return 1;

    int n1 = 0;
    int n2 = 0;

    task_region([&](task_region_handle& trh)
    {
        trh.run([&]
        {
            n1 = fib_task_region(n - 1);
        });

        n2 = fib_task_region(n - 2);
    });

    return n1 + n2;
}

int main()
{
    for (int i = 0; i < 25; i++) {
        std::cout << fib_task_region(i) << " ";
    }
    std::cout << std::endl;
}