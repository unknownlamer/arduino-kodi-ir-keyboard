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
 * IR device: Logitech Harmony 880 with remote profile: 'Panasonic TV TX-43CXW754'
 * 
 * Initial proof of concept.
 * 
 * Testing with: 
 * - SparkFun Pro Micro, ATmega32u4 (5V, 16MHz)
 * - Arduino IDE 1.6.7
 * - HID Project 2.4.3
 * - IRLremote   1.7.4
 * 
 * ISSUES:
 * - Most promising branch yet :)
 * - IRLremote doesn't work with Logitech Plex Remote profile. Now (ab)using Panasonic TV TX-43CXW754 device template.
 * - Watch dog reset doesn't work, according to Google this is most likely an Arduino bootloader bug.
 */
// --------GLOBAL FLAGS ---------------
// enable debugging output over usb
#define DEBUG_SKETCH
//#define WATCHDOG

// hack for MattairTech MT-DB-U4 1.6.9-mt1
#ifndef USB_EP_SIZE
 #define USB_EP_SIZE USB_DEFAULT_EP_SIZE
#endif

// --------INCLUDES ---------------

#ifdef WATCHDOG
  #include <avr/wdt.h>
#endif
#include <HID-Project.h>
#include <IRLremote.h>
#include "Debug.h"
// either include Panasonic or Sony definitions
#include "Panasonic.h"
//#include "Sony.h"

// --------CONSTANTS ---------------

// pin assignments
#define RECV_PIN 7

struct CodeMap {
  uint32_t        irCommand;
  KeyboardKeycode keyCode;
};

// Key press time must be slightly higher then the repeat IR code duration. 
// My Logitech resends the first repeat after 40ms and afterwards every 110ms
const int KEY_PRESS_TIME = 150;

// TODO not sure yet if a loop delay is required. With the IRRemote library it increased realibility...
const int LOOP_DELAY = KEY_PRESS_TIME / 3;

// IR code to key mapping
// Kodi keyboard controls: http://kodi.wiki/view/Keyboard_controls
const CodeMap irToKeyMap[] = {
  {REMOTE_LEFT  , KEY_LEFT_ARROW},
  {REMOTE_RIGHT , KEY_RIGHT_ARROW},
  {REMOTE_UP    , KEY_UP_ARROW},
  {REMOTE_DOWN  , KEY_DOWN_ARROW},
  {REMOTE_OK    , KEY_RETURN},
  {REMOTE_ENTER , KEY_TAB},  // Fullscreen playback
  {REMOTE_MENU  , KEY_C},
  {REMOTE_BACK  , KEY_BACKSPACE},
  {REMOTE_EXIT  , KEY_ESC},
  {REMOTE_GUIDE , KEY_E},
  {REMOTE_INFO  , KEY_I},
  {REMOTE_STOP  , KEY_X},
  {REMOTE_PLAY  , KEY_P},
  {REMOTE_PAUSE , KEY_SPACE},
  {REMOTE_REC   , KEY_B},
  {REMOTE_REW   , KEY_R},
  {REMOTE_FWD   , KEY_F},
  {REMOTE_PREV  , KEY_QUOTE}, // FIXME doesn't seem to work with non-us keyboard layout
  {REMOTE_SKIP  , KEY_PERIOD},
  {REMOTE_REPLAY, KEY_COMMA},
  {REMOTE_BLUE  , KEY_T},      // toggle subtitles 
  {REMOTE_RED   , KEY_W},      // Marked as watched / unwatched
  {REMOTE_GREEN , KEY_S},      // Shutdown menu
  {REMOTE_YELLOW, KEY_DELETE},
  {REMOTE_1     , KEY_1},
  {REMOTE_2     , KEY_2},
  {REMOTE_3     , KEY_3},
  {REMOTE_4     , KEY_4},
  {REMOTE_5     , KEY_5},
  {REMOTE_6     , KEY_6},
  {REMOTE_7     , KEY_7},
  {REMOTE_8     , KEY_8},
  {REMOTE_9     , KEY_9},
  {REMOTE_0     , KEY_0},

// TODO:
// CTRL + D -> boot ChromeOS
// CTRL + W -> boot openELEC
};

const int IR_KEY_MAP_SIZE = sizeof(irToKeyMap) / sizeof(CodeMap);

//------------ VARIABLES ---------------------

unsigned long lastIRValue = 0;  // previously received IR code value
unsigned long timeKeyDown = 0;  // time of key press initiation

// temporary variables to save latest IR input
uint8_t IRProtocol = 0;
uint16_t IRAddress = 0;
uint32_t IRCommand = 0;

//========================================

void setup() {                
    // open debug console
    #ifdef DEBUG_SKETCH
      delay(3000);
      Serial.begin(115200);  // afaik the baudrate is ignored on a 32u4
    #endif
    DEBUG_PRINTLN("Chromebox_REMOTE_USB_Keyboard");
    
    // initialize control over the keyboard:
    BootKeyboard.begin();
    
    // Start the receiver
    attachInterrupt(digitalPinToInterrupt(RECV_PIN), IRLinterrupt<irType>, CHANGE);
    
    #ifdef WATCHDOG
      // enable watch dog 
      wdt_enable(WDTO_1S);
    #endif
}

//========================================

void loop() {
#ifdef WATCHDOG  
    //Test if watchdog interrupt enabled
    // http://forum.arduino.cc/index.php?topic=295345.msg2628807#msg2628807
    if (WDTCSR & (1<<WDIE)) {   
        //Prolong wtachdog timer
        wdt_reset();
    }
    //No interrupt enabled - Test if watchdog reset enabled
    else if (WDTCSR & (1<<WDE)) {
        //Bootloader about to reset - do not prolong watchdog
    }
    //It has ben disabled - Enable and prolong
    else {
        //Watchdog disabled - Enable again
        wdt_enable(WDTO_1S);
    }
#endif

    // temporary disable interrupts and print newest input
    // TODO read up on interrup handling
    uint8_t oldSREG = SREG;
    cli();

    if (IRProtocol) {
        DEBUG_PRINT(IRProtocol);DEBUG_PRINT("/0x");DEBUG_PRINT(IRAddress, HEX);DEBUG_PRINT("/0x");DEBUG_PRINTLN(IRCommand, HEX);
        
        if (IRProtocol == irType) {
            // check if it's a NEC repeat code
            if (IRCommand == 0xFFFF) {
                IRCommand = lastIRValue;
            } else {
                lastIRValue = IRCommand;
            }
            
            if (IRCommand != lastIRValue) {
                // immediately release last key if a different IR value is received. We don't want multiple keys pressed at the same time.
                releaseKeys(); 
            }
            
            for (int i = 0; i < IR_KEY_MAP_SIZE; i++) {
                if (irToKeyMap[i].irCommand == IRCommand) {  // irToKeyMap[i].irAddress == IRAddress && 
                    // only press key if not yet pressed
                    if (timeKeyDown == 0) {
                        KeyboardKeycode keyCode = irToKeyMap[i].keyCode;
                        DEBUG_PRINT(millis()); DEBUG_PRINT(" Press key: 0x"); DEBUG_PRINTLN(keyCode, HEX);
                        BootKeyboard.press(keyCode);              
                    }
                    timeKeyDown = millis();
                    break;
                }
            }
        }     
    
    } else {
        // check if it's time to release a previously pressed key
        if (timeKeyDown > 0 && (millis() - timeKeyDown >= KEY_PRESS_TIME)) {
            releaseKeys(); 
        }
    }

    IRProtocol = 0;
    SREG = oldSREG;
    
    sei();
    
    // TODO required?
    delay(LOOP_DELAY);
}

//========================================

void releaseKeys() {
    timeKeyDown = 0;
    BootKeyboard.releaseAll();
    DEBUG_PRINT(millis()); DEBUG_PRINTLN(" Release keys");   
}

//========================================

// IR decoding callback: update the values to the newest valid input
void IREvent(uint8_t protocol, uint16_t address, uint32_t command) {
    // called when directly received a valid IR signal.
    // do not use Serial inside, it can crash your program!
    
    // dont update value if not yet processed
    if (!IRProtocol) {
        IRProtocol = protocol;
        IRAddress = address;
        IRCommand = command;
    }
}
