#include <windows.h>

typedef void(*Fucn)();

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    HMODULE hLib = LoadLibrary("Dll1.dll");
    if (!hLib) return 1;

    Fucn dllMain = (Fucn)GetProcAddress(hLib, "mainOwO");
    if (!dllMain)
    {
        FreeLibrary(hLib);
        return 1;
    }

    dllMain();
}