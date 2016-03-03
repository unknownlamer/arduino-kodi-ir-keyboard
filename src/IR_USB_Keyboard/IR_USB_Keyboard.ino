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
 * IR device: Logitech Harmony 880 with remote profile: 'Plex Player'
 * Tested with SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
 * 
 * Initial proof of concept: 
 * - Kodi control works pretty well already :-)
 * - key repeat works
 * - boot protocol mode works (tested on a MacMini)
 * 
 * ISSUES:
 * - Sketch doesn't always start after power up!? 
 *   Needs more testing...
 * 
 * TODO:
 * - Power off / sleep / wakeup
 * - CTRL+D / CTRL+W key combinations for boot selection on the Asus Chromebox
 * - Chromebox hardware modification to control power toggle switch with Arduino
 */

#include <HID-Project.h>
#include <IRremote.h>

// --------CONSTANTS ---------------

// pin assignments
#define RECV_PIN 7

struct CodeMap {
  unsigned long   irCode;
  KeyboardKeycode keyCode;
};

// Logitech "Plex Player" device as KODI remote
#define IR_CLEAR        0x77E12059
#define IR_ENTER        0x77E14099
#define IR_LEFT         0x77E11069
#define IR_RIGHT        0x77E1E069
#define IR_UP           0x77E1D069
#define IR_DOWN         0x77E1B069
#define IR_OK           0x77E12069
#define IR_MENU         0x77E14069
#define IR_EXIT         0x77E150D9
#define IR_POWER_OFF    0x77E14059
#define IR_GUIDE        0x77E1A0B9
#define IR_INFO         0x77E1D099
#define IR_STOP         0x77E1A0E9
#define IR_PLAY         0x77E1C0E9
#define IR_PAUSE        0x77E1C019
#define IR_REC          0x77E130D9
#define IR_REW          0x77E1D059
#define IR_FWD          0x77E1B059
#define IR_REPLAY       0x77E1D005
#define IR_SKIP         0x77E1B005
#define IR_PREV         0x77E1B099
#define IR_PGDOWN       0x77E1B0F9
#define IR_PGUP         0x77E1D0F9
#define IR_BLUE         0x77E140F9
#define IR_RED          0x77E110F9
#define IR_GREEN        0x77E1E0F9
#define IR_YELLOW       0x77E120F9
#define IR_1            0x77E150E9
#define IR_2            0x77E130E9
#define IR_3            0x77E190E9
#define IR_4            0x77E160E9
#define IR_5            0x77E19019
#define IR_6            0x77E16019
#define IR_7            0x77E11099
#define IR_8            0x77E1E099
#define IR_9            0x77E11059
#define IR_0            0x77E1E059
#define IR_F1           0x77E190D9
#define IR_F2           0x77E160D9
#define IR_F3           0x77E1A0D9
#define IR_F4           0x77E1C0D9
#define IR_F5           0x77E11005
#define IR_F6           0x77E1E005
#define IR_F7           0x77E12005
#define IR_F8           0x77E14005
#define IR_MUTE         0x77E1A019
#define IR_VOL_UP       0x77E15019
#define IR_VOL_DOWN     0x77E13019
#define IR_CH_UP        0x77E15079
#define IR_CH_DOWN      0x77E13079
#define IR_POWER_TOGGLE 0x77E1C0B9
#define IR_SLEEP        0x77E14059
#define IR_QUEUE        0x77E130B9
#define IR_ASPECT       0x77E150B9

// Key press time must be slightly higher then the repeat IR code duration. 
// My Logitech resends the first repeat after 40ms and afterwards every 110ms
const int KEY_PRESS_TIME = 150;

// IR code to key mapping
// Kodi keyboard controls: http://kodi.wiki/view/Keyboard_controls
const CodeMap irToKeyMap[] = {
  {IR_LEFT  , KEY_LEFT_ARROW},
  {IR_RIGHT , KEY_RIGHT_ARROW},
  {IR_UP    , HID_KEYBOARD_UPARROW},
  {IR_DOWN  , KEY_DOWN_ARROW},
  {IR_OK    , KEY_RETURN},
  {IR_ENTER , KEY_TAB},  // Fullscreen playback
  {IR_MENU  , KEY_C},
  {IR_EXIT  , KEY_ESC},
  {IR_F8    , KEY_BACKSPACE},
  {IR_GUIDE , KEY_E},
  {IR_INFO  , KEY_I},
  {IR_STOP  , KEY_X},
  {IR_PLAY  , KEY_P},
  {IR_PAUSE , KEY_SPACE},
  {IR_REC   , KEY_B},
  {IR_REW   , KEY_R},
  {IR_FWD   , KEY_F},
  {IR_PREV  , KEY_QUOTE}, // FIXME doesn't seem to work with non-us keyboard layout
  {IR_SKIP  , KEY_PERIOD},
  {IR_REPLAY, KEY_COMMA},
  {IR_PGDOWN, KEY_PAGE_DOWN},
  {IR_PGUP  , KEY_PAGE_UP},
  {IR_BLUE  , KEY_T},      // toggle subtitles 
  {IR_RED   , KEY_W},      // Marked as watched / unwatched
  {IR_GREEN , KEY_S},      // Shutdown menu
  {IR_YELLOW, KEY_DELETE},
  {IR_1     , KEY_1},
  {IR_2     , KEY_2},
  {IR_3     , KEY_3},
  {IR_4     , KEY_4},
  {IR_5     , KEY_5},
  {IR_6     , KEY_6},
  {IR_7     , KEY_7},
  {IR_8     , KEY_8},
  {IR_9     , KEY_9},
  {IR_0     , KEY_0},

// TODO:
// CTRL + D -> boot ChromeOS
// CTRL + W -> boot openELEC
};

const int IR_KEY_MAP_SIZE = sizeof(irToKeyMap) / sizeof(CodeMap);

//------------ VARIABLES ---------------------

IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long lastIRValue = 0;  // previously received IR code value
unsigned long timeKeyDown = 0;  // time of key press initiation

//========================================

void setup() {                
  // open debug console
  Serial.begin(115200);  // afaik the baudrate is ignored on a 32u4
  Serial.println("Chromebox_IR_USB_Keyboard");

  // initialize control over the keyboard:
  BootKeyboard.begin();
  
  // Start the receiver
  irrecv.enableIRIn();
}

//========================================

void loop() {
    if (irrecv.decode(&results) && results.decode_type == NEC) {
      // check if it's a NEC repeat code
      if (results.value == 0xFFFFFFFF) {
        results.value = lastIRValue;
      } else {
        lastIRValue = results.value;
      }

      if (results.value != lastIRValue) {
        // immediately release last key if a different IR value is received. We don't want multiple keys pressed at the same time.
        releaseKeys(); 
      }

      for (int i = 0; i < IR_KEY_MAP_SIZE; i++) {
        if (irToKeyMap[i].irCode == results.value) {
            // only press key if not yet pressed
            if (timeKeyDown == 0) {
              KeyboardKeycode keyCode = irToKeyMap[i].keyCode;
              Serial.print(millis()); Serial.print(" Press key: 0x"); Serial.println(keyCode, HEX);
              BootKeyboard.press(keyCode);              
            }
            timeKeyDown = millis();
            break;
        }
      }

      Serial.print(millis()); Serial.print(" IR: 0x"); Serial.println(results.value, HEX);

      irrecv.resume(); // restarts decoding state machine
    } else {
      // check if it's time to release a previously pressed key
      if (timeKeyDown > 0 && (millis() - timeKeyDown >= KEY_PRESS_TIME)) {
        releaseKeys(); 
      }
    }
}

//========================================

void releaseKeys() {
    timeKeyDown = 0;
    BootKeyboard.releaseAll();
    Serial.print(millis()); Serial.println(" Release keys");   
}

