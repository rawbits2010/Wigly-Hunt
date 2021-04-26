#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include <tonc.h>


// call this once - load resources
extern void titlescreenInit();

// call this every frame
extern void titlescreenUpdate();

// call this before displaying
extern void titlescreenReset( u32 top_score, u32 last_score );

extern bool titlescreenHandleInput();

#endif // TITLE_SCREEN_H