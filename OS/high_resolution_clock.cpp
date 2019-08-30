#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
 
volatile int sink;

using namespace std;

int main()
{
    for (auto size = 1000ull; size <= 1000000000ull; size *= 1000ull)
    {
        // record start time
        auto start = chrono::high_resolution_clock::now();
        // do some work
        vector<int> v(size, 42u);
        sink = accumulate(v.begin(), v.end(), 0u); // make sure it's a side effect
        // record end time
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> diff = end-start;
        cout << "Time to fill and iterate a vector of " 
             << size << " ints : " << diff.count() << " s\n";
    }
}
