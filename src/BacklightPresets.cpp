#include "BacklightPresets.h"
#include "Backlight.h"

void BacklightPresets::Static::RussianFlag()
{
    Backlight::SetRow(1, { 255, 255, 255 });
    Backlight::SetRow(2, { 255, 255, 255 });
    Backlight::SetRow(3, { 0, 106, 255 });
    Backlight::SetRow(4, { 0, 106, 255 });
    Backlight::SetRow(5, { 255, 0, 0 });
    Backlight::SetRow(6, { 255, 0, 0 });
}

void BacklightPresets::Static::GermanFlag()
{
    Backlight::SetRow(1, { 0, 0, 0 });
    Backlight::SetRow(2, { 0, 0, 0 });
    Backlight::SetRow(3, { 255, 0, 0 });
    Backlight::SetRow(4, { 255, 0, 0 });
    Backlight::SetRow(5, { 255, 204, 0 });
    Backlight::SetRow(6, { 255, 204, 0 });
}

void BacklightPresets::Static::JapaneseFlag()
{
    Backlight::SetRow(1, { 255, 255, 255 });
    Backlight::SetRow(2, { 255, 255, 255 });
    Backlight::SetRow(3, { 255, 255, 255 });
    Backlight::SetRow(4, { 255, 255, 255 });
    Backlight::SetRow(5, { 255, 255, 255 });
    Backlight::SetRow(6, { 255, 255, 255 });

    Backlight::SetKey(Backlight::Keys::Y, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::U, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::H, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::J, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::_6, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::_7, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::_8, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::T, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::I, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::G, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::K, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::B, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::N, { 255, 0, 0 });
    Backlight::SetKey(Backlight::Keys::M, { 255, 0, 0 });
}
