#ifndef Debug_H
#define Debug_H

#ifdef DEBUG_SKETCH
 #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__);
 #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__);
#else
 #define DEBUG_PRINT(...)
 #define DEBUG_PRINTLN(...)
#endif

#endif
