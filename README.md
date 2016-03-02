# arduino-kodi-ir-keyboard
Kodi optimized infrared USB keyboard for an Asus Chromebox running openELEC.

IR device: Logitech Harmony 880 with remote profile: 'Plex Player'

Tested with MattairTech MT-DB-U4 (ATmega 32U4), 16MHZ 5V, CDC_HID

Initial proof of concept: 
- Kodi control works pretty well already :-)
- key repeat works

## TODO:
- Power off / sleep / wakeup
- CTRL+D / CTRL+W key combinations for boot selection 
- Test if boot protocol mode HID modification is required

  Required change in hardware/arduino/cores/arduino/HID.cpp

        extern const HIDDescriptor _hidInterface PROGMEM;
        const HIDDescriptor _hidInterface =
        {
        // D_INTERFACE(HID_INTERFACE,1,3,0,0),
          D_INTERFACE(HID_INTERFACE,1,3,1,1),
          D_HIDREPORT(sizeof(_hidReportDescriptor)),
          D_ENDPOINT(USB_ENDPOINT_IN (HID_ENDPOINT_INT),USB_ENDPOINT_TYPE_INTERRUPT,0x40,0x01)
        };

- Chromebox hardware modification to control power toggle switch with Arduino
