#include "pch.h"
#include <windows.h>
#include <iostream>
#include <thread>

using namespace std;





DWORD countStart;
LPCWSTR regPath = L"SOFTWARE\\Marvel's Spider-Man 2";
LPCWSTR regValueName = L"Time Played";

DWORD ReadRegistryValue() {
    HKEY hKey;
    DWORD value;
    DWORD dataSize = sizeof(DWORD);
    LONG result = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey);
    if (result == ERROR_SUCCESS) {
        result = RegQueryValueEx(hKey, regValueName, NULL, NULL, (LPBYTE)&value, &dataSize);
        RegCloseKey(hKey);
        if (result == ERROR_SUCCESS) {
            return value;
        }
    }
    return -1;
}

void WriteRegistryValue(DWORD value) {
    HKEY hKey;
    LONG result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS) {
        result = RegSetValueEx(hKey, regValueName, 0, REG_DWORD, (const BYTE*)&value, sizeof(DWORD));
        RegCloseKey(hKey);
    }
}

void Handle_Timer() {
    BYTE delayCounter = 0;

    while (true) {
        Sleep(1000);
        countStart++;

        delayCounter++;
        if (delayCounter == 10) {
            delayCounter = 0;
            WriteRegistryValue(countStart);
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        
        countStart = ReadRegistryValue();
        thread(Handle_Timer).detach();
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        WriteRegistryValue(countStart);
        break;
    }

    return TRUE;
}