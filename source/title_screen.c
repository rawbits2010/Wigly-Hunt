#include "title_screen.h"

#include <string.h>

#include "sprite_buffer.h"
#include "letter.h"
#include "title.h"
#include "text.h"

// top:lascre_0123456789->
static Letter top_score_text[17];
static Letter last_score_text[17];
static Letter start_text[5];
static Letter arrow;

static u32 top_score_idx[] =	{  0,  1,  2, 10,  6,  7,  1,  8,  9,  3, 10, 21,21,21,21,21,21};
static u32 last_score_idx[] =	{  4,  5,  6,  0, 10,  6,  7,  1,  8,  9,  3, 21,21,21,21,21,21};
static u32 start_idx[] = 		{  6,  0,  5,  8,  0 };

static u32 curr_arrow_theta;

static inline u32 getArrowBasePosX() {
	return 240/2 - (7*8)/2;
}

void titlescreenInit() {
	
	//
	// BG

	// Load palette
	//memcpy(pal_bg_mem, titlePal, 16*2);
	// Load map into SBB 30
	memcpy(&se_mem[29][0], titleMap, titleMapLen);

	//
	// TEXT
	memcpy(&tile_mem[4][0], textTiles, textTilesLen);
	memcpy(&pal_obj_mem[0], &textPal, 16*2);

	
	// > START
	u32 text_pos_x = getArrowBasePosX();
	u32 text_pos_y = 160/2 - 8/2;

	// arrow
	spritebufferCreateLetter(&arrow, 22);
	arrow.pos_x = text_pos_x;
	arrow.pos_y = text_pos_y;
	letterSetPosition( &arrow );
	curr_arrow_theta = 0x9000;

	text_pos_x += 16;
	for(u32 i = 0; i < 5; i++) {

		spritebufferCreateLetter(&start_text[i], start_idx[i]);

		start_text[i].pos_x = text_pos_x + i*8;
		start_text[i].pos_y = text_pos_y;

		letterSetPosition( &start_text[i] );

	}

	// top score:  ------
	text_pos_x = 88;
	text_pos_y = 120;
	for(u32 i = 0; i < 17; i++) {

		spritebufferCreateLetter(&top_score_text[i], top_score_idx[i]);

		top_score_text[i].pos_x = text_pos_x + i*8;
		top_score_text[i].pos_y = text_pos_y;

		letterSetPosition( &top_score_text[i] );

	}
	
	// last score:  ------
	text_pos_y += 16 ;
	for(u32 i = 0; i < 17; i++) {

		spritebufferCreateLetter(&last_score_text[i], last_score_idx[i]);

		last_score_text[i].pos_x = text_pos_x + i*8;
		last_score_text[i].pos_y = text_pos_y;

		letterSetPosition( &last_score_text[i] );

	}

}


void titlescreenUpdate() {

	// from Tonc - i'm too stupid for doing math
	s32 arrow_pos = (6*lu_sin(2*curr_arrow_theta)>>12) + getArrowBasePosX();
	curr_arrow_theta -= 0x380;
	arrow.pos_x = arrow_pos;
	letterSetPosition( &arrow );

}

void titlescreenReset( u32 top_score, u32 last_score ) {
	pal_bg_mem[0] = 0;

	// Load tiles into CBB 0
	memcpy(pal_bg_mem, titlePal, 16*2);
	memcpy(&tile_mem[0][0], titleTiles, titleTilesLen);

	obj_unhide(arrow.obj_attr, ATTR1_SIZE_8);
	for(u32 i = 0; i < 5; i++) {
		obj_unhide(start_text[i].obj_attr, ATTR1_SIZE_8);
	}
	for(u32 i = 0; i < 17; i++) {
		obj_unhide(top_score_text[i].obj_attr, ATTR1_SIZE_8);
		obj_unhide(last_score_text[i].obj_attr, ATTR1_SIZE_8);
	}

	// scores
	if( top_score == 0 ) {	// no top score = never played
		return;
	}

	u32 score_start = 11;
	for(u32 i = 16; i >= 11; i--) {

		u32 ts_num = top_score % 10;
		top_score /= 10;
		letterSetTile( &top_score_text[i], score_start + ts_num );

		u32 ls_num = last_score % 10;
		last_score /= 10;
		letterSetTile( &last_score_text[i], score_start + ls_num );

	}
}

inline bool titlescreenHandleInput() {
	
	key_poll();

	if( key_hit(KEY_A) || key_hit(KEY_START) ) {

		// TODO: ugliest hack in the world but brain is a fail big...
		obj_hide(arrow.obj_attr);
		for(u32 i = 0; i < 5; i++) {
			obj_hide(start_text[i].obj_attr);
		}
		for(u32 i = 0; i < 17; i++) {
			obj_hide(top_score_text[i].obj_attr);
			obj_hide(last_score_text[i].obj_attr);
		}

		return true;
	}

	return false;
}
