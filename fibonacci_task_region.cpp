#include <iostream>
#include <boost/thread/future.hpp>
#include <boost/thread/csbl/vector.hpp>
#include <boost/thread/experimental/parallel/v2/task_region.hpp>

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

int main()
{
    for (int i = 0; i < 10; i++) {
        std::cout << fib_task_region(i) << " ";
    }
    std::cout << std::endl;
}