/* Keyboard Driver Configuration -- Part of IR USB Keyboard
The MIT License (MIT)

Copyright (c) 2022 Clinton Ebadi <clinton@unknownlamer.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

// HID Project does not work on Teensy 3.x, but Teensyduino its own
// Keyboard library that is adequately compatible with HID Project
// BootKeyboard

#if defined(ARDUINO_TEENSY30) \
  || defined(ARDUINO_TEENSY32) \
  || defined(ARDUINO_TEENSY35) \
  || defined(ARDUINO_TEENSY36)

#include <Keyboard.h>
#define BootKeyboard Keyboard
typedef uint16_t KeyboardKeycode;

const KeyboardKeycode KEY_VOLUME_MUTE = 0x7F;
const KeyboardKeycode KEY_VOLUME_UP   = 0x80;
const KeyboardKeycode KEY_VOLUME_DOWN = 0x81;

#else

#include <HID-Project.h>

#endif

#endif
