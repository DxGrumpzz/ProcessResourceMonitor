#include <windows.h>
#include <cstdint>
#include <vector>
#include <vector>
#include <string>

#include "WindowsHelpers.hpp"


#define EXTREN_DLL extern "C" __declspec(dllexport)



struct ProcessData
{
    const wchar_t* ProcessName = nullptr;
    HANDLE ProcessHandle = NULL;
    DWORD ProcessID = NULL;
    HWND ProcessHWND = NULL;
};


BOOL CALLBACK EnumWindowsCountCallback(HWND hwnd, LPARAM param)
{
    int titleLength = GetWindowTextLengthW(hwnd);

    if (titleLength == 0)
        return true;

    titleLength += 1;

    wchar_t* title = new wchar_t[titleLength];
    GetWindowTextW(hwnd, title, titleLength);


    if (std::wcscmp(title, L"Program Manager") == 0)
    {
        delete[] title;
        title = nullptr;

        return true;
    }

    if (IsWindow(hwnd) == false ||
        IsWindowVisible(hwnd) == false)
        return true;

    std::uintmax_t& numberOfProcesses = *reinterpret_cast<std::uintmax_t*>(param);
    numberOfProcesses++;

    return true;
};


BOOL CALLBACK EnumWindowProcessesCallback(HWND hwnd, LPARAM param)
{
    int titleLength = GetWindowTextLengthW(hwnd);

    if (titleLength == 0)
        return true;

    titleLength += 1;

    wchar_t* title = new wchar_t[titleLength];
    GetWindowTextW(hwnd, title, titleLength);


    if (std::wcscmp(title, L"Program Manager") == 0)
    {
        delete[] title;
        title = nullptr;

        return true;
    }

    if (IsWindow(hwnd) == false ||
        IsWindowVisible(hwnd) == false)
        return true;


    std::pair<ProcessData*, std::uintmax_t>& processData = *reinterpret_cast<std::pair<ProcessData*, std::uintmax_t>*>(param);

    std::uintmax_t& processDataIndex = processData.second;
    ProcessData& process = processData.first[processDataIndex];

    process.ProcessHWND = hwnd;
    process.ProcessName = title;

    WINCALL(GetWindowThreadProcessId(hwnd, &process.ProcessID));

    if (std::wcscmp(title, L"Task Manager") == 0)
    {
        process.ProcessHandle = WINCALL(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, process.ProcessID));
    }
    else
        process.ProcessHandle = WINCALL(OpenProcess(PROCESS_ALL_ACCESS, false, process.ProcessID));

    processDataIndex++;

    return true;
};



EXTREN_DLL bool GetProcesses(ProcessData*& processListOut, std::uintmax_t& numberOfProcesses)
{
    EnumWindows(EnumWindowsCountCallback, reinterpret_cast<LPARAM>(&numberOfProcesses));

    processListOut = new ProcessData[numberOfProcesses];
    memset(processListOut, 0, sizeof(ProcessData) * numberOfProcesses);

    std::pair<ProcessData*, std::uintmax_t> enumWindowsCallbackFunctionData(processListOut, 0);
    EnumWindows(EnumWindowProcessesCallback, reinterpret_cast<LPARAM>(&enumWindowsCallbackFunctionData));

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