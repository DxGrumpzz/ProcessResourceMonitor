#include <Windows.h>
#include <winuser.h>
#include <cstdint>
#include <psapi.h>
#include <tlhelp32.h>
#include <exception>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <unordered_map>
#include <thread>
#include <CommCtrl.h>
#include <sstream>
#include <thread>

#include "WindowsHelpers.hpp"




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


    std::vector<std::pair<DWORD, HWND>>& windowHandles = *reinterpret_cast<std::vector<std::pair<DWORD, HWND>>*>(param);

    DWORD processID = 0;

    GetWindowThreadProcessId(hwnd, &processID);

    windowHandles.emplace_back(processID, hwnd);

    delete[] title;
    title = nullptr;

    return true;
};



void DisplayProcesses(const std::vector<std::pair<DWORD, HWND>>& windowHandles)
{
    int counter = 1;


    for (const std::pair<DWORD, HWND>& element : windowHandles)
    {
        const HWND hwnd = element.second;
        const DWORD pid = element.first;

        int titleLength = GetWindowTextLengthW(hwnd);


        titleLength += 1;

        wchar_t* windowTitle = new wchar_t[titleLength];

        GetWindowTextW(hwnd, windowTitle, titleLength);


        std::wcout << counter << L"." << windowTitle << L" hwnd: " << hwnd << L" PID: " << pid << L"\n";

        delete[] windowTitle;


        counter++;
    };

    std::cout << "\n";
};

char AddShowGrowthToStream(std::intmax_t current, std::intmax_t previous)
{
    if (current == previous)
    {
        return '=';
    }
    else if (current < previous)
    {
        return 'v';
    }
    else if (current > previous)
    {
        return '^';
    };

    return '?';
}


const std::pair<DWORD, HWND>& FindProcessByID(const std::vector<std::pair<DWORD, HWND>>& windowHandles)
{
    while (1)
    {
        std::cout << "Process ID: ";

        DWORD processID = 0;
        std::cin >> processID;

        std::vector<std::pair<DWORD, HWND>>::const_iterator processData = std::find_if(windowHandles.cbegin(), windowHandles.cend(),
                                                                                       [processID](const std::pair<DWORD, HWND>& element)
        {
            if (element.first == processID)
                return true;

            return false;
        });
        if (processData != windowHandles.cend())
            return *processData;
        else
        {
            std::cout << "Invalid process ID: " << processID << "\n";
        };
    };
};


float GetPercentageChange(std::intmax_t current, std::intmax_t previous)
{
    std::intmax_t difference = current - previous;

    if (difference > 0)
        int s = 0;

    float percentage = std::abs(difference) / (static_cast<float>(current) / 100);

    return percentage;
};


void ShowProcessMemoryData(const PROCESS_MEMORY_COUNTERS_EX& currentProcessMemory, const PROCESS_MEMORY_COUNTERS_EX& previousProcessMemory)
{
    std::cout << "\n\n" <<
        "Page faults:                " << currentProcessMemory.PageFaultCount << " " << AddShowGrowthToStream(currentProcessMemory.PageFaultCount, previousProcessMemory.PageFaultCount) << " " << static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount) - static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount) << " " << GetPercentageChange(static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount), static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount)) << '%' << "\n" <<
        "Working set size:           " << currentProcessMemory.WorkingSetSize << " " << AddShowGrowthToStream(currentProcessMemory.WorkingSetSize, previousProcessMemory.WorkingSetSize) << " " << static_cast<std::intmax_t>(currentProcessMemory.WorkingSetSize) - static_cast<std::intmax_t>(previousProcessMemory.WorkingSetSize) << " " << GetPercentageChange(static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount), static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount)) << '%' << "\n" <<
        "Quota paged pool usage:     " << currentProcessMemory.QuotaPagedPoolUsage << " " << AddShowGrowthToStream(currentProcessMemory.QuotaPagedPoolUsage, previousProcessMemory.QuotaPagedPoolUsage) << " " << static_cast<std::intmax_t>(currentProcessMemory.QuotaPagedPoolUsage) - static_cast<std::intmax_t>(previousProcessMemory.QuotaPagedPoolUsage) << " " << GetPercentageChange(static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount), static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount)) << '%' << "\n" <<
        "Quota non paged pool usage: " << currentProcessMemory.QuotaNonPagedPoolUsage << " " << AddShowGrowthToStream(currentProcessMemory.QuotaNonPagedPoolUsage, previousProcessMemory.QuotaNonPagedPoolUsage) << " " << static_cast<std::intmax_t>(currentProcessMemory.QuotaNonPagedPoolUsage) - static_cast<std::intmax_t>(previousProcessMemory.QuotaNonPagedPoolUsage) << " " << GetPercentageChange(static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount), static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount)) << '%' << "\n" <<
        "Page file usage:            " << currentProcessMemory.PagefileUsage << " " << AddShowGrowthToStream(currentProcessMemory.PagefileUsage, previousProcessMemory.PagefileUsage) << " " << static_cast<std::intmax_t>(currentProcessMemory.PagefileUsage) - static_cast<std::intmax_t>(previousProcessMemory.PagefileUsage) << " " << GetPercentageChange(static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount), static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount)) << '%' << "\n" <<
        "Private usage:              " << currentProcessMemory.PrivateUsage << " " << AddShowGrowthToStream(currentProcessMemory.PrivateUsage, previousProcessMemory.PrivateUsage) << " " << static_cast<std::intmax_t>(currentProcessMemory.PrivateUsage) - static_cast<std::intmax_t>(previousProcessMemory.PrivateUsage) << " " << GetPercentageChange(static_cast<std::intmax_t>(currentProcessMemory.PageFaultCount), static_cast<std::intmax_t>(previousProcessMemory.PageFaultCount));

};


int main()
{
    MEMORYSTATUSEX memoryStatus { sizeof(memoryStatus) };

    GlobalMemoryStatusEx(&memoryStatus);


    std::vector<std::pair<DWORD, HWND>> windowHandles;

    EnumWindows(EnumWindowProcessesCallback, reinterpret_cast<LPARAM>(&windowHandles));

    DisplayProcesses(windowHandles);


    std::pair<DWORD, HWND> processData = FindProcessByID(windowHandles);
    DWORD processID = processData.first;


    HANDLE processHandle = WINCALL(OpenProcess(PROCESS_ALL_ACCESS, false, processID));


    PERFORMACE_INFORMATION performaceInfo { 0 };
    performaceInfo.cb = sizeof(performaceInfo);

    WINCALL(K32GetPerformanceInfo(&performaceInfo, sizeof(performaceInfo)));


    PROCESS_MEMORY_COUNTERS_EX previousProcessMemory { 0 };

    while (1)
    {
        const std::size_t processTitleLength = static_cast<std::size_t>(GetWindowTextLengthW(processData.second)) + 1;
        wchar_t* processTitle = new wchar_t[processTitleLength];
        memset(processTitle, 0, processTitleLength);

        WINCALL(GetWindowTextW(processData.second, processTitle, static_cast<int>(processTitleLength)));

        std::time_t timePoint = std::time(0);
        std::tm timePointNow { 0 };

        localtime_s(&timePointNow, &timePoint);

        std::wcout <<
            "Press \"Ctrl\" + \"C\" to exit anytime" << "\n" <<
            timePointNow.tm_hour << ":" << timePointNow.tm_min << ":" << timePointNow.tm_sec << "\n" <<
            "PID: " << processID << "\n" <<
            "Title: " << processTitle << "\n";


        PROCESS_MEMORY_COUNTERS_EX  currentProcessMemory { 0 };
        currentProcessMemory.cb = sizeof(currentProcessMemory);


        WINCALL(K32GetProcessMemoryInfo(processHandle, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&currentProcessMemory), sizeof(currentProcessMemory)));

        ShowProcessMemoryData(currentProcessMemory, previousProcessMemory);


        delete[] processTitle;
        processTitle = nullptr;

        previousProcessMemory = currentProcessMemory;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    };


    CloseHandle(processHandle);

};