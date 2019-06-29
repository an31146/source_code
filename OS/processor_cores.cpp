#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <iostream>

using namespace std;

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
}
