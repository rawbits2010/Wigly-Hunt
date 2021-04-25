#include <tonc.h>
#include <string.h>

#include "double_sprite.h"
#include "sprite.h"
#include "sprite_buffer.h"

// gfx data
#include "worm.h"
#include "worm_hit.h"
#include "fish_1_A.h"
#include "fish_1_B.h"
#include "platform.h"


OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;


void handleInput( Sprite *obj ) {

	key_poll();

	s32 force_x = 0;
	s32 force_y = 0;

	force_x += 1*key_tri_horz();
	force_y += 1*key_tri_vert();

	obj->pos_x += force_x;
	obj->pos_y += force_y;

	if(force_x != 0) {
		if(force_x > 0) {
			spriteSetHFlipped( obj, true );
		} else {
			spriteSetHFlipped( obj, false );
		}
	}

	if( key_hit(KEY_B) || key_hit(KEY_A) ) {

		if( key_hit(KEY_B) ) {
			spriteSetAnimationFrame( obj, 1 );
			spriteSetHFlipped( obj, false );
		}

		if( key_hit(KEY_A) ) {
			spriteSetAnimationFrame( obj, 1 );
			spriteSetHFlipped( obj, true );
		}

	}

	spriteSetPosition( obj );

}

int main() {

	//
	// create the worm sprite with animations
	Animation worm_anims[2];

	Animation worm_wiggle; // 16x16@4
	worm_wiggle.tiles = wormTiles;
	worm_wiggle.tile_size = 32;
	worm_wiggle.tiles_per_frame = 4;
	worm_wiggle.frame_count = 4;
	worm_wiggle.h_offset = 0;
	animationInit(&worm_wiggle, 15, true, false);

	worm_anims[0] = worm_wiggle;

	Animation worm_hit; // 16x16@4
	worm_hit.tiles = worm_hitTiles;
	worm_hit.tile_size = 32;
	worm_hit.tiles_per_frame = 4;
	worm_hit.frame_count = 4;
	worm_hit.h_offset = -5;
	animationInit(&worm_hit, 4, false, false);

	worm_anims[1] = worm_hit;

	Sprite worm;
	worm.anims = &worm_anims[0];
	worm.default_anim_idx = 0;
	worm.palette = wormPal;
	worm.palette_startidx = 0;
	worm.palette_count = 16;	// use only the first 16 colors
	spriteLoadPalette( &worm );

	//
	// create the double sprite fish
	Animation fish1A_anims[1];

	Animation fish1A_swim; // 16x16@4
	fish1A_swim.tiles = fish_1_ATiles;
	fish1A_swim.tile_size = 32;
	fish1A_swim.tiles_per_frame = 4;
	fish1A_swim.frame_count = 3;
	fish1A_swim.h_offset = 0;
	animationInit(&fish1A_swim, 15, true, true);

	fish1A_anims[0] = fish1A_swim;

	Sprite fish1A;
	fish1A.anims = &fish1A_anims[0];
	fish1A.default_anim_idx = 0;
	fish1A.palette = fish_1_APal;
	fish1A.palette_startidx = 0;
	fish1A.palette_count = 16;	// use only the first 16 colors
	spriteLoadPalette( &fish1A );

	Animation fish1B_anims[1];

	Animation fish1B_swim; // 16x16@4
	fish1B_swim.tiles = fish_1_BTiles;
	fish1B_swim.tile_size = 32;
	fish1B_swim.tiles_per_frame = 4;
	fish1B_swim.frame_count = 3;
	fish1B_swim.h_offset = 0;
	animationInit(&fish1B_swim, 15, true, true);

	fish1B_anims[0] = fish1B_swim;
	
	Sprite fish1B;
	fish1B.anims = &fish1B_anims[0];
	fish1B.default_anim_idx = 0;
	fish1B.palette = fish_1_BPal;
	fish1B.palette_startidx = 0;
	fish1B.palette_count = 16;	// use only the first 16 colors
	spriteLoadPalette( &fish1B );

	DoubleSprite fish1;
	fish1.left = &fish1A;
	fish1.right = &fish1B;


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

	// position the worm and fish
	worm.pos_x = 96;
	worm.pos_y = 32;
	spriteSetPosition( &worm );
	fish1.pos_x = 96;
	fish1.pos_y = 96;
	doublespriteSetPosition( &fish1 );

	u32 v = 0;
	while(1) {

		VBlankIntrWait();

		spriteAdvanceAnimation( &worm );
		doublespriteAdvanceAnimation( &fish1 );

		REG_BG0VOFS= v++;


		fish1.pos_x -= 1;
		doublespriteSetPosition( &fish1 );


		handleInput( &worm );

		spritebufferUpload(3);

	}

	return 0;
}