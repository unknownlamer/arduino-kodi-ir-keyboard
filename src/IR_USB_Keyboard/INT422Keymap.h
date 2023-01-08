// Clinton Ebadi's <clinton@unknownlamer.org> preferred keymapping for
// INT422 for Kodi

// Kodi keyboard controls: http://kodi.wiki/view/Keyboard_controls
const CodeMap irToKeyMap[] = {
  {REMOTE_LEFT    , 0, KEY_LEFT_ARROW},
  {REMOTE_RIGHT   , 0, KEY_RIGHT_ARROW},
  {REMOTE_UP      , 0, KEY_UP_ARROW},
  {REMOTE_DOWN    , 0, KEY_DOWN_ARROW},
  {REMOTE_OK      , 0, KEY_RETURN},
  {REMOTE_ENTER   , 0, KEY_TAB},           // Fullscreen playback
  {REMOTE_MENU    , 0, KEY_C},
  {REMOTE_EXIT    , 0, KEY_BACKSPACE},
  {REMOTE_CLEAR   , 0, KEY_ESC},
  {REMOTE_GUIDE   , 0, KEY_E},
  {REMOTE_INFO    , 0, KEY_I},
  {REMOTE_STOP    , 0, KEY_X},
  {REMOTE_PLAY    , 0, KEY_P},
  {REMOTE_PAUSE   , 0, KEY_SPACE},
  {REMOTE_REC     , 0, KEY_PRINTSCREEN},   // Screenshot
  {REMOTE_REW     , 0, KEY_R},
  {REMOTE_FWD     , 0, KEY_F},
  {REMOTE_PREV    , 0, KEY_QUOTE},         // FIXME doesn't seem to work with non-us keyboard layout
  {REMOTE_SKIP    , 0, KEY_PERIOD},
  {REMOTE_SUBTITLE, 0, KEY_T},             // toggle subtitles
  // Should use green for something as well
  {REMOTE_BLUE    , 0, KEY_N},             // Current playlist
  {REMOTE_RED     , 0, KEY_DELETE},
  {REMOTE_YELLOW  , 0, KEY_Q},             // Queue item
  /*
  {REMOTE_BLUE    , 0, KEY_O},             // Codec Info
  {REMOTE_RED     , 0, KEY_W},             // Marked as watched / unwatched
  {REMOTE_GREEN   , 0, KEY_S},             // shutdown / suspend / reboot menu
  {REMOTE_YELLOW  , 0, KEY_DELETE},
  */
  // KEYPAD_* would be more convenient for SMS style input, but
  // initial tests show this is affected by scroll lock and
  // keypad-less systems seem to default to scrolling when receiving
  // keypad keys (boo). Maybe add CTRL modifier?
  {REMOTE_1       , 0, KEY_1},
  {REMOTE_2       , KEY_CTRL, KEY_2},
  {REMOTE_3       , KEY_CTRL, KEY_3},
  {REMOTE_4       , KEY_CTRL, KEY_4},
  {REMOTE_5       , KEY_CTRL, KEY_5},
  {REMOTE_6       , KEY_CTRL, KEY_6},
  {REMOTE_7       , KEY_CTRL, KEY_7},
  {REMOTE_8       , KEY_CTRL, KEY_8},
  {REMOTE_9       , KEY_CTRL, KEY_9},
  {REMOTE_0       , 0, KEY_0},
  {REMOTE_CH_UP   , 0, KEY_PAGE_UP},       // PgUp / Skip to next queued video or next chapter if no videos are queued. / Increase Rating
  {REMOTE_CH_DOWN , 0, KEY_PAGE_DOWN},     // PgDown / Skip to previous queued video or previous chapter if no videos are queued. / Decrease Rating
  {REMOTE_ASPECT  , 0, KEY_Z},             // Zoom/aspect ratio
  {REMOTE_MUTE    , 0, KEY_VOLUME_MUTE},
  {REMOTE_VOL_UP  , 0, KEY_VOLUME_UP},
  {REMOTE_VOL_DOWN, 0, KEY_VOLUME_DOWN},
  //{REMOTE_F1      , 0, KEY_Q},
  {REMOTE_F1      , 0, KEY_L},             // Next subtitle
  {REMOTE_F2      , 0, KEY_U},             // Move item down (Playlist editor & Favorites window)
  {REMOTE_F4      , 0, KEY_D},             // Move item up (Playlist editor & Favorites window)
  {REMOTE_F3      , 0, KEY_HOME},          // Jump to the top of the menu
  //{REMOTE_F3      , 0, KEY_DELETE},
  {REMOTE_F5      , 0, KEY_A},             // Audio delay control

  {REMOTE_POWER_TOGGLE, 0, KEY_S},         // Shutdown menu

  //{REMOTE_F5      , 0, KEY_V},             // Teletext / Visualisation settings
  //  {REMOTE_ARROW_DOWN, KEY_CTRL, KEY_DOWN_ARROW}, // Move subtitles down
  //  {REMOTE_ARROW_UP  , KEY_CTRL, KEY_UP_ARROW},   // Move subtitles up
};
