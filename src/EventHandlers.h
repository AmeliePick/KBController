#pragma once
typedef unsigned __int8 uint8;

typedef void(*EventTrigger)(uint8*);

#ifdef USE
extern EventTrigger EventNumpadOn;
extern EventTrigger EventNumpadOff;
extern EventTrigger KeyboardInitState;
#elif defined(INIT)
EventTrigger EventNumpadOn;
EventTrigger EventNumpadOff;
EventTrigger KeyboardInitState;
#endif
