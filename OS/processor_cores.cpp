/*
 * g++ -Wall -O2 -std=c++11 -o processor_cores processor_cores.cpp -lpthread
 */
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#elif defined __linux__
#include <pthread.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#endif

#include <iostream>
#include <chrono>

#define MAX_CORES 4

using namespace std;

typedef std::chrono::high_resolution_clock Clock;


#ifdef _WIN32
struct Core
{
    int CoreNumber;
};

static void startMonitoringCoreSpeeds(void *param)
{
    Core core = *((Core *)param);
    SetThreadAffinityMask(GetCurrentThread(), 1 << core.CoreNumber);
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    while (true)
    {
        LARGE_INTEGER first;
        LARGE_INTEGER second;
        QueryPerformanceCounter(&first);
        Sleep(1000);
        QueryPerformanceCounter(&second);
        cout << "Core " << core.CoreNumber << " has frequency " << ((float)(second.QuadPart - first.QuadPart)/frequency.QuadPart) << " GHz" << endl;
    }
}

int GetNumberOfProcessorCores()
{
    DWORD process, system;
    if (GetProcessAffinityMask(GetCurrentProcess(), &process, &system))
    {
        int count = 0;
        for (int i = 0; i < 32; i++)
        {
            if (system & (1 << i))
            {
                count++;
            }
        }
        return count;
    }
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
#elif defined __linux__
static void startMonitoringCoreSpeeds(void *param)
{
    while (true)
    {
        auto t1 = Clock::now();
	usleep(1000000);
        auto t2 = Clock::now();
        std::cout << "Delta t2-t1: " 
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
                  << " nanoseconds" << std::endl;
    }
}

int GetNumberOfProcessorCores()
{
    return get_nprocs();
}
#endif

#ifdef _WIN32
int main(int argc, _TCHAR* argv[])
{
    for (int i = 0; i < GetNumberOfProcessorCores(); i++)
    {
        Core *core = new Core {0};
        core->CoreNumber = i;
        _beginthread(startMonitoringCoreSpeeds, 0, core);
    }
    cin.get();

    return EXIT_SUCCESS;
}
#elif defined __linux__
int main(int argc, char* argv[])
{
    cpu_set_t cpuset;
    pthread_t threads[MAX_CORES];

    for (int t = 0; t < GetNumberOfProcessorCores(); t++)
    {
        CPU_ZERO(&cpuset);
        CPU_SET(t, &cpuset);
        int rc = pthread_create(&threads[t], NULL, startMonitoringCoreSpeeds, (void *)t);
	rc = pthread_setaffinity_np(threads[t], sizeof(cpu_set_t), &cpuset);
    }
    cin.get();

    return EXIT_SUCCESS;
}
#endif
