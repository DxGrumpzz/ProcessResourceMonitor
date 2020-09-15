#pragma once
#include <string>
#include <windows.h>


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


#ifdef _DEBUG

#define WINCALL(result) WinCall(result, __LINE__, __FILEW__)


void ShowWinError(std::intmax_t line, const wchar_t* file)
{
    const wchar_t* errorTitle = L"WinError";

    std::wstring error;
    error.append(L"An error occured in ")
        .append(file)
        .append(L"\n")
        .append(L"Line: ")
        .append(std::to_wstring(line))
        .append(L"\n")
        .append(L"Error:\n")
        .append(GetLastErrorAsStringW());

    MessageBoxW(NULL, error.c_str(), errorTitle, MB_ICONERROR);

};


std::intmax_t WinCall(std::intmax_t result, std::intmax_t line, const wchar_t* file)
{
    if (!result)
    {
        ShowWinError(line, file);
      
        DebugBreak();
    };

    return result;
};

HANDLE WinCall(HANDLE handleResult, std::intmax_t line, const wchar_t* file)
{
    if (handleResult == NULL)
    {
        ShowWinError(line, file);

        DebugBreak();
    };

    return handleResult;
};

#else
// This macro will not check if the call has failed because the environment is currently set to Release
#define WINCALL
#endif