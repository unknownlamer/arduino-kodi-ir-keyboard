# arduino-kodi-ir-keyboard / BootKeyboard branch
Kodi optimized infrared USB keyboard for an Asus Chromebox running openELEC.

IR device: Logitech Harmony 880 with remote profile: 'Plex Player'

Testing with: 
- SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
- MattairTech MT-DB-U4 (ATmega 32U4), 16MHZ 5V, CDC_HID

Arduino IDE 1.6.7
HID Project 2.4.3
IRRemote 2.0.1

Initial proof of concept with HID-Project instead of standard Keyboard library: 
- Kodi control works pretty well already :-)
- key repeat works
- boot protocol mode works (tested on a MacMini)

## Issues:
- Stability issues: program hangs within 0..60 minutes 
   (Watch dog reset didn't work either, according to Google this is most likely a bootloader bug)
 
## TODO:
- Stable operation: continue testing with an Arduino Leonardo
- Power off / sleep / wakeup
- CTRL+D / CTRL+W key combinations for boot selection 
- Chromebox hardware modification to control power toggle switch with Arduino
