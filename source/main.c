#include <tonc.h>
#include <stdlib.h> // for rand()

#include "double_sprite.h"
#include "sprite.h"
#include "sprite_buffer.h"
#include "enemy_buffer.h"
#include "level_map.h"

#include "title_screen.h"
#include "end_screen.h"

// gfx data
#include "worm.h"
#include "worm_hit.h"


#define WORM_WIGGLE 0
#define WORM_HIT 1

#define DESCEND_SCORE 1
#define FISH_1_SCORE 100
#define FISH_2_SCORE 250
#define FISH_3_SCORE 500

void handleInput( Sprite *obj ) {

	key_poll();

	s32 force_x = 0;
	s32 force_y = 0;

	force_x += 1*key_tri_horz();
	force_y += 1*key_tri_vert();

	obj->pos_x += force_x;
	obj->pos_y += force_y;

	// clamp worm into view
	if( obj->pos_x > 245 ) { // u32
		obj->pos_x = 0;
	}
	if( obj->pos_x > 224 ) { // 240-16
		obj->pos_x = 224;
	}
	if( obj->pos_y > 245 ) { // u32
		obj->pos_y = 0;
	}
	if( obj->pos_y > 144 ) { // 160-16
		obj->pos_y = 144;
	}

	if(force_x != 0) {
		if(force_x > 0) {
			spriteSetHFlipped( obj, true );
		} else {
			spriteSetHFlipped( obj, false );
		}
	}

	if( obj->curr_anim_idx != WORM_HIT ) {

		if( key_hit(KEY_B) ) {
			spriteSetAnimationFrame( obj, WORM_HIT );
			spriteSetHFlipped( obj, false );
		}

		if( key_hit(KEY_A) ) {
			spriteSetAnimationFrame( obj, WORM_HIT );
			spriteSetHFlipped( obj, true );
		}

	}

	spriteSetPosition( obj );

}


//
// helpers

bool canSpawn( u32 drop_recent ) {
	bool spawn_chance = (rand() % 10000) > 9700; // give a chance to spawn
	if( spawn_chance ) {
		spawn_chance = (rand() % 100) > drop_recent;	// reject it 3rd of the time
	}
	return spawn_chance;
}

// call this before new game
// TODO: worm still doesn't have his own obj..
void resetRun( Sprite *worm, u32 frames, u32 *run_score ) {

	// reset random number generator
	srand((unsigned) frames);

	// reset score
	*run_score = 0;

	// start above center
	worm->pos_x = (240-16)/2;
	worm->pos_y = ((160-16)/4);
	spriteSetHidden(worm, false);

	// eliminate enemies
	enemybufferReset();

	// reset the level
	levelmapReset();

}

u32 frame_count;
u32 curr_screen;
#define TITLE_SCREEN 0
#define GAME_SCREEN 1
#define DEATH_SCREEN 2
#define END_SCREEN 3

u32 killed_by;
u32 killed_at_depth;

void doTitleScreen( u32 last_run_score, u32 top_score ) {

	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT= BG_CBB(0) | BG_SBB(29) | BG_4BPP | BG_REG_32x32;

	titlescreenReset( top_score, last_run_score );

	do {

		VBlankIntrWait();
		frame_count++;
		
		// update stuff
		titlescreenUpdate();

		// finalize
		spritebufferUpload(128);

	// check for start / a
	} while( !titlescreenHandleInput() );

	curr_screen = GAME_SCREEN;
}


	//
	// create the worm sprite with animations
	// TODO: move into a worm object
	Animation worm_wiggle; // 16x16@4
	Animation worm_hit; // 16x16@4
	Animation worm_anims[2];
	Sprite worm;
	void wormInit() {

	createAnimation( &worm_wiggle, wormTiles, 4, 0 );
	worm_wiggle.coll_x_offset = 5;
	worm_wiggle.coll_y_offset = 6;
	animationInit(&worm_wiggle, 15, true, false);

	createAnimation( &worm_hit, worm_hitTiles, 4, -5 );
	worm_hit.coll_x_offset = 10;
	worm_hit.coll_y_offset = 6;
	animationInit(&worm_hit, 4, false, false);

	worm_anims[WORM_WIGGLE] = worm_wiggle;
	worm_anims[WORM_HIT] = worm_hit;

	createSprite( &worm, &worm_anims[0], wormPal );
	spriteLoadPalette( &worm );
	spritebufferAddSprite( &worm );

	spriteSetHidden(&worm, true);
	}



void doGameScreen( u32 *run_score ) {

	// init graphics mode
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT= BG_CBB(0) | BG_SBB(30) | BG_4BPP | BG_REG_32x32;
	REG_BG1CNT= BG_CBB(0) | BG_SBB(31) | BG_4BPP | BG_REG_32x32;

	// reset all positions and stuff for a new game
	resetRun( &worm, frame_count, run_score );

	bool win = false;
	while(1) {

		VBlankIntrWait();
		frame_count++;

		// give point for descending
		*run_score += DESCEND_SCORE;

		// do animation
		spriteAdvanceAnimation( &worm );

		// update game state
		enemybufferUpdateEnemies( &worm );
		levelmapUpdate();


		u32 deepness = levelmapGetDeepnessLevel();
		u32 section = levelmapGetDeepnessSection();

		// spawn only while descending
		if( section == 1 ) {
			switch( deepness ) {

				// want to see enemies for the start
				case 0: {

						if( canSpawn( 60 ) ) {
							enemybufferSpawnEnemy(1, rand() % 240, 0);
						}

				} break;

				// steady spawn but let the get his power up
				case 1:
				case 2: {

						if( canSpawn( 30 ) ) {
							enemybufferSpawnEnemy(1, rand() % 240, 0);
						}

				} break;

				// just introducing the harder fish
				case 3:
				case 4: {

						if( canSpawn( 50 )) {
							enemybufferSpawnEnemy(1, rand() % 240, 0);
						}
						if( canSpawn( 30 ) ) {
							enemybufferSpawnEnemy(2, rand() % 240, 0);
						}

				} break;

				case 5:
				case 6:
				case 7:
				case 8: {

						if( canSpawn( 50 ) ) {
							enemybufferSpawnEnemy(2, rand() % 240, 0);
						}

				} break;

				case 9:
				case 10:
				case 11: {

						if( canSpawn( 50 ) ) {
							enemybufferSpawnEnemy(2, rand() % 240, 0);
						}
						if( canSpawn( 30 ) ) {
							enemybufferSpawnEnemy(3, rand() % 240, 0);
						}

				} break;

				case 12:
				case 13:
				case 14:
				case 15: {

						if( canSpawn( 60 ) ) {
							enemybufferSpawnEnemy(3, rand() % 240, 0);
						}

				} break;
			}
		}

		// check hitting to have a chance to survive
		if( worm.curr_anim_idx == WORM_HIT ) {
			u32 kind;
			do {
				kind = enemybufferDoSlapTest( &worm );

				switch( kind ) {

					case 1: {
						*run_score += FISH_1_SCORE;
					} break;

					case 2: {
						*run_score += FISH_2_SCORE;
					} break;

					case 3: {
						*run_score += FISH_3_SCORE;
					} break;

				}

			} while( kind != 255 );
		}

		//
		// end condition checks!!!

		// hittest
		u32 kind = enemybufferDoHitTest( &worm );
		if( kind != 255 ) {
			enemybufferHideFish();
			spriteSetHidden(&worm, true);
			spritebufferUpload(128);

killed_by = kind;
killed_at_depth = deepness;

			break; // You are dead Jim!
		}

		// are you winning, son?
		if( section == 3 && enemybufferGetFishCount() == 0 ) {
			win = true;
			enemybufferHideFish();
			spriteSetHidden(&worm, true);
			spritebufferUpload(128);
			break;
		}

		// do that
		handleInput( &worm );

		// finalize
		spritebufferUpload(128);

	}

	if( win ) {
		curr_screen = END_SCREEN;
	} else {
		curr_screen = DEATH_SCREEN;
	}
	
}

void doEndScreen( bool win ) {

	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_OBJ | DCNT_OBJ_1D;
	REG_BG0CNT= BG_CBB(0) | BG_SBB(28) | BG_4BPP | BG_REG_32x32;

	endscreenReset( win );

// the who killed you when 
if(!win) {
			// set fish color on death screen
			u32 fishpalstart = (16*2) + ((killed_by-1)*2);
			u32 deadpalstart = 1;
			pal_bg_mem[deadpalstart] = pal_obj_mem[fishpalstart+(2)];
			pal_bg_mem[deadpalstart+1] = pal_obj_mem[fishpalstart+(3)];
			pal_bg_mem[deadpalstart+2] = pal_obj_mem[fishpalstart+(4)];

			// set bg color to depth color
			pal_bg_mem[0] = pal_bg_mem[16*3 + (15-killed_at_depth)];
}
			u32 delay = 0;
			bool start = false;

	do {

		VBlankIntrWait();
		frame_count++;
		
		if( delay <= 3*60) {
			delay++;
		} else {
			start = endscreenHandleInput();
		}
		// update stuff
		//endscreenUpdate();

	// check for start / a
	} while( !start );

	curr_screen = TITLE_SCREEN;
}

int main() {

	// accumulate score in this
	u32 run_score = 0;
	u32 top_score = 0;

	frame_count = 0;

	spritebufferInit();

	titlescreenInit();
	endscreenInit();

 	wormInit();
	
	// create an enemy buffer and load enemy sprites
	enemybufferInit();

	// load and setup the background
	levelmapInit();


	// enable isr switchboard and VBlank interrupt
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// dirty basic screen switcher
	curr_screen = TITLE_SCREEN;
	while(1) {

		switch(curr_screen) {

			case TITLE_SCREEN: {
				doTitleScreen(run_score, top_score);
			} break;

			case GAME_SCREEN: {
				doGameScreen( &run_score );
				
				// update score
				if( run_score > top_score) {
					top_score = run_score;
				}
			} break;

			case DEATH_SCREEN: {
				doEndScreen( false );
			} break;

			case END_SCREEN: {
				doEndScreen( true );
			} break;

		}
	}


	return 0;
}