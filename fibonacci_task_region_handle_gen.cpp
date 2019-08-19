/*
 * $ g++ -Wall -O2 -std=c++11 -L/c/Users/rhammond/Documents/boost_1_69_0/stage/lib -o fibonacci_task_region_handle_gne.exe fibonacci_task_region_handle_gen.cpp -llibboost_thread-mgw49-mt-x64-1_69
 */
#include <iostream>

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_EXECUTORS
#define BOOST_THREAD_TASK_REGION_HAS_SHARED_CANCELED

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

  int n1;
  int n2;

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

template <class Ex>
int fib_task_region_gen( Ex& ex, int n)
{
  using boost::experimental::parallel::task_region;
  using boost::experimental::parallel::task_region_handle_gen;

  if (n == 0) return 0;
  if (n == 1) return 1;

  int n1 = 0;
  int n2 = 0;

  task_region(ex, [&](task_region_handle_gen<Ex>& trh) // (2)
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
  boost::basic_thread_pool tp; // (1)
  for (int i = 0; i < 20; ++i) {
    std::cout << fib_task_region_gen(tp, i) << " ";
  }
  std::cout << std::endl;
  return 0;
}