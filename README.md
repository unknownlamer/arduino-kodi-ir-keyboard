# arduino-kodi-ir-keyboard
 
Kodi optimized infrared USB keyboard for an Asus Chromebox running openELEC.

**DEVELOPMENT BRANCH for MCE remotes with RC6 IR protocol.**
**Uses IRremote library instead of IRLremote for RC6 protocol.**

IR device: Logitech Harmony 880 with remote profile: 'Intel NUC IntelD54250WYK'

Tested with: 
- SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
- Arduino IDE 1.8.5
- [HID Project](https://github.com/NicoHood/HID "HID Project") 2.4.4
- [IRremote](https://github.com/z3t0/Arduino-IRremote "IRremote") 2.2.3

## Features
- Full Kodi control (at least what I use ;)
- Power on / off with Chromebox hardware modification
- Key repeat
- Boot protocol mode (tested on a MacMini)

## Issues
- KEY_QUOTE doesn't work with non-us keyboard layout
- work in progress, not yet fully tested
  
## TODO
- CTRL+D / CTRL+W key commands for boot selection 
- sleep / wakeup (IF openELEC supports it)
