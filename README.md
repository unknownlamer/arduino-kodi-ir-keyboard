# arduino-kodi-ir-keyboard / IRLremote + BootKeyboard branch
Kodi optimized infrared USB keyboard for an Asus Chromebox running openELEC.

IR device: Logitech Harmony 880 with remote profile: 'Panasonic TV TX-43CXW754'

Testing with: 
- SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
- Arduino IDE 1.6.7
- HID Project 2.4.3
- IRLremote   1.7.4

Initial proof of concept with HID-Project instead of standard Keyboard library: 
- Kodi control works pretty well already :-)
- key repeat works
- boot protocol mode works (tested on a MacMini)

## Issues:
- Most promising branch yet :)
- IRLremote doesn't work with Logitech Plex Remote profile. Now (ab)using Panasonic TV TX-43CXW754 device template.
- Watch dog reset doesn't work, according to Google this is most likely an Arduino bootloader bug.
  
## TODO:
- Power off / sleep / wakeup
- CTRL+D / CTRL+W key combinations for boot selection 
- Chromebox hardware modification to control power toggle switch with Arduino
