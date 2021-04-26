#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H


#include <tonc.h>

#define SEAWEED_SPAWN_TIME_MAX 146	// in frames

#define DEEPNESS_LEVEL_MAX 4900 // in frames (60/sec)
#define DEEPNESS_COLOR_CHANGE (DEEPNESS_LEVEL_MAX / 16)
#define DEEPNESS_SCROLL_TIME (DEEPNESS_COLOR_CHANGE / 32) // should scroll out in 1 color change

typedef struct LevelMap {

	// seaweed stuff - should be only one at a time
	u32 seaweed_spawn_cooldown;		// random number between SEAWEED_SPAWN_TIME_MAX/3 and SEAWEED_SPAWN_TIME_MAX
	u32 curr_seaweed_spawn_time;	// frame counter

	// technical
	u32 seaweed_scroll_pos;
	u32 seaweed_added_at;
	u32 seaweed_time_left;

	// background
	u32 curr_deepness_level;	// how far we are down
	u32 curr_deepness_section;	// 0 - top, 1 - deep, 3 - bottom

	u32 bg_color_change_level;	// bg color changes when zero
	u32 curr_bg_color_idx;		// going down in the palette

	u32 bg_scroll_pos;
	u32 bg_scroll_time;			// advance scroll when zero

} LevelMap;


// call this only once!
extern void levelmapInit();
// call this before a new run
extern void levelmapReset();
// call this every frame
extern void levelmapUpdate();

// determine state
extern u32 levelmapGetDeepnessLevel();		// this is the background level - there are 16 levels of deepness
extern u32 levelmapGetDeepnessSection();	// the section as is



#endif // LEVEL_MAP_H