#ifndef END_SCREEN_H
#define END_SCREEN_H


#include <tonc.h>


// call this once - load resources
extern void endscreenInit();

// call this every frame
//extern void endscreenUpdate();

// call this before displaying
extern void endscreenReset( bool win );

extern bool endscreenHandleInput();



#endif // END_SCREEN_H