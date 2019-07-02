#include <iostream>
#include <thread>
#include <mutex>

#include <windows.h>
#include <tchar.h>
#include <process.h>
<<<<<<< HEAD
=======
#include <iostream>
#include <mutex>
>>>>>>> refs/rewritten/onto

using namespace std;

mutex g_io_mutex;

struct Core
{
    int CoreNumber;
};

mutex g_i_mutex;

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
        Sleep(1666);
        QueryPerformanceCounter(&second);
<<<<<<< HEAD
        
        lock_guard<mutex> lock(g_i_mutex);
            cout << "Core " << core.CoreNumber << " has frequency " << ((float)(second.QuadPart - first.QuadPart)/frequency.QuadPart) << " GHz" << endl;
        g_i_mutex.unlock();
=======
        lock_guard<mutex> lock(g_io_mutex);
        cout << "Core " << core.CoreNumber << " has frequency " << ((float)(second.QuadPart - first.QuadPart) / frequency.QuadPart) << " GHz" << endl;
        g_io_mutex.unlock();
>>>>>>> refs/rewritten/onto
    }
}

int GetNumberOfProcessorCores()
{
    DWORDLONG process, system;
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
