/* IR code -> Keyboard key mapping -- Part of IR USB Keyboard
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

#ifndef REMOTE_KEYMAP_H
#define REMOTE_KEYMAP_H

#include "KeyboardConfig.h"   // Keyboard library config
#include "MCE.h"              // Remote control codes

// IR code to key mapping
struct CodeMap {
  unsigned long   irCommand;  // received IR value
  byte            modifier;   // keyboard modifier to send
  KeyboardKeycode keyCode;    // keyboard key code to send
};

// Modifier keys
const byte KEY_CTRL  = 1;
const byte KEY_ALT   = 2;
const byte KEY_SHIFT = 4;
const byte KEY_GUI   = 8;

#ifdef CUSTOM_KEYMAP_INCLUDE

#include CUSTOM_KEYMAP_INCLUDE

#else
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
#endif

const int IR_KEY_MAP_SIZE = sizeof(irToKeyMap) / sizeof(CodeMap);

#endif
