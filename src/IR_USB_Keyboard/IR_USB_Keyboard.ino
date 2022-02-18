/*  -*- mode: c++; c-basic-offset: 4; -*-
The MIT License (MIT)

Copyright (c) 2016 Markus Zehnder
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

/* IR to USB keyboard optimized for KODI openELEC running on Asus Chromebox.
 * IR device: Logitech Harmony 880 with remote profile: 'Intel NUC IntelD54250WYK'
 * 
 * DEVELOPMENT BRANCH for MCE remotes with RC6 IR protocol.
 * Uses IRremote library instead of IRLremote for RC6 protocol.
 * 
 * Tested with: 
 * - SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
 * - Teensy 3.2, ARM Cortex-M4 (3.3V, 96MHz)
 * - Arduino IDE 1.8.15
 * - HID Project 2.4.4 https://github.com/NicoHood/HID
 * - IRremote    3.5.2 https://github.com/Arduino-IRremote/Arduino-IRremote
 * - Teensyduino 1.56  https://www.pjrc.com/teensy/td_download.html
 * 
 * ISSUES:
 * - work in progress, not yet fully tested
 * - only tested with MCE profile
 */
// --------GLOBAL FLAGS ---------------
// enable debugging output over usb
#define DEBUG_SKETCH

// --------INCLUDES ---------------
#include "KeyboardConfig.h"

/* Save resources and processing overhead by setting compile options before including IRremote:
 *  https://github.com/Arduino-IRremote/Arduino-IRremote#compile-options--macros-for-this-library
 * Only DECODE_RC6 is required for MCE and INT422 remotes
 * e.g. can likely omit higher overhead protocols:
 * #define EXCLUDE_EXOTIC_PROTOCOLS 0 (saves around 2000 bytes program space)
 */
#include <IRremote.hpp>

#include "Debug.h"
#include "MCE.h"

// --------CONSTANTS ---------------

// pin assignments
#define RECV_PIN        7
#define POWER_SENSE_PIN 8
#define POWER_BTN_PIN   9

// min 500ms hold time seems to be required  (< 500ms was too short for power on)
const int POWER_BTN_HOLD_TIME = 700;

// Mapping of received IR values to keyboard commands
struct CodeMap {
  unsigned long   irCommand;  // received IR value
  byte            modifier;   // keyboard modifier to send
  KeyboardKeycode keyCode;    // keyboard key code to send
};

// TODO fine tune MIN_KEY_PRESS_TIME to specific setup.
// Key press time must be slightly higher then the repeat IR code duration. 
// My Logitech resends the first repeat after 40ms and afterwards every 110ms.
// The value must be high enough to correctly handle repeat IR codes but not too high to trigger a repeated keyboard key for a single IR code.
const int MIN_KEY_PRESS_TIME = 150;

// TODO fine tune LOOP_DELAY to specific setup.
// IrReceiver.decode should return a minimum of IrReceiver.decodedIRData.protocol == UNKNOWN
// Try to increase delay to improve reliability, decrease if key repeat handling is sluggish
const int LOOP_DELAY = 100;

// IR code to key mapping
const byte KEY_CTRL  = 1;
const byte KEY_ALT   = 2;
const byte KEY_SHIFT = 4;
const byte KEY_GUI   = 8;

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
// TODO remap for MCE remote
//  {REMOTE_REPLAY  , 0, KEY_COMMA},
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
/* TODO function key mappings for MCE remote
  {REMOTE_F2      , 0, KEY_D},             // Move item down (Playlist editor & Favorites window)
  {REMOTE_F3      , 0, KEY_U},             // Move item up (Playlist editor & Favorites window)  
  {REMOTE_F4      , 0, KEY_Q},             // Queue
  {REMOTE_F7      , 0, KEY_HOME},          // Jump to the top of the menu
*/
  {REMOTE_F5      , 0, KEY_V},             // Teletext / Visualisation settings
  {REMOTE_F6      , 0, KEY_Y},             // Switch/choose player
  {REMOTE_REC     , 0, KEY_PRINTSCREEN},   // Screenshot
  {REMOTE_ARROW_DOWN, KEY_CTRL, KEY_DOWN_ARROW}, // Move subtitles down
  {REMOTE_ARROW_UP  , KEY_CTRL, KEY_UP_ARROW},   // Move subtitles up
//{REMOTE_F8        , KEY_CTRL, KEY_D},          // boot: ChromeOS    TODO test...
//{REMOTE_F9        , KEY_CTRL, KEY_W},          // boot: openELEC    TODO test...
};


const int IR_KEY_MAP_SIZE = sizeof(irToKeyMap) / sizeof(CodeMap);

//------------ VARIABLES ---------------------

unsigned long lastIRValue = 0;          // previously received IR code value for scancode matching (normalized, without toggle bit)
unsigned long lastOriginalIRValue = 0;  // previously received IR code value in its original form (with toggle bit)
unsigned long timeKeyDown = 0;          // time of key press initiation
unsigned long timeLastKeyEvent = 0;     // time of last key press event
boolean decoded = false;                // whether decoded IR data is available

//========================================

void setup() {                
    // open debug console
    #ifdef DEBUG_SKETCH
      delay(3000); // serial device is not immediatly ready after power up
      Serial.begin(115200);  // afaik the baudrate is ignored on a 32u4
    #endif
    DEBUG_PRINTLN("Chromebox_REMOTE_USB_Keyboard");

    pinMode(POWER_SENSE_PIN, INPUT); 
    pinMode(POWER_BTN_PIN, OUTPUT); 
    
    // initialize control over the keyboard:
    BootKeyboard.begin();
    
    // Start the receiver
    #ifdef DEBUG_SKETCH
      IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
    #else
      IrReceiver.begin(RECV_PIN, DISABLE_LED_FEEDBACK);
    #endif
}

//========================================

void loop() {
    // Poll IR receiver for new decoded value
    decoded = IrReceiver.decode();

    if (preProcessIrData()) {
      handleIrValue(IrReceiver.decodedIRData.decodedRawData);
    }

    handlePressedKeys();

    // continue IR decoding
    if (decoded) {
      IrReceiver.resume();
      decoded = false;
    }

    delay(LOOP_DELAY);
}

//========================================

/*
 * Pre-process received IR value:
 * - filter out all values from other protocols.
 *   The received value is set to zero if of none interest.
 * - handle protocol specific repeat codes.
 *   NEC: replace repeat value 0xFFFFFFFF with first value
 *   RC6: clear toggle bit in received value
 *
 * Returns true if a new IR value is received
 */
boolean preProcessIrData() {
    if (!decoded) {
      return false;
    }
    DEBUG_PRINT(millis()); DEBUG_PRINT(" ");
    DEBUG_PRINT(IrReceiver.decodedIRData.protocol);DEBUG_PRINT("/0x");DEBUG_PRINT(IrReceiver.decodedIRData.address, HEX);DEBUG_PRINT("/0x");DEBUG_PRINTLN(IrReceiver.decodedIRData.decodedRawData, HEX);

    // only process the IR protocol we're interested in
    if (IrReceiver.decodedIRData.protocol != irType) {
      IrReceiver.decodedIRData.decodedRawData = 0;
      // HACK: prolong hold time to avoid keyboard key release while holding a remote key. Improves reliability for sporadic IrReceiver.decodedIRData.protocol == UNKNOWN
      timeLastKeyEvent = millis();
      return false;
    }

    // handle protocol specific quirks
    switch(irType) {
      case RC5: // TODO test RC5, for now just assume the client logic is the same as RC6
      case RC6:
        /* Toggle bit handling: filter out repeat codes: http://www.sbprojects.com/knowledge/ir/rc6.php
         * From http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html :
         * "The second complication is RC5 and RC6 codes use a "toggle bit" to distinguish between a button
         * that is held down and a button that is pressed twice. While a button is held down, a code is sent.
         * When the button is released and pressed a second time, a new code with one bit flipped is sent. On
         * the third press, the original code is sent. ...
         * When receiving with the IRremote library, you will receive one of two different codes, so you must
         * clear out the toggle bit."
         */
        if (IrReceiver.decodedIRData.decodedRawData == lastOriginalIRValue) {
          handleRepeatIrCode();
          return  false;
        } else {
          // new remote key press
          lastOriginalIRValue = IrReceiver.decodedIRData.decodedRawData;
        }
        // eliminate RC6 toggle bit
        IrReceiver.decodedIRData.decodedRawData = IrReceiver.decodedIRData.decodedRawData & 0xFFFF7FFF;
        break;
      // check if it's a NEC repeat code
      case NEC:
        if (IrReceiver.decodedIRData.decodedRawData == REPEAT) {
            handleRepeatIrCode();
            return false;
        }
        break;
    }

    return true;
}

void handleRepeatIrCode() {
    IrReceiver.decodedIRData.decodedRawData = lastIRValue;
    timeLastKeyEvent = millis(); // prolong hold time to avoid keyboard key release
}

/*
 * Process given IR value.
 * Looks up keyboard key codes in irToKeyMap and presses keys.
 * IR value is stored in global variable lastIRValue.
 */
void handleIrValue(unsigned long irValue) {
    if (irValue == 0) {
      return;
    }

    if (irValue != lastIRValue) {
        // immediately release last key if a different IR value is received. We don't want multiple keys pressed at the same time.
        releaseKeys();
    }

    switch (irValue) {
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
              if (irToKeyMap[i].irCommand == irValue) {
                  // only press key if not yet pressed
                  if (timeLastKeyEvent == 0) {
                      KeyboardKeycode keyCode = irToKeyMap[i].keyCode;
                      DEBUG_PRINT(" Press key: 0x"); DEBUG_PRINT(keyCode, HEX);

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
                      timeKeyDown = millis();
                  }
                  timeLastKeyEvent = millis();
                  break;
              }
          }
    } // end switch irValue

    lastIRValue = irValue;
}

//========================================

/*
 * Check if it's time to release any previously pressed keys.
 */
void handlePressedKeys() {
    if (timeLastKeyEvent > 0 && (millis() - timeLastKeyEvent >= MIN_KEY_PRESS_TIME)) {
        releaseKeys();
    }
}

/*
 * Release all pressed keyboard keys and reset key event timestamps.
 */
void releaseKeys() {
    BootKeyboard.releaseAll();
    if (timeKeyDown > 0) {
      DEBUG_PRINT(" Released keys after "); DEBUG_PRINTLN(millis() - timeKeyDown);
      // clear last IR value to stop repeat handling, i.e. start a new keypress even if the same IR value is received again.
      lastIRValue = 0;
    }
    timeKeyDown = 0;
    timeLastKeyEvent = 0;
}

//========================================

/*
 * Push power button for defined time in POWER_BTN_HOLD_TIME.
 * Key event times are set to current timestamp.
 */
void pushPowerButton() {
    timeLastKeyEvent = timeKeyDown = millis();
    digitalWrite(POWER_BTN_PIN, HIGH);
    delay(POWER_BTN_HOLD_TIME);
    digitalWrite(POWER_BTN_PIN, LOW);
    DEBUG_PRINTLN("power button triggered");
}

//========================================
