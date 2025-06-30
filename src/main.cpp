/*
+-----------------------+
|  by AmeliePick. 2025  |
| github.com/AmeliePick |
+-----------------------+
*/

#include <windows.h>
#include <hidsdi.h>
#include "../hidapi/hidapi.h"
#pragma comment(lib, "hidapi.lib")
#pragma comment(lib, "Hid.lib")
#pragma comment(lib, "Winmm.lib")

#define VID 0x05AC
#define PID 0x024F

typedef unsigned __int8 uint8;

static uint8 toggles = 4; // <-- 1 bit - NumLock trigger, 2 bit - blink button trigger, 3 bit - blink state.
static uint8 keyboardData[520] = {  };
static uint8 keyboardDataON[5] = { 0x22, 0x22, 0x22, 0x22, 0x0 };
static HANDLE deviceHandle = NULL;
static MMRESULT RECBlinkTimer = NULL;


VOID CALLBACK BlinkREC(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    toggles = (toggles & ~(1 << 2) | ((-(!(toggles & 4))) & (1 << 2)));

    uint8 value = 0x22 * (toggles & 4);
    memcpy(keyboardData + 121, &value, 1);
    HidD_SetFeature(deviceHandle, keyboardData, 520);
}

#pragma region NumPad
void NumLockOFF()
{
    ZeroMemory(keyboardData + 44,  4);
    ZeroMemory(keyboardData + 65,  4);
    ZeroMemory(keyboardData + 86,  3);
    ZeroMemory(keyboardData + 107, 4);
    ZeroMemory(keyboardData + 129, 2);
}


void NumLockON()
{
    memcpy(keyboardData + 44,  keyboardDataON, 4);
    memcpy(keyboardData + 65,  keyboardDataON, 4);
    memcpy(keyboardData + 86,  keyboardDataON, 3);
    memcpy(keyboardData + 107, keyboardDataON, 4);
    memcpy(keyboardData + 129, keyboardDataON, 2);
}

void (*numLockActions[2])() = { &NumLockOFF, &NumLockON };
#pragma endregion


#pragma region Blinker
void BlinkerON()
{
    ZeroMemory(keyboardData + 121, 1);
    RECBlinkTimer = timeSetEvent(500, 0, BlinkREC, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);
}


void BlinkerOFF()
{
    timeKillEvent(RECBlinkTimer);
    memcpy(keyboardData + 121, keyboardDataON, 1);
}

void(*blinkerActions[2])() = { &BlinkerOFF, &BlinkerON };
#pragma endregion


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode != HC_ACTION || (wParam != WM_KEYDOWN && wParam != WM_SYSKEYDOWN)) return CallNextHookEx(NULL, nCode, wParam, lParam);
    
    KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
    switch (p->vkCode)
    {
    case VK_NUMLOCK:
        toggles = (toggles & 254) | ((-(!(toggles & 1))) & 1);
        numLockActions[(toggles & 1)]();
        break;
    case VK_SCROLL:
        toggles = (toggles & 253) | ((-(!(toggles & 2))) & 2);
        blinkerActions[(toggles & 2) >> 1]();
        break;
    default:
        return CallNextHookEx(NULL, nCode, wParam, lParam);;
    }

    HidD_SetFeature(deviceHandle, keyboardData, 520);
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    
    if (hid_init() != 0)
    {
        MessageBox(NULL, "HID_API init error. Exiting...", "Error", 0);
        return 0;
    }

    hid_device_info* device, *devBegin;
    device = devBegin = hid_enumerate(VID, PID);
    if (!device)
    {
        MessageBox(NULL, "No available devices. Exiting...", "Error", 0);
        hid_exit();
        return 0;
    }

    while (device)
    {
        if (device->usage_page == 0xFF01)
        {
            deviceHandle = CreateFile(device->path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (deviceHandle == INVALID_HANDLE_VALUE)
            {
                MessageBox(NULL, "Cannot find specified device. Exiting...", "Error", 0);
                return 0;
            }
            break;
        }
        device = device->next;
    }
    hid_free_enumeration(devBegin);
    hid_exit();


    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);
    if (!hHook)
    {
        MessageBox(NULL, "Cannot set a hook. Exiting...", "Error", 0);
        return 0;
    }

    toggles |= (GetKeyState(VK_NUMLOCK) & 0x0001);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    CloseHandle(deviceHandle);
    return 0;
}