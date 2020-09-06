#include <windows.h>
#include <cstdint>

#define EXTREN_DLL extern "C" __declspec(dllexport)

struct ProcessData
{
    const wchar_t* ProcessName = nullptr;
    HANDLE ProcessHandle = NULL;
    HWND ProcessHWND = NULL;
};


EXTREN_DLL bool GetProcesses(ProcessData*& processListOut, std::uintmax_t& numberOfProcesses)
{
    numberOfProcesses = 2;
    processListOut = new ProcessData[2] { 0 };
    
    processListOut[0] = ProcessData({ L"1", 0, 0 });
    processListOut[1] = ProcessData({ L"2", 0, 0 });

    return true;
};


EXTREN_DLL void DeleteUnmangedMemory(void*& pointer, bool isArray = false)
{
    if (isArray == true)
    {
        delete[] pointer;
    }
    else
        delete pointer;

    pointer = nullptr;
};