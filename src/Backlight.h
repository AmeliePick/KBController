#pragma once

typedef unsigned __int8 uint8;

namespace Backlight
{
    enum class Keys : uint8
    {
        ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrtSc, Del, Home, Ins, PgUp, PgDn,
        Tilde = 21, _1, _2, _3, _4, _5, _6, _7, _8, _9, _0, Minus, Plus, Backspace, NumLock = 36, NumSlash, NumMul, NumMinus,
        Tab = 42, Q, W, E, R, T, Y, U, I, O, P, LeftBrace, RightBrace, Backslash, Num7 = 57, Num8, Num9, NumPlus,
        CapsLock = 63, A, S, D, F, G, H, J, K, L, Semiñolon, DoubleQuotes, Enter = 76, Num4 = 78, Num5, Num6,
        Shift = 84, Z = 86, X, C, V, B, N, M, Comma, Period, Slash, RShift, UP, Num1 = 99, Num2, Num3, NumEnter,
        Ctrl = 105, Win, Alt, Space = 110, RAlt = 113, Fn, RCtrl, LEFT = 117, DOWN, RIGHT = 120, Num0, NumPeriod
    };

    struct Color
    {
        uint8 r;
        uint8 g;
        uint8 b;

        static Color&& Create(uint8 r, uint8 g, uint8 b, uint8 brightness);
    };

    // -- Sets the color for the key --
    // Use black color to turn off the key.
    __declspec(dllexport) void SetKey(Keys key, Color color);

    // -- Sets the whole row to the color --
    __declspec(dllexport) void SetRow(__int8 row, Color color);

    __declspec(dllexport) void SetNumPad(Color color);
}