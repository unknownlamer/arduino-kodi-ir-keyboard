/*
The MIT License (MIT)

Copyright (c) 2016 Markus Zehnder

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
 * - Arduino IDE 1.8.5
 * - HID Project 2.4.4
 * - IRremote   2.2.3
 * 
 * ISSUES:
 * - work in progress, not yet fully tested
 * - only tested with MCE profile
 */
// --------GLOBAL FLAGS ---------------
// enable debugging output over usb
#define DEBUG_SKETCH

// --------INCLUDES ---------------
#include <HID-Project.h>

/* Save resources and processing overhead with changing IRremote.h:
 * Only DECODE_RC6 is required for MCE remote (saves around 2900 bytes of program storage space)
#define DECODE_RC5           0
#define SEND_RC5             0
#define DECODE_RC6           1
#define SEND_RC6             0
#define DECODE_NEC           0
#define SEND_NEC             0
#define DECODE_SONY          0
#define SEND_SONY            0
#define DECODE_PANASONIC     0
#define SEND_PANASONIC       0
#define DECODE_JVC           0
#define SEND_JVC             0
#define DECODE_SAMSUNG       0
#define SEND_SAMSUNG         0
#define DECODE_WHYNTER       0
#define SEND_WHYNTER         0
#define DECODE_AIWA_RC_T501  0
#define SEND_AIWA_RC_T501    0
#define DECODE_LG            0
#define SEND_LG              0
#define DECODE_SANYO         0
#define SEND_SANYO           0 // NOT WRITTEN
#define DECODE_MITSUBISHI    0
#define SEND_MITSUBISHI      0 // NOT WRITTEN
#define DECODE_DISH          0 // NOT WRITTEN
#define SEND_DISH            0
#define DECODE_SHARP         0 // NOT WRITTEN
#define SEND_SHARP           0
#define DECODE_DENON         0
#define SEND_DENON           0
*/
#include <IRremote.h>

#include "Debug.h"
// include desired IR code to key mapping definitions (MCE, Panasonic, Sony)
#include "MCE.h"

// --------CONSTANTS ---------------

// pin assignments
#define RECV_PIN 7
#define POWER_SENSE_PIN 8
#define POWER_BTN_PIN 9

// min 500ms hold time seems to be required  (< 500ms was too short for power on)
#define POWER_BTN_HOLD_TIME 700

struct CodeMap {
  uint32_t        irCommand;
  uint8_t         modifier;
  KeyboardKeycode keyCode;
};

// Key press time must be slightly higher then the repeat IR code duration. 
// My Logitech resends the first repeat after 40ms and afterwards every 110ms
const int KEY_PRESS_TIME = 150;

const int LOOP_DELAY = KEY_PRESS_TIME / 3;

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
/* TODO remap for MCE remote
  {REMOTE_F1      , 0, KEY_A},             // Audio delay control
  {REMOTE_F2      , 0, KEY_D},             // Move item down (Playlist editor & Favorites window)
  {REMOTE_F3      , 0, KEY_U},             // Move item up (Playlist editor & Favorites window)  
  {REMOTE_F4      , 0, KEY_Q},             // Queue
  {REMOTE_F5      , 0, KEY_V},             // Teletext / Visualisation settings
  {REMOTE_F6      , 0, KEY_Y},             // Switch/choose player
  {REMOTE_F7      , 0, KEY_HOME},          // Jump to the top of the menu
*/
  {REMOTE_REC     , 0, KEY_PRINTSCREEN},   // Screenshot
/* TODO remap for MCE remote
  {REMOTE_ARROW_DOWN, KEY_CTRL, KEY_DOWN_ARROW}, // Move subtitles down
  {REMOTE_ARROW_UP  , KEY_CTRL, KEY_UP_ARROW},   // Move subtitles up
*/
  {REMOTE_F8        , KEY_CTRL, KEY_D},          // boot: ChromeOS    TODO test...
//{REMOTE_F9        , KEY_CTRL, KEY_W},          // boot: openELEC    TODO test...
};


const int IR_KEY_MAP_SIZE = sizeof(irToKeyMap) / sizeof(CodeMap);

//------------ VARIABLES ---------------------

unsigned long lastIRValue = 0;  // previously received IR code value
unsigned long timeKeyDown = 0;  // time of key press initiation
unsigned long timeLastKeyEvent = 0;  // time of last key press event

//========================================

IRrecv irrecv(RECV_PIN);
decode_results  results;


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
    irrecv.enableIRIn();
}

//========================================

void loop() {
    if (irrecv.decode(&results)) {

        DEBUG_PRINT(results.decode_type);DEBUG_PRINT("/0x");DEBUG_PRINT(results.address, HEX);DEBUG_PRINT("/0x");DEBUG_PRINTLN(results.value, HEX);

        if (results.decode_type == irType) {
            // handle protocol specific quirks
            switch(irType) {
              case RC6:
                // TODO filter out repeat codes, each code is sent three times with the same toggle bit
                // eliminate RC6 toggle bit
                results.value = results.value & 0xFFFF7FFF;
                break;
              // check if it's a NEC repeat code
              case NEC:
              case PANASONIC:
                if (results.value == 0xFFFF) {
                    results.value = lastIRValue;
                }
                break;
            }

            if (results.value != lastIRValue) {
                // immediately release last key if a different IR value is received. We don't want multiple keys pressed at the same time.
                releaseKeys(); 
            } 

            switch (results.value) {
              // special commands
              case REMOTE_POWER_TOGGLE :
                  // simple way to filter out multiple RC6 repeat codes
                  if (timeLastKeyEvent == 0) {
                      DEBUG_PRINTLN("Power toggle");
                      pushPowerButton();
                  }
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
                      if (irToKeyMap[i].irCommand == results.value) {
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
            } // end switch result.value
        } // end decode_type

        lastIRValue = results.value;

        irrecv.resume(); // Receive the next value

    } else {
        // check if it's time to release a previously pressed key
        if (timeLastKeyEvent > 0 && (millis() - timeLastKeyEvent >= KEY_PRESS_TIME)) {
            releaseKeys(); 
        }
    }

    delay(LOOP_DELAY);
}

//========================================

void releaseKeys() {
    BootKeyboard.releaseAll();
    if (timeLastKeyEvent > 0) {
      DEBUG_PRINT(" Released keys after "); DEBUG_PRINTLN(millis() - timeKeyDown);
    }
    timeKeyDown = 0;
    timeLastKeyEvent = 0;
}

//========================================

void pushPowerButton() {
    digitalWrite(POWER_BTN_PIN, HIGH);
    delay(POWER_BTN_HOLD_TIME);
    digitalWrite(POWER_BTN_PIN, LOW);
    DEBUG_PRINTLN("power button triggered");
    timeLastKeyEvent = timeKeyDown = millis();
}

//========================================
