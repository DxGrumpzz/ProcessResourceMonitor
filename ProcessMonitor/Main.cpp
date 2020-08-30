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

std::wstring GetLastErrorAsStringW()
{
    // Stores the error message as a string in memory
    LPWSTR buffer = nullptr;

    // Format DWORD error ID to a string 
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   GetLastError(),
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPWSTR)&buffer, 0, NULL);

    // Create std string from buffer
    std::wstring message(buffer);

    return message;
};

std::string GetLastErrorAsStringA()
{
    // Stores the error message as a string in memory
    LPSTR buffer = nullptr;

    // Format DWORD error ID to a string 
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   GetLastError(),
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPSTR)&buffer, 0, NULL);

    // Create std string from buffer
    std::string message(buffer);

    return message;
};

#define WINCALL(result) WinCall(result)



void WinCall(std::intmax_t result)
{
    if (!result)
    {
        throw std::exception(GetLastErrorAsStringA().c_str());
    };
};


BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM param)
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

};



int main()
{
    std::vector<std::pair<DWORD, HWND>> windowHandles;

    EnumWindows(EnumWindowsCallback, reinterpret_cast<LPARAM>(&windowHandles));

    DisplayProcesses(windowHandles);


    DWORD processID = 0;
    std::cin >> processID;

    const std::pair<DWORD, HWND>& processData =
        *std::find_if(windowHandles.cbegin(), windowHandles.cend(),
                      [processID](const std::pair<DWORD, HWND>& element)
                      {
                          if (element.first == processID)
                              return true;

                          return false;
                      });


    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, processID);

    while (1)
    {
        std::time_t timePoint = std::time(0);
        std::tm timePointNow { 0 };

        localtime_s(&timePointNow, &timePoint);

        std::cout << timePointNow.tm_hour << ":" << timePointNow.tm_min << ":" << timePointNow.tm_sec << "\n";
        std::cout << "Press \"Ctrl\" + \"C\" to exit anytime" << "\n";


        PROCESS_MEMORY_COUNTERS_EX  processMemory { 0 };
        processMemory.cb = sizeof(processMemory);

        WINCALL(K32GetProcessMemoryInfo(processHandle, reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&processMemory), sizeof(processMemory)));

        std::cout << 
            "Page faults: " <<                        processMemory.PageFaultCount << "\n"  << 
            "Peak working set size: " <<              processMemory.PeakWorkingSetSize << "\n" << 
            "WorkingSetSize: " <<                     processMemory.WorkingSetSize << "\n" << 
            "QuotaPeakPagedPoolUsage: " <<            processMemory.QuotaPeakPagedPoolUsage << "\n" <<
            "QuotaPagedPoolUsage: " <<                processMemory.QuotaPagedPoolUsage << "\n" << 
            "QuotaPeakNonPagedPoolUsage: " << processMemory.QuotaPeakNonPagedPoolUsage << "\n" << 
            "QuotaNonPagedPoolUsage: " <<     processMemory.QuotaNonPagedPoolUsage << "\n"  <<
            "PagefileUsage: " <<              processMemory.PagefileUsage << "\n" << 
            "PeakPagefileUsage: " <<          processMemory.PeakPagefileUsage << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    };


    CloseHandle(processHandle);

    /*
    HWND hwnd = NULL;


    for (auto& element : windowHandles)
    {
        if (element.first == pid)
        {
            hwnd = element.second;
        };
    };

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);


    HINSTANCE processInstanceHandle = reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(hwnd, GWLP_HINSTANCE));

    wchar_t executeablePath[MAX_PATH] { 0 };

    K32GetModuleFileNameExW(processHandle, nullptr, executeablePath, MAX_PATH);

    HICON iconHandle = ExtractIconW(processInstanceHandle, executeablePath, 0);

    if (!iconHandle)
    {
        auto error = GetLastErrorAsStringW();
        DebugBreak();
    };


    ICONINFO iconInfo { 0 };
    GetIconInfo(iconHandle, &iconInfo);

    std::uint8_t pixels1[1024] { 0};
    std::uint8_t pixels2[1024] { 0};

    WINCALL(GetBitmapBits(iconInfo.hbmColor, 1024, pixels1));
    WINCALL(GetBitmapBits(iconInfo.hbmMask, 1024, pixels2));

    CloseHandle(processHandle);

    */

    //char* pImage = NULL;
    //WINCALL(GlobalLock(iconHandle));
};