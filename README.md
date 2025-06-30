# KBController [Windows]

This is a small custom software prototype to work with ZEON K98 PRO keyboard firmware.
Currently, it has 2 features:
- On/Off numpad backlight with turning on/off the numpad itself. So, if the numpad is off then its backlight will be also off.
- Blinker for any key that can be a trigger. I remapped RAlt to ScrollLock and useit for different purposes like a toggle. When the toggle is on, then the key's backlight will blink until the trigger is turned off.
In future updates it's planned to add a feature to configure the software and keyboard from JSON.


## Usage
1. To use this software you need close the official app.
2. Specify VID and PID of your keyboard in the code.
3. Also you need find the correct HID by vendor. In my case it uses User Page(UP) FF00. Try search it all in Device Manager -> HID devices -> HID by vender -> Properties -> details -> Hardware ID.
4. I didn't decrypt the requests, so I'm not sure if it contains any extra data in the requests, so you need fill `keyboardData` with data from your PC. To do it, just use Wireshark with USBPcap, open the official app keyabord app, record a few seconds with USBPcap and in Wireshark find 520+ lenght record from your host to your device. Copy the data fragment as C-array and past it in the code. 


## Not really, maybe
Since ZEON K98 PRO is based on some chinese-korean keyboards, I guess, this software will work K98 keyboard and its derivatives.

## Build
To build this code you will also need hidapi.

---
Tested on Windows 10.