
const decode_type_t  irType = RC6;

// "Kodi/PC" preset (code 02000) for Inteset INT422-3 remote. It's the
// MCE codes but with a different device address and supporting a few
// additional buttons that are not enabled when the remote is using
// the MCE preset (code 01272).

#define IR_INT422_NUMERIC_0      0x800f1400
#define IR_INT422_NUMERIC_1      0x800f1401
#define IR_INT422_NUMERIC_2      0x800f1402
#define IR_INT422_NUMERIC_3      0x800f1403
#define IR_INT422_NUMERIC_4      0x800f1404
#define IR_INT422_NUMERIC_5      0x800f1405
#define IR_INT422_NUMERIC_6      0x800f1406
#define IR_INT422_NUMERIC_7      0x800f1407
#define IR_INT422_NUMERIC_8      0x800f1408
#define IR_INT422_NUMERIC_9      0x800f1409

#define IR_INT422_DELETE         0x800f140a
#define IR_INT422_ENTER          0x800f140b
#define IR_INT422_SLEEP          0x800f140c      /* Formerly PC Power */
#define IR_INT422_MEDIA          0x800f140d      /* Windows MCE button */
#define IR_INT422_MUTE           0x800f140e
#define IR_INT422_INFO           0x800f140f

#define IR_INT422_VOLUMEUP       0x800f1410
#define IR_INT422_VOLUMEDOWN     0x800f1411
#define IR_INT422_CHANNELUP      0x800f1412
#define IR_INT422_CHANNELDOWN    0x800f1413

#define IR_INT422_FASTFORWARD    0x800f1414
#define IR_INT422_REWIND         0x800f1415
#define IR_INT422_PLAY           0x800f1416
#define IR_INT422_RECORD         0x800f1417
#define IR_INT422_PAUSE          0x800f1418
#define IR_INT422_STOP           0x800f1419
#define IR_INT422_NEXT           0x800f141a
#define IR_INT422_PREVIOUS       0x800f141b
#define IR_INT422_NUMERIC_POUND  0x800f141c
#define IR_INT422_NUMERIC_STAR   0x800f141d

#define IR_INT422_UP             0x800f141e
#define IR_INT422_DOWN           0x800f141f
#define IR_INT422_LEFT           0x800f1420
#define IR_INT422_RIGHT          0x800f1421

#define IR_INT422_OK             0x800f1422
#define IR_INT422_EXIT           0x800f1423
#define IR_INT422_DVD            0x800f1424
#define IR_INT422_TUNER          0x800f1425      /* LiveTV */
#define IR_INT422_EPG            0x800f1426      /* Guide */
#define IR_INT422_ZOOM           0x800f1427      /* Aspect */

#define IR_INT422_MODE           0x800f1432      /* Visualization */
#define IR_INT422_PRESENTATION   0x800f1433      /* Slide Show */
#define IR_INT422_EJECTCD        0x800f1434
#define IR_INT422_BRIGHTNESSUP   0x800f143a

#define IR_INT422_TV             0x800f1446
#define IR_INT422_AUDIO          0x800f1447      /* My Music */
#define IR_INT422_PVR            0x800f1448      /* RecordedTV */
#define IR_INT422_CAMERA         0x800f1449
#define IR_INT422_VIDEO          0x800f144a
#define IR_INT422_LANGUAGE       0x800f144c
#define IR_INT422_TITLE          0x800f144d
#define IR_INT422_PRINT          0x800f144e      /* Print - HP OEM version of remote */

#define IR_INT422_RADIO          0x800f1450

#define IR_INT422_SUBTITLE       0x800f145a      /* Caption/Teletext */
#define IR_INT422_RED            0x800f145b
#define IR_INT422_GREEN          0x800f145c
#define IR_INT422_YELLOW         0x800f145d
#define IR_INT422_BLUE           0x800f145e

#define IR_INT422_POWER2         0x800f1465      /* TV Power */
#define IR_INT422_MESSENGER      0x800f1469
#define IR_INT422_PLAYPAUSE      0x800f146e
#define IR_INT422_PLAYER         0x800f146f      /* Start media application (NEW) */

#define IR_INT422_BRIGHTNESSDOWN 0x800f1480
#define IR_INT422_PLAYPAUSE2     0x800f1481

// additional scancodes of an Intel NUC
#define IR_INT422_POWER_ON       0x800F1429
#define IR_INT422_POWER_OFF      0x800F142A

//------------ Standard Logitech 880 buttons ---------------------
#define REMOTE_MUTE         IR_INT422_MUTE
#define REMOTE_VOL_UP       IR_INT422_VOLUMEUP
#define REMOTE_VOL_DOWN     IR_INT422_VOLUMEDOWN

#define REMOTE_ARROW_DOWN   IR_INT422_BRIGHTNESSDOWN
#define REMOTE_ARROW_UP     IR_INT422_BRIGHTNESSUP

#define REMOTE_CH_DOWN      IR_INT422_CHANNELDOWN
#define REMOTE_CH_UP        IR_INT422_CHANNELUP
#define REMOTE_PREV         IR_INT422_PREVIOUS
#define REMOTE_UP           IR_INT422_UP
#define REMOTE_DOWN         IR_INT422_DOWN
#define REMOTE_LEFT         IR_INT422_LEFT
#define REMOTE_RIGHT        IR_INT422_RIGHT
#define REMOTE_OK           IR_INT422_OK
// TODO test REMOTE_MENU mapping
#define REMOTE_MENU         IR_INT422_MODE
#define REMOTE_EXIT         IR_INT422_EXIT
#define REMOTE_GUIDE        IR_INT422_EPG
#define REMOTE_INFO         IR_INT422_INFO
#define REMOTE_STOP         IR_INT422_STOP

//#define REMOTE_REPLAY       ?
#define REMOTE_SKIP         IR_INT422_NEXT
#define REMOTE_PLAY         IR_INT422_PLAY
#define REMOTE_REC          IR_INT422_RECORD
#define REMOTE_REW          IR_INT422_REWIND
#define REMOTE_FWD          IR_INT422_FASTFORWARD
#define REMOTE_PAUSE        IR_INT422_PAUSE
#define REMOTE_1            IR_INT422_NUMERIC_1
#define REMOTE_2            IR_INT422_NUMERIC_2
#define REMOTE_3            IR_INT422_NUMERIC_3
#define REMOTE_4            IR_INT422_NUMERIC_4
#define REMOTE_5            IR_INT422_NUMERIC_5
#define REMOTE_6            IR_INT422_NUMERIC_6
#define REMOTE_7            IR_INT422_NUMERIC_7
#define REMOTE_8            IR_INT422_NUMERIC_8
#define REMOTE_9            IR_INT422_NUMERIC_9
#define REMOTE_0            IR_INT422_NUMERIC_0
#define REMOTE_ENTER        IR_INT422_ENTER

#define REMOTE_POWER_TOGGLE IR_INT422_SLEEP
#define REMOTE_POWER_ON     IR_INT422_POWER_ON
#define REMOTE_POWER_OFF    IR_INT422_POWER_OFF

#define REMOTE_CLEAR        IR_INT422_DELETE
#define REMOTE_RED          IR_INT422_RED
#define REMOTE_GREEN        IR_INT422_GREEN
#define REMOTE_YELLOW       IR_INT422_YELLOW
#define REMOTE_BLUE         IR_INT422_BLUE
#define REMOTE_SUBTITLE     IR_INT422_SUBTITLE
#define REMOTE_ASPECT       IR_INT422_MEDIA

#define REMOTE_F1           IR_INT422_AUDIO
/*
#define REMOTE_F2           ?
#define REMOTE_F3           ?
#define REMOTE_F4           ?
#define REMOTE_F7           ?
*/
#define REMOTE_F5           IR_INT422_MODE
#define REMOTE_F6           IR_INT422_PLAYER
#define REMOTE_F8           IR_INT422_TV
