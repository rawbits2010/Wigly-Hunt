#include "level_map.h"

#include <string.h>

#include "platform.h"
#include "bg.h"
#include "bg_color.h"


static LevelMap levelmap;
static u32 tiles_offset = 20*32;

// always add at the row just before the view area (pos_x: 0-32)
static void s_AddSeaweed( u32 pos_x ) {

	u32 scroll_pos = (levelmap.seaweed_scroll_pos/256)/8;
	u32 base_pos = (32*2)*((15+scroll_pos)%15) + pos_x;

	se_mem[31][base_pos] = (SE_PALBANK(1) | (tiles_offset+1));
	se_mem[31][base_pos+1] = (SE_PALBANK(1) | (tiles_offset+2));
	se_mem[31][base_pos + 32] = (SE_PALBANK(1) | (tiles_offset+3));
	se_mem[31][base_pos + 32 + 1] = (SE_PALBANK(1) | (tiles_offset+4));

	levelmap.seaweed_added_at = base_pos;
}

static void s_RemoveSeaweed() {

	u32 base_pos = levelmap.seaweed_added_at;

	se_mem[31][base_pos] = tiles_offset;
	se_mem[31][base_pos+1] = tiles_offset;
	se_mem[31][base_pos + 32] = tiles_offset;
	se_mem[31][base_pos + 32 + 1] = tiles_offset;

}

static void s_AddOceanTop() {

	for(u32 i = 0; i < 32; i+=2 ) {
		se_mem[30][i] = (SE_PALBANK(2) | (tiles_offset+10));
		se_mem[30][i+1] = (SE_PALBANK(2) | (tiles_offset+11));
		se_mem[30][i + 32] = (SE_PALBANK(2) | (tiles_offset+12));
		se_mem[30][i + 32 + 1] = (SE_PALBANK(2) | (tiles_offset+13));
	}

	for(u32 i = 0; i < 32; i+=2 ) {
		se_mem[30][i+64] = (SE_PALBANK(2) | (tiles_offset+14));
		se_mem[30][i+64+1] = (SE_PALBANK(2) | (tiles_offset+15));
		se_mem[30][i+64 + 32] = (SE_PALBANK(2) | (tiles_offset+16));
		se_mem[30][i+64 + 32 + 1] = (SE_PALBANK(2) | (tiles_offset+17));
	}

}

static void s_AddOceanBottom() {

	u32 pos = 20*32 + 4*32; // bottom of screen + top scrolling out

	for(u32 i = 0; i < 32; i+=2 ) {
		se_mem[30][pos + i] = (SE_PALBANK(2) | (tiles_offset+18));
		se_mem[30][pos + i+1] = (SE_PALBANK(2) | (tiles_offset+19));
		se_mem[30][pos + i+32] = (SE_PALBANK(2) | (tiles_offset+20));
		se_mem[30][pos + i+32+1] = (SE_PALBANK(2) | (tiles_offset+21));
	}

	for(u32 i = 0; i < 32; i+=2 ) {
		se_mem[30][pos + i+64] = (SE_PALBANK(2) | (tiles_offset+22));
		se_mem[30][pos + i+64+1] = (SE_PALBANK(2) | (tiles_offset+23));
		se_mem[30][pos + i+64 + 32] = (SE_PALBANK(2) | (tiles_offset+24));
		se_mem[30][pos + i+64 + 32 + 1] = (SE_PALBANK(2) | (tiles_offset+25));
	}

}


static void s_SetBGColorTo(u32 bg_pal_idx) {
	pal_bg_mem[0] = pal_bg_mem[16*3 + bg_pal_idx];
}

inline void levelmapInit() {

	// load level map tiles and background color
	memcpy(&pal_bg_mem[16], platformPal, 16*2);
	memcpy(&tile_mem[0][tiles_offset+1], platformTiles, platformTilesLen);
	memcpy(&pal_bg_mem[16*2], bgPal, 16*2);
	memcpy(&tile_mem[0][tiles_offset+10], bgTiles, bgTilesLen);
	memcpy(&pal_bg_mem[16*3], bg_colorPal, 16*2);

	s_AddOceanTop();

	// clear ocean
	memset(&se_mem[30][4*32], 0, 40*32);

	s_AddOceanBottom();

	//levelmapReset();

}

void levelmapReset() {

	// the seaweed layer
	levelmap.curr_seaweed_spawn_time = 0;
	levelmap.seaweed_spawn_cooldown = SEAWEED_SPAWN_TIME_MAX;
	levelmap.seaweed_added_at = 0;
	levelmap.seaweed_time_left = 0;

	levelmap.seaweed_scroll_pos = 0;

	// the actual background
	levelmap.curr_deepness_level = DEEPNESS_LEVEL_MAX;

	levelmap.bg_color_change_level = DEEPNESS_COLOR_CHANGE;
	levelmap.curr_bg_color_idx = 15;
	s_SetBGColorTo(levelmap.curr_bg_color_idx);

	levelmap.bg_scroll_time = DEEPNESS_SCROLL_TIME;
	levelmap.bg_scroll_pos = 0;

}

inline void levelmapUpdate() {

	//
	// descending

	// determine deepness section
	if( levelmap.curr_deepness_level >= (DEEPNESS_LEVEL_MAX - (DEEPNESS_SCROLL_TIME*32)) ) {
		levelmap.curr_deepness_section = 0;	// top
	} else if( levelmap.curr_deepness_level > DEEPNESS_SCROLL_TIME*32 )	{
		levelmap.curr_deepness_section = 1;	// descending to the deep
	} else if( levelmap.curr_deepness_level <= DEEPNESS_SCROLL_TIME*32 && levelmap.curr_deepness_level != 0 ) {
		levelmap.curr_deepness_section = 2;	// bottom
	} else {
		levelmap.curr_deepness_section = 3;	// level end - TODO: boss fight
	}

	switch( levelmap.curr_deepness_section ) {

		// scrolling the top out
		case 0: {

			if( levelmap.bg_scroll_pos <= 32) {	// started descending

				levelmap.bg_scroll_time--;
				if(levelmap.bg_scroll_time <= 0 ) {
					levelmap.bg_scroll_time = DEEPNESS_SCROLL_TIME;

					REG_BG0VOFS = levelmap.bg_scroll_pos++;

				}
			}

		} break;

		// changing background color
		case 1: {

			levelmap.bg_color_change_level--;
			if( levelmap.bg_color_change_level <= 0 ) {
				levelmap.bg_color_change_level = DEEPNESS_COLOR_CHANGE;
			
				levelmap.curr_bg_color_idx--;
				s_SetBGColorTo(levelmap.curr_bg_color_idx);

			}

		} break;

		// scrolling the bottom in
		case 2: {
				
			if( levelmap.bg_scroll_pos <= 64) {	// started descending

				levelmap.bg_scroll_time--;
				if(levelmap.bg_scroll_time <= 0 ) {
					levelmap.bg_scroll_time = DEEPNESS_SCROLL_TIME;

					REG_BG0VOFS = levelmap.bg_scroll_pos++;

				}
			}

		} break;

	}
	
	if( levelmap.curr_deepness_level >= 0 ) {
		levelmap.curr_deepness_level--;
	} else {
		// stop descending
	}

	//
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
			//s_AddSeaweed( 0 ); buggy as hell
			levelmap.seaweed_time_left = 160+8; // scroll exactly one screen height

			// TODO: random here
			levelmap.seaweed_spawn_cooldown = SEAWEED_SPAWN_TIME_MAX;
		}

	}

	REG_BG1VOFS= levelmap.seaweed_scroll_pos++;

}

u32 levelmapGetDeepnessLevel() {
	return 15 - levelmap.curr_bg_color_idx;
}

u32 levelmapGetDeepnessSection() {
	return levelmap.curr_deepness_section;
}