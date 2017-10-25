# arduino-kodi-ir-keyboard
 
Kodi optimized infrared USB keyboard for an Asus Chromebox running openELEC.

IR device: Logitech Harmony 880 with remote profile: 'Panasonic TV TX-43CXW754' (NEC IR codes)

See [development branch mce-remote](https://github.com/zehnm/arduino-kodi-ir-keyboard/tree/mce-remote) for an MCE based version with [IRremote](https://github.com/z3t0/Arduino-IRremote).


Tested with: 
- Arduino Leonardo & SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
- Arduino IDE 1.6.7
- [HID Project](https://github.com/NicoHood/HID "HID Project") 2.4.3
- [IRLremote](https://github.com/NicoHood/IRLremote "IRLremote") 1.7.4

## Features
- Full Kodi control (at least what I use ;)
- Power on / off with Chromebox hardware modification.

   Documentation will follow, see [img folder](https://github.com/zehnm/arduino-kodi-ir-keyboard/tree/master/img) in the meantime.

- Key repeat
- Boot protocol mode (tested on a MacMini)

## Issues
- Watch dog reset doesn't work, according to Google this is most likely an Arduino bootloader bug.
- KEY_QUOTE doesn't work with non-us keyboard layout
  
## TODO
- CTRL+D / CTRL+W key commands for boot selection 
- sleep / wakeup (IF openELEC supports it)
