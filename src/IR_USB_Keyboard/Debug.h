#ifndef Debug_H
#define Debug_H

#ifdef DEBUG_SKETCH
  #ifndef DEBUG_OUT 
    // TODO enhance with 2nd serial port and LCD output
    #define DEBUG_OUT Serial
  #endif
  #define DEBUG_PRINT(...) DEBUG_OUT.print(__VA_ARGS__);
  #define DEBUG_PRINTLN(...) DEBUG_OUT.println(__VA_ARGS__);
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif

#endif
