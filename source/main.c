#include <tonc.h>
#include <string.h>
#include "sprite.h"
#include "sprite_buffer.h"

// gfx data
#include "worm.h"
#include "fish_1_A.h"
#include "fish_1_B.h"
#include "platform.h"


OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;


void handleInput( Sprite *obj ) {

	key_poll();

	u32 force_x = 0;
	u32 force_y = 0;

	force_x += 1*key_tri_horz();
	force_y += 1*key_tri_vert();

	obj->pos_x += force_x;
	obj->pos_y += force_y;

	if( key_hit(KEY_A) ) {
		obj->curr_frame = 1;
	} else {
		obj->curr_frame = 0;
	}

	spriteSetAnimationFrame( obj );
	spriteSetPosition( obj );

}

int main() {

	// create sprites
	Sprite worm; // 16x16@4
	worm.tiles = wormTiles;
	worm.tile_size = 32;
	worm.tiles_per_frame = 4;
	worm.frame_count = 2;
	worm.palette = wormPal;
	worm.palette_startidx = 0;
	worm.palette_count = 16;	// use only the first 16 colors
	worm.curr_frame = 1;

	// create sprites
	Sprite fish1A; // 16x16@4
	fish1A.tiles = fish_1_ATiles;
	fish1A.tile_size = 32;
	fish1A.tiles_per_frame = 4;
	fish1A.frame_count = 1;
	fish1A.palette = fish_1_APal;
	fish1A.palette_startidx = 0;
	fish1A.palette_count = 16;	// use only the first 16 colors
	fish1A.curr_frame = 0;
	Sprite fish1B; // 16x16@4
	fish1B.tiles = fish_1_BTiles;
	fish1B.tile_size = 32;
	fish1B.tiles_per_frame = 4;
	fish1B.frame_count = 1;
	fish1B.palette = fish_1_BPal;
	fish1B.palette_startidx = 0;
	fish1B.palette_count = 16;	// use only the first 16 colors
	fish1B.curr_frame = 0;

	// load gfx
	spriteLoadTiles( &worm );
	spriteLoadPalette( &worm );
	spriteLoadTiles( &fish1A );
	spriteLoadPalette( &fish1A );
	spriteLoadTiles( &fish1B );
	spriteLoadPalette( &fish1B );

// Load palette
    memcpy(&pal_bg_mem[16], platformPal, 16*2);
    // Load tiles into CBB 0
    memcpy(&tile_mem[0][1], platformTiles, platformTilesLen);
	memset16(&se_mem[30][0], (SE_PALBANK(1) | 1), 64*32);
	//pal_bg_mem[0] = 0x7FAE;

	// init graphics mode
	spritebufferInit();
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_64x32;

	// add sprites to the buffer
	spritebufferAddSprite( &worm );
	spritebufferAddSprite( &fish1A );
	spritebufferAddSprite( &fish1B );

	// enable isr switchboard and VBlank interrupt
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// position the worm
	worm.pos_x = 96;
	worm.pos_y = 32;
	spriteSetPosition( &worm );
	fish1A.pos_x = 96;
	fish1A.pos_y = 64;
	spriteSetPosition( &fish1A );
	fish1B.pos_x = 96+16;
	fish1B.pos_y = 64;
	spriteSetPosition( &fish1B );

	u32 v = 0;
	while(1) {

		VBlankIntrWait();

		REG_BG0VOFS= v++;

		handleInput( &worm );

		fish1A.pos_x -= 1;
		spriteSetPosition( &fish1A );
		fish1B.pos_x -= 1;
		spriteSetPosition( &fish1B );

		spritebufferUpload(3);

	}

	return 0;
}