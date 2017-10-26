/*
The MIT License (MIT)

Copyright (c) 2016-2017 Markus Zehnder

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

/* IR to USB keyboard optimized for KODI LibreELEC running on Asus Chromebox.
 * IR device: Logitech Harmony 880 with remote profile: 'Panasonic TV TX-43CXW754'
 * 
 * Development branch for new IRLremote 2.0.0 library
 *
 * Tested with: 
 * - Arduino Leonardo & SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
 * - Arduino IDE 1.8.5
 * - HID Project 2.4.4
 * - IRLremote   2.0.0
 * 
 * ISSUES:
 * - IRLremote doesn't work with Logitech Plex Remote profile with RC6 codes.
 *   Now (ab)using Panasonic TV TX-43CXW754 device template with NEC codes.
 */
// --------GLOBAL FLAGS ---------------
// enable debugging output over usb
#define DEBUG_SKETCH

// --------INCLUDES ---------------

#include <HID-Project.h>
#include <IRLremote.h>
#include "Debug.h"
#include "Panasonic.h"

// --------CONSTANTS ---------------

// pin assignments
#define RECV_PIN        7
#define POWER_SENSE_PIN 8
#define POWER_BTN_PIN   9

// min 500ms hold time seems to be required  (< 500ms was too short for power on)
#define POWER_BTN_HOLD_TIME 700

struct CodeMap {
  uint32_t        irCommand;
  uint8_t         modifier;
  KeyboardKeycode keyCode;
};

// TODO fine tune MIN_KEY_PRESS_TIME to specific setup.
// Key press time must be slightly higher then the repeat IR code duration. 
// My Logitech resends the first repeat after 40ms and afterwards every 110ms.
// The value must be high enough to correctly handle repeat IR codes (i.e. not releasing the pressed keyboard key while still receiving a repeat code)
// but not too high to trigger a repeated keyboard key for a single IR code and create a sluggish feeling.
const int KEY_PRESS_TIME = 300;

// TODO fine tune LOOP_DELAY to specific setup.
// Try to increase delay to improve reliability, decrease if key repeat handling is sluggish
const int LOOP_DELAY = 100;

// IR code to key mapping
const uint8_t KEY_CTRL  = 1;
const uint8_t KEY_ALT   = 2;
const uint8_t KEY_SHIFT = 4;
const uint8_t KEY_GUI   = 8;

// Kodi keyboard controls: http://kodi.wiki/view/Keyboard_controls
const CodeMap irToKeyMap[] = {
  {REMOTE_LEFT    , 0, KEY_LEFT_ARROW},
  {REMOTE_RIGHT   , 0, KEY_RIGHT_ARROW},
  {REMOTE_UP      , 0, KEY_UP_ARROW},
  {REMOTE_DOWN    , 0, KEY_DOWN_ARROW},
  {REMOTE_OK      , 0, KEY_RETURN},
  {REMOTE_ENTER   , 0, KEY_TAB},           // Fullscreen playback
  {REMOTE_MENU    , 0, KEY_C},
  {REMOTE_CLEAR   , 0, KEY_BACKSPACE},
  {REMOTE_EXIT    , 0, KEY_ESC},
  {REMOTE_GUIDE   , 0, KEY_E},
  {REMOTE_INFO    , 0, KEY_I},
  {REMOTE_STOP    , 0, KEY_X},
  {REMOTE_PLAY    , 0, KEY_P},
  {REMOTE_PAUSE   , 0, KEY_SPACE},
  {REMOTE_REC     , 0, KEY_B},
  {REMOTE_REW     , 0, KEY_R},
  {REMOTE_FWD     , 0, KEY_F},
  {REMOTE_PREV    , 0, KEY_QUOTE},         // FIXME doesn't seem to work with non-us keyboard layout
  {REMOTE_SKIP    , 0, KEY_PERIOD},
  {REMOTE_REPLAY  , 0, KEY_COMMA},
  {REMOTE_SUBTITLE, 0, KEY_T},             // toggle subtitles 
  {REMOTE_BLUE    , 0, KEY_O},             // Codec Info
  {REMOTE_RED     , 0, KEY_W},             // Marked as watched / unwatched
  {REMOTE_GREEN   , 0, KEY_S},             // shutdown / suspend / reboot menu
  {REMOTE_YELLOW  , 0, KEY_DELETE},
  {REMOTE_1       , 0, KEY_1},
  {REMOTE_2       , 0, KEY_2},
  {REMOTE_3       , 0, KEY_3},
  {REMOTE_4       , 0, KEY_4},
  {REMOTE_5       , 0, KEY_5},
  {REMOTE_6       , 0, KEY_6},
  {REMOTE_7       , 0, KEY_7},
  {REMOTE_8       , 0, KEY_8},
  {REMOTE_9       , 0, KEY_9},
  {REMOTE_0       , 0, KEY_0},
  {REMOTE_CH_UP   , 0, KEY_PAGE_UP},       // PgUp / Skip to next queued video or next chapter if no videos are queued. / Increase Rating
  {REMOTE_CH_DOWN , 0, KEY_PAGE_DOWN},     // PgDown / Skip to previous queued video or previous chapter if no videos are queued. / Decrease Rating
  {REMOTE_ASPECT  , 0, KEY_Z},             // Zoom/aspect ratio
  {REMOTE_MUTE    , 0, KEY_VOLUME_MUTE},
  {REMOTE_VOL_UP  , 0, KEY_VOLUME_UP},
  {REMOTE_VOL_DOWN, 0, KEY_VOLUME_DOWN},
  {REMOTE_F1      , 0, KEY_A},             // Audio delay control
  {REMOTE_F2      , 0, KEY_D},             // Move item down (Playlist editor & Favorites window)
  {REMOTE_F3      , 0, KEY_U},             // Move item up (Playlist editor & Favorites window)  
  {REMOTE_F4      , 0, KEY_Q},             // Queue
  {REMOTE_F5      , 0, KEY_V},             // Teletext / Visualisation settings
  {REMOTE_F6      , 0, KEY_Y},             // Switch/choose player
  {REMOTE_F7      , 0, KEY_HOME},          // Jump to the top of the menu
  {REMOTE_REC     , 0, KEY_PRINTSCREEN},   // Screenshot

  {REMOTE_ARROW_DOWN, KEY_CTRL, KEY_DOWN_ARROW}, // Move subtitles down
  {REMOTE_ARROW_UP  , KEY_CTRL, KEY_UP_ARROW},   // Move subtitles up
  {REMOTE_F8        , KEY_CTRL, KEY_D},          // boot: ChromeOS    TODO test...
//{REMOTE_F9        , KEY_CTRL, KEY_W},          // boot: openELEC    TODO test...
};

const int IR_KEY_MAP_SIZE = sizeof(irToKeyMap) / sizeof(CodeMap);

//------------ VARIABLES ---------------------

unsigned long lastIRValue = 0;  // previously received IR code value
unsigned long timeKeyDown = 0;  // time of key press initiation

//========================================

void setup() {                
    // open debug console
    #ifdef DEBUG_SKETCH
      delay(3000);
      Serial.begin(115200);  // afaik the baudrate is ignored on a 32u4
    #endif
    DEBUG_PRINTLN("Chromebox_REMOTE_USB_Keyboard");

    pinMode(POWER_SENSE_PIN, INPUT); 
    pinMode(POWER_BTN_PIN, OUTPUT); 
    
    // initialize control over the keyboard:
    BootKeyboard.begin();
    
    // Start the receiver
    if (!IRLremote.begin(RECV_PIN)) {
        DEBUG_PRINTLN(F("Invalid RECV_PIN"));
    }
}

//========================================

void loop() {
    if (IRLremote.available()) {
        auto data = IRLremote.read();
        DEBUG_PRINT(millis()); DEBUG_PRINT(" "); DEBUG_PRINT(data.address, HEX);DEBUG_PRINT("/0x");DEBUG_PRINTLN(data.command, HEX);

        // check if it's a NEC repeat code. TODO still required with IRLremote 2?
        if (data.command == 0xFFFF) {
            data.command = lastIRValue;
        }

        if (data.command != lastIRValue) {
            DEBUG_PRINTLN("IRValue changed: releasing keys");
            // immediately release last key if a different IR value is received. We don't want multiple keys pressed at the same time.
            releaseKeys();
        }
        
        switch (data.command) {
          // special commands
          case REMOTE_POWER_TOGGLE :
              DEBUG_PRINTLN("Power toggle");
              pushPowerButton();
              break;
          case REMOTE_POWER_OFF :
              DEBUG_PRINT("Power off: ");
              if (digitalRead(POWER_SENSE_PIN) == HIGH) {
                pushPowerButton();
              } else {
                DEBUG_PRINTLN("ignored, device already off");
              }
              break;
          case REMOTE_POWER_ON :
              DEBUG_PRINT("Power on: ");
              if (digitalRead(POWER_SENSE_PIN) == LOW) {
                pushPowerButton();
              } else {
                DEBUG_PRINTLN("ignored, device already on");
              }
              break;
          // keyboard commands
          default :
              for (int i = 0; i < IR_KEY_MAP_SIZE; i++) {
                  if (irToKeyMap[i].irCommand == data.command) {
                      // only press key if not yet pressed
                      if (timeKeyDown == 0) {
                          KeyboardKeycode keyCode = irToKeyMap[i].keyCode;
                          DEBUG_PRINT(millis()); DEBUG_PRINT(" Press key: 0x"); DEBUG_PRINT(keyCode, HEX);

                          if (irToKeyMap[i].modifier & KEY_CTRL) {
                            BootKeyboard.press(KEY_LEFT_CTRL);
                            DEBUG_PRINT(" + CTRL ");
                          }
                          if (irToKeyMap[i].modifier & KEY_ALT) {
                            BootKeyboard.press(KEY_LEFT_ALT);
                            DEBUG_PRINT(" + ALT ");
                          }
                          if (irToKeyMap[i].modifier & KEY_SHIFT) {
                            BootKeyboard.press(KEY_LEFT_SHIFT);
                            DEBUG_PRINT(" + SHIFT ");
                          }
                          if (irToKeyMap[i].modifier & KEY_GUI) {
                            BootKeyboard.press(KEY_LEFT_GUI);
                            DEBUG_PRINT(" + GUI ");
                          }
                          DEBUG_PRINTLN();
                          BootKeyboard.press(keyCode);
                      }
                      timeKeyDown = millis();
                      break;
                  }
              }
        }

        lastIRValue = data.command;

    } else {
        // check if it's time to release a previously pressed key
        if (timeKeyDown > 0 && (millis() - timeKeyDown >= KEY_PRESS_TIME)) {
            DEBUG_PRINT(millis()); DEBUG_PRINTLN(" Keypress timeout: releasing keys");
            releaseKeys(); 
        }
    }

    delay(LOOP_DELAY);
}

//========================================

void releaseKeys() {
    timeKeyDown = 0;
    BootKeyboard.releaseAll();
}

//========================================

void pushPowerButton() {
    digitalWrite(POWER_BTN_PIN, HIGH);
    delay(POWER_BTN_HOLD_TIME);
    digitalWrite(POWER_BTN_PIN, LOW);
    DEBUG_PRINTLN("power button triggered");
}

//========================================

