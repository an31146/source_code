#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

vector<thread> threads;
mutex mutex1;

void worker(int id)
{
    chrono::milliseconds delay{500};
    mutex1.lock();
    this_thread::sleep_for(delay);
	cout << "Worker thread #" << id << endl;
	mutex1.unlock();
}

int main() {
	// your code goes here
	
	for (int t=0; t<20; t++)
	{
	    threads.push_back(thread(worker, t));
	}
	//threads.push_back(thread(worker, 1));
	
	for (auto& th : threads)
    {
        cout << "Thread ID: " << th.get_id() << endl;
        th.join();
    }
	
	return 0;
}
