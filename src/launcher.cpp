/*
+-----------------------+
|  by AmeliePick. 2025  |
| github.com/AmeliePick |
+-----------------------+
*/

#undef UNICODE
#define VID 0x05AC
#define PID 0x024F

#include <Windows.h>
#include <hidsdi.h>
#include "../hidapi/hidapi.h"
#pragma comment(lib, "hidapi.lib")
#pragma comment(lib, "Hid.lib")
#pragma comment(lib, "Winmm.lib")
#include "Backlight.h"

#define USE
#include "EventHandlers.h"
#include "BacklightProfile.h"


static uint8 toggles = 4; // <-- 1 bit - NumLock trigger, 2 bit - blink button trigger, 3 bit - blink state, 4 bit - blink button trigger state(Up/Down).
uint8 keyboardData[520];
static HANDLE deviceHandle = NULL;


VOID CALLBACK BlinkREC(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    toggles = (toggles & ~(1 << 2) | ((-(!(toggles & 4))) & (1 << 2)));

    Backlight::SetKey(Backlight::Keys::PgUp, { (uint8)(34 * (toggles & 4)), 0, 0 });
    HidD_SetFeature(deviceHandle, keyboardData, 520);
}


#pragma region NumPad
void NumLockOFF()
{
    EventNumpadOff(keyboardData);
}


void NumLockON()
{
    EventNumpadOn(keyboardData);
}

static void(*numLockActions[2])() = { &NumLockOFF, &NumLockON };
#pragma endregion


#pragma region Blinker
static MMRESULT RECBlinkTimer = NULL;

void BlinkerON()
{
    Backlight::SetKey(Backlight::Keys::PgUp, { 0, 0, 0 });
    RECBlinkTimer = timeSetEvent(500, 0, BlinkREC, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);
}


void BlinkerOFF()
{
    timeKillEvent(RECBlinkTimer);
    Backlight::SetKey(Backlight::Keys::PgUp, { 1, 69, 0 });
}

static void(*blinkerActions[2])() = { &BlinkerOFF, &BlinkerON };
#pragma endregion


VOID CALLBACK InputPoll(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    uint8 oldFlags = toggles; // Save CPU and USB bus and send data only when the state is actually changes.
    if ((GetKeyState(VK_NUMLOCK) & 1) != (toggles & 1))
    {
        toggles = (toggles & 254) | ((-(!(toggles & 1))) & 1);
        numLockActions[toggles & 1]();
    }

    if ((GetKeyState(VK_SCROLL) & 0x8000)) toggles |= 8; // Key DOWN event.
    else if ((GetKeyState(VK_SCROLL) & 0x8000) == 0 && (toggles & 8)) // Key UP event.
    {
        toggles = (toggles & 253) | ((-(!(toggles & 2))) & 2);
        blinkerActions[(toggles & 2) >> 1]();
        toggles &= 247;
    }

    if (oldFlags != toggles) HidD_SetFeature(deviceHandle, keyboardData, 520);
}


int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    LoadProfile();

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


    // Configure the backlight to the keyboard's state.
    KeyboardInitState(keyboardData);
    toggles |= (GetKeyState(VK_NUMLOCK) & 0x0001);
    numLockActions[toggles & 1]();


    PHIDP_PREPARSED_DATA preparsedData = NULL;
    HIDP_CAPS caps;
    HidD_GetPreparsedData(deviceHandle, &preparsedData);
    HidP_GetCaps(preparsedData, &caps);

    if (caps.NumberFeatureValueCaps != 0 || caps.NumberFeatureButtonCaps != 0)
    {
        HIDP_VALUE_CAPS* valueCaps = (HIDP_VALUE_CAPS*)malloc(sizeof(HIDP_VALUE_CAPS) * caps.NumberFeatureValueCaps);
        if (HidP_GetValueCaps(HidP_Feature, valueCaps, &caps.NumberFeatureValueCaps, preparsedData) == HIDP_STATUS_SUCCESS)
        {
            // On a test keyboard it has only one Report ID and uses it twice in first and second bytes of the data buffer.
            // Needs more info from similar keyboards about multiple IDs usage.

            keyboardData[0] = valueCaps[0].ReportID;
            keyboardData[1] = valueCaps[0].ReportID;
        }
        free(valueCaps);
    }

    HidD_SetFeature(deviceHandle, keyboardData, sizeof(keyboardData));


    // ~62.5 Hz.
    MMRESULT polling = timeSetEvent(16, 0, InputPoll, NULL, TIME_CALLBACK_FUNCTION | TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    timeKillEvent(polling);
    CloseHandle(deviceHandle);
    return 0;
}