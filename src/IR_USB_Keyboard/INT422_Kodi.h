
const decode_type_t  irType = RC6;

// "Kodi/PC" preset (code 02000) for Inteset INT422-3 remote. It's the
// MCE codes but with a different device address and supporting a few
// additional buttons that are not enabled when the remote is using
// the MCE preset (code 01272).

// Codes are roughly organized top -> bottom as the buttons appear on
// the remote

#define IR_INT422_POWER          0x800f140c

#define IR_INT422_MENU           0x800f140d
#define IR_INT422_INFO           0x800f140f
#define IR_INT422_GUIDE          0x800f1426
#define IR_INT422_BACK           0x800f1423

#define IR_INT422_UP             0x800f141e
#define IR_INT422_DOWN           0x800f141f
#define IR_INT422_LEFT           0x800f1420
#define IR_INT422_RIGHT          0x800f1421
#define IR_INT422_OK             0x800f1422

#define IR_INT422_GREEN          0x800f1447
#define IR_INT422_RED            0x800f1448
#define IR_INT422_YELLOW         0x800f1449
#define IR_INT422_BLUE           0x800f144a

#define IR_INT422_DISPLAY        0x800f1424
#define IR_INT422_FASTFORWARD    0x800f1414
#define IR_INT422_REWIND         0x800f1415
#define IR_INT422_PLAY           0x800f1416
#define IR_INT422_RECORD         0x800f1417
#define IR_INT422_PAUSE          0x800f1418
#define IR_INT422_STOP           0x800f1419
#define IR_INT422_SKIPFORWARD    0x800f141a
#define IR_INT422_SKIPBACK       0x800f141b

#define IR_INT422_LAST           0x800f1438
#define IR_INT422_MUTE           0x800f140e
#define IR_INT422_VOLUMEUP       0x800f1410
#define IR_INT422_VOLUMEDOWN     0x800f1411
#define IR_INT422_CHANNELUP      0x800f1412
#define IR_INT422_CHANNELDOWN    0x800f1413

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

#define IR_INT422_INPUT          0x800f142c
#define IR_INT422_ENTER          0x800f140b

#define IR_INT422_STAR           0x800f1435
#define IR_INT422_CIRCLE         0x800f1434
#define IR_INT422_TRIANGLE       0x800f1436
#define IR_INT422_DIAMOND        0x800f1433
#define IR_INT422_SQUARE         0x800f1437
#define IR_INT422_LIVE           0x800f1425
#define IR_INT422_EJECT          0x800f1428
#define IR_INT422_ZOOM           0x800f1427


//------------ Standard Logitech 880 buttons ---------------------
#define REMOTE_MUTE         IR_INT422_MUTE
#define REMOTE_VOL_UP       IR_INT422_VOLUMEUP
#define REMOTE_VOL_DOWN     IR_INT422_VOLUMEDOWN

#define REMOTE_ARROW_DOWN   0x0                     /* Unsupported */
#define REMOTE_ARROW_UP     0x0                     /* Unsupported */

#define REMOTE_CH_DOWN      IR_INT422_CHANNELDOWN
#define REMOTE_CH_UP        IR_INT422_CHANNELUP
#define REMOTE_PREV         IR_INT422_SKIPBACK
#define REMOTE_UP           IR_INT422_UP
#define REMOTE_DOWN         IR_INT422_DOWN
#define REMOTE_LEFT         IR_INT422_LEFT
#define REMOTE_RIGHT        IR_INT422_RIGHT
#define REMOTE_OK           IR_INT422_OK
// TODO test REMOTE_MENU mapping
#define REMOTE_MENU         IR_INT422_MENU
#define REMOTE_EXIT         IR_INT422_BACK
#define REMOTE_GUIDE        IR_INT422_GUIDE
#define REMOTE_INFO         IR_INT422_INFO
#define REMOTE_STOP         IR_INT422_STOP

//#define REMOTE_REPLAY       ?
#define REMOTE_SKIP         IR_INT422_SKIPFORWARD
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

#define REMOTE_POWER_TOGGLE IR_INT422_POWER

#define REMOTE_CLEAR        IR_INT422_LAST
#define REMOTE_RED          IR_INT422_RED
#define REMOTE_GREEN        IR_INT422_GREEN
#define REMOTE_YELLOW       IR_INT422_YELLOW
#define REMOTE_BLUE         IR_INT422_BLUE
#define REMOTE_SUBTITLE     IR_INT422_DISPLAY
#define REMOTE_ASPECT       IR_INT422_ZOOM

// Probably nonsensical
#define REMOTE_F1           IR_INT422_CIRCLE
/*
#define REMOTE_F2           ?
#define REMOTE_F3           ?
#define REMOTE_F4           ?
#define REMOTE_F7           ?
*/
#define REMOTE_F5           IR_INT422_TRIANGLE
#define REMOTE_F6           IR_INT422_DIAMOND
#define REMOTE_F8           IR_INT422_SQUARE
