#include "Backlight.h"
#include <cstring>
#include <algorithm>


extern uint8 keyboardData[520];


Backlight::Color&& Backlight::Color::Create(uint8 r, uint8 g, uint8 b, uint8 brightness)
{
    // By default, the official software just changes the color values, not the physical brightness levels of LEDS.
    // For example, if color is 255; 255; 255 and "Visibility" set to 50 then the color isn't pure white. The pure white will be only
    // when "Visibility" is 100.

    return Color{};
}



void Backlight::SetKey(Keys key, Color color)
{
    *(keyboardData + (  8 + (uint8)key)) = color.r;
    *(keyboardData + (134 + (uint8)key)) = color.g;
    *(keyboardData + (260 + (uint8)key)) = color.b;
}


void Backlight::SetRow(__int8 row, Color color)
{
    switch (row)
    {
    case 1:
        memset(keyboardData +   8, color.r, 19);
        memset(keyboardData + 134, color.g, 19);
        memset(keyboardData + 260, color.b, 19);
        break;
    case 2:
        memset(keyboardData +   8 + 21, color.r, 14); memset(keyboardData +   8 + 36, color.r, 4);
        memset(keyboardData + 134 + 21, color.g, 14); memset(keyboardData + 134 + 36, color.g, 4);
        memset(keyboardData + 260 + 21, color.b, 19); memset(keyboardData + 260 + 36, color.b, 4);
        break;
    case 3:
        memset(keyboardData +   8 + 42, color.r, 14); memset(keyboardData +   8 + 57, color.r, 4);
        memset(keyboardData + 134 + 42, color.g, 14); memset(keyboardData + 134 + 57, color.g, 4);
        memset(keyboardData + 260 + 42, color.b, 14); memset(keyboardData + 260 + 57, color.b, 4);
        break;
    case 4:
        memset(keyboardData +   8 + 63, color.r, 12); memset(keyboardData +   8 + 78, color.r, 3);
        memset(keyboardData + 134 + 63, color.g, 12); memset(keyboardData + 134 + 78, color.g, 3);
        memset(keyboardData + 260 + 63, color.b, 12); memset(keyboardData + 260 + 78, color.b, 3);
        SetKey(Backlight::Keys::Enter, color);
        break;
    case 5:
        memset(keyboardData +   8 + 86, color.r, 12); memset(keyboardData +   8 + 99, color.r, 4);
        memset(keyboardData + 134 + 86, color.g, 12); memset(keyboardData + 134 + 99, color.g, 4);
        memset(keyboardData + 260 + 86, color.b, 12); memset(keyboardData + 260 + 99, color.b, 4);
        SetKey(Backlight::Keys::Shift, color);
        break;
    case 6:
        memset(keyboardData +   8 + 105, color.r, 3); memset(keyboardData +   8 + 120, color.r, 3);
        memset(keyboardData + 134 + 105, color.g, 3); memset(keyboardData + 134 + 120, color.g, 3);
        memset(keyboardData + 260 + 105, color.b, 3); memset(keyboardData + 260 + 120, color.b, 3);
        SetKey(Backlight::Keys::Space, color); SetKey(Backlight::Keys::RAlt,  color);
        SetKey(Backlight::Keys::Fn,    color); SetKey(Backlight::Keys::RCtrl, color);
        SetKey(Backlight::Keys::LEFT,  color); SetKey(Backlight::Keys::DOWN,  color);
        break;
    default:
        break;
    }
}


void Backlight::SetNumPad(Color color)
{
    memset(keyboardData +   8 + 36,  color.r, 4);
    memset(keyboardData + 134 + 36,  color.g, 4);
    memset(keyboardData + 260 + 36,  color.b, 4);

    memset(keyboardData +   8 + 57, color.r, 4);
    memset(keyboardData + 134 + 57, color.g, 4);
    memset(keyboardData + 260 + 57, color.b, 4);

    memset(keyboardData +   8 + 78, color.r, 3);
    memset(keyboardData + 134 + 78, color.g, 3);
    memset(keyboardData + 260 + 78, color.b, 3);

    memset(keyboardData +   8 + 99, color.r, 4);
    memset(keyboardData + 134 + 99, color.g, 4);
    memset(keyboardData + 260 + 99, color.b, 4);

    memset(keyboardData +   8 + 121, color.r, 2);
    memset(keyboardData + 134 + 121, color.g, 2);
    memset(keyboardData + 260 + 121, color.b, 2);
}


