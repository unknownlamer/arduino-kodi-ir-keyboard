# arduino-kodi-ir-keyboard
Kodi optimized infrared USB keyboard for an Asus Chromebox running openELEC.

IR device: Logitech Harmony 880 with remote profile: 'Plex Player'

Tested with SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)

Initial proof of concept with HID-Project instead of standard Keyboard library: 
- Kodi control works pretty well already :-)
- key repeat works
- boot protocol mode works (tested on a MacMini)

## Issues:
- Sketch doesn't always start after power up!? 
   Needs more testing...
 
## TODO:
- Power off / sleep / wakeup
- CTRL+D / CTRL+W key combinations for boot selection 
- Chromebox hardware modification to control power toggle switch with Arduino
