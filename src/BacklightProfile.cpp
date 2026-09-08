#include "BacklightProfile.h"
#define INIT
#include "EventHandlers.h"
#include <Windows.h>
#include <string>
#include <fstream>
#include <smmintrin.h>


bool GetNewHash(const std::string& filePath, BYTE* hash)
{
    HCRYPTPROV hProv = 0, hHash = 0;
    BYTE buffer[4096];
    DWORD bytesRead = 0;
    

    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE ||
        CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == false ||
        CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == false) return false;

    while (ReadFile(hFile, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0)
        if (!CryptHashData(hHash, buffer, bytesRead, 0)) break;
    CloseHandle(hFile);
    

    DWORD hashSize = 16;
    CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0);

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return true;
}

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
void LoadProfile()
{
    wchar_t exePath[MAX_PATH];

    if (GetModuleFileNameW(NULL, exePath, MAX_PATH) > 0)
    {
        PathRemoveFileSpecW(exePath);
        SetCurrentDirectoryW(exePath);
    }

    std::string fileName = "hash.kbc";


    char oldHash[16];
    HANDLE file = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE)
    {
        DWORD read = 0;
        ReadFile(file, oldHash, sizeof(oldHash), &read, NULL);
    }

    BYTE newHash[16];
    if (!GetNewHash("KBProfile.cpp", newHash))
    {
    ExitNoProfile:
        MessageBox(NULL, "Profile is not detected. Exiting...", "Error", 0);
        ExitProcess(1);
    }

    if (!_mm_test_all_ones(_mm_cmpeq_epi8(_mm_loadu_si128((const __m128i*)oldHash), _mm_loadu_si128((const __m128i*)newHash))))
    {
        if (file == INVALID_HANDLE_VALUE) goto ExitNoProfile;

        SetFilePointer(file, 0, NULL, FILE_BEGIN);
        WriteFile(file, newHash, sizeof(newHash), (DWORD*)oldHash, NULL);
        CloseHandle(file);

        std::wstring file = exePath;
        file += L"\\KBTriggers.dll";

        DeleteFileW(file.c_str());

        std::string CL = "set INCLUDE=msvc\\lib\\include; && set LIB=msvc\\lib; && \"msvc\\bin\\cl.exe\" /std:c++17 /EHsc /MDd /LD \"KBProfile.cpp\" /link /OUT:KBTriggers.dll KBController.lib";
        system(CL.c_str());
    }

    HMODULE triggers = LoadLibrary("KBTriggers.dll");
    if (triggers == NULL)
    {
        MessageBox(NULL, "Triggers are not detected. Exiting...", "Error", 0);
        ExitProcess(1);
    }

    EventNumpadOn = (EventTrigger)GetProcAddress(triggers, "EventNumpadOn");
    EventNumpadOff = (EventTrigger)GetProcAddress(triggers, "EventNumpadOff");
    KeyboardInitState = (EventTrigger)GetProcAddress(triggers, "KeyboardInitState");
}
