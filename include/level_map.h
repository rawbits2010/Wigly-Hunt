#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H


#include <tonc.h>

#define SEAWEED_SPAWN_TIME_MAX 146	// in frames

#define DEEPNESS_LEVEL_MAX 1000 // in frames
#define DEEPNESS_COLOR_CHANGE (DEEPNESS_LEVEL_MAX / 16)

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
	u32 bg_color_change_level;	// bg color changes when zero

} LevelMap;


extern void levelmapInit();
extern void levelmapUpdate();



#endif // LEVEL_MAP_H