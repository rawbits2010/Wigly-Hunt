#include "level_map.h"

LevelMap levelmap;


// always add at the row just before the view area (pos_x: 0-32)
static void s_AddSeaweed( u32 pos_x ) {

	u32 scroll_pos = (levelmap.seaweed_scroll_pos/256)/8;
	u32 base_pos = (32*2)*((15+scroll_pos)%15) + pos_x;

	se_mem[31][base_pos] = (SE_PALBANK(1) | 1);
	se_mem[31][base_pos+1] = (SE_PALBANK(1) | 2);
	se_mem[31][base_pos + 32] = (SE_PALBANK(1) | 3);
	se_mem[31][base_pos + 32 + 1] = (SE_PALBANK(1) | 4);

	levelmap.seaweed_added_at = base_pos;
}

static void s_RemoveSeaweed() {

	u32 base_pos = levelmap.seaweed_added_at;

	se_mem[31][base_pos] = 0;
	se_mem[31][base_pos+1] = 0;
	se_mem[31][base_pos + 32] = 0;
	se_mem[31][base_pos + 32 + 1] = 0;

}

static void s_AddOceanTop() {

	for(u32 i = 0; i < 32; i+=2 ) {
		se_mem[30][i] = (SE_PALBANK(2) | 10);
		se_mem[30][i+1] = (SE_PALBANK(2) | 11);
		se_mem[30][i + 32] = (SE_PALBANK(2) | 12);
		se_mem[30][i + 32 + 1] = (SE_PALBANK(2) | 13);
	}

	for(u32 i = 0; i < 32; i+=2 ) {
		se_mem[30][i+64] = (SE_PALBANK(2) | 14);
		se_mem[30][i+64+1] = (SE_PALBANK(2) | 15);
		se_mem[30][i+64 + 32] = (SE_PALBANK(2) | 16);
		se_mem[30][i+64 + 32 + 1] = (SE_PALBANK(2) | 17);
	}

}

static void s_RemoveOceanTop() {

}

static void s_AddOceanBottom() {

}

static void s_RemoveOceanBottom() {

}


inline void levelmapInit() {

	// the seaweed layer
	levelmap.curr_seaweed_spawn_time = 0;
	levelmap.seaweed_spawn_cooldown = SEAWEED_SPAWN_TIME_MAX;
	levelmap.seaweed_added_at = 0;
	levelmap.seaweed_time_left = 0;

	levelmap.seaweed_scroll_pos = 0;

	// the actual background
	levelmap.curr_deepness_level = DEEPNESS_LEVEL_MAX;
	levelmap.bg_color_change_level = DEEPNESS_COLOR_CHANGE;

	s_AddOceanTop();

}

inline void levelmapUpdate() {

	// descending
	levelmap.curr_deepness_level--;
	levelmap.bg_color_change_level--;
	//REG_BG0VOFS= levelmap.bg_scroll_pos--;

	// apearing seaweed
	if( levelmap.seaweed_time_left > 0 ) {

		levelmap.seaweed_time_left--;

		if( levelmap.seaweed_time_left <= 0 ) {
			
			s_RemoveSeaweed();

		}

	} else {

		levelmap.curr_seaweed_spawn_time++;
		if( levelmap.curr_seaweed_spawn_time >= levelmap.seaweed_spawn_cooldown ) {
			levelmap.curr_seaweed_spawn_time = 0;

			// TODO: random here
			s_AddSeaweed( 0 );
			levelmap.seaweed_time_left = 160+8; // scroll exactly one screen height

			// TODO: random here
			levelmap.seaweed_spawn_cooldown = SEAWEED_SPAWN_TIME_MAX;
		}

	}

	REG_BG1VOFS= levelmap.seaweed_scroll_pos++;

}