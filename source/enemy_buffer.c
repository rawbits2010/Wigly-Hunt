#include "enemy_buffer.h"

#include <stdlib.h>	// for rand()

#include "fish_1_A.h"
#include "fish_1_B.h"
#include "fish_2_A.h"
#include "fish_2_B.h"
#include "fish_3_A.h"
#include "fish_3_B.h"


static EnemyBuffer enemy_buff;

// fish1
Animation fish1A_swim; // 16x16@4
Animation fish1A_anims[1];
Sprite fish1A;
Animation fish1B_swim; // 16x16@4
Animation fish1B_anims[1];
Sprite fish1B;
DoubleSprite fish1;

// fish2
Animation fish2A_anims[1];
Animation fish2A_swim; // 16x16@4
Animation fish2B_anims[1];
Animation fish2B_swim; // 16x16@4
Sprite fish2A;
Sprite fish2B;
DoubleSprite fish2;

// fish3
Animation fish3A_anims[1];
Animation fish3A_swim; // 16x16@4
Animation fish3B_anims[1];
Animation fish3B_swim; // 16x16@4
Sprite fish3A;
Sprite fish3B;
DoubleSprite fish3;


void enemybufferInit() {

	//
	// fish1
	createAnimation( &fish1A_swim, fish_1_ATiles, 3, 0 );
	fish1A_swim.coll_x_offset = 7;
	fish1A_swim.coll_y_offset = 10;
	animationInit(&fish1A_swim, 15, true, true);

	fish1A_anims[0] = fish1A_swim;

	createAnimation( &fish1B_swim, fish_1_BTiles, 3, 0 );
	// no collision
	animationInit(&fish1B_swim, 15, true, true);

	fish1B_anims[0] = fish1B_swim;

	createSprite( &fish1A, &fish1A_anims[0], fish_1_APal );
	spriteLoadPalette( &fish1A );
	spritebufferAddSprite( &fish1A );
	
	createSprite( &fish1B, &fish1B_anims[0], fish_1_BPal );
	spriteLoadPalette( &fish1B );
	spritebufferAddSprite( &fish1B );

	fish1.left = &fish1A;
	fish1.right = &fish1B;
	doublespriteSetHidden( &fish1, true );

	//
	// fish2
	createAnimation( &fish2A_swim, fish_2_ATiles, 3, 0 );
	fish2A_swim.coll_x_offset = 7;
	fish2A_swim.coll_y_offset = 10;
	animationInit(&fish2A_swim, 15, true, true);

	fish2A_anims[0] = fish2A_swim;

	createAnimation( &fish2B_swim, fish_2_BTiles, 3, 0 );
	animationInit(&fish2B_swim, 15, true, true);

	fish2B_anims[0] = fish2B_swim;

	createSprite( &fish2A, &fish2A_anims[0], fish_2_APal );
	spriteLoadPalette( &fish2A );
	spritebufferAddSprite( &fish2A );
	
	createSprite( &fish2B, &fish2B_anims[0], fish_2_BPal );
	spriteLoadPalette( &fish2B );
	spritebufferAddSprite( &fish2B );

	fish2.left = &fish2A;
	fish2.right = &fish2B;
	doublespriteSetHidden( &fish2, true );

	//
	// fish3
	createAnimation( &fish3A_swim, fish_3_ATiles, 3, 0 );
	fish3A_swim.coll_x_offset = 7;
	fish3A_swim.coll_y_offset = 10;
	animationInit(&fish3A_swim, 15, true, true);

	fish3A_anims[0] = fish3A_swim;

	createAnimation( &fish3B_swim, fish_3_BTiles, 3, 0 );
	animationInit(&fish3B_swim, 15, true, true);

	fish3B_anims[0] = fish3B_swim;

	createSprite( &fish3A, &fish3A_anims[0], fish_3_APal );
	spriteLoadPalette( &fish3A );
	spritebufferAddSprite( &fish3A );
	
	createSprite( &fish3B, &fish3B_anims[0], fish_3_BPal );
	spriteLoadPalette( &fish3B );
	spritebufferAddSprite( &fish3B );

	fish3.left = &fish3A;
	fish3.right = &fish3B;
	doublespriteSetHidden( &fish3, true );

	//
	// create empty, hidden sprites
	for(u32 i = 0; i < 5; i++ ) {

		Sprite *TempA = &(enemy_buff.enemy_sprites_A[i]);
		spritebufferCreateEmpty( TempA );
		Sprite *TempB = &(enemy_buff.enemy_sprites_B[i]);
		spritebufferCreateEmpty( TempB );

		DoubleSprite *Temp = &(enemy_buff.enemies[i]);
		Temp->left = TempA;
		Temp->right = TempB;
		doublespriteSetHidden( Temp, true );

	}

}

void enemybufferReset() {

	// only need to hide enemies as everything else will be overwritten on spawn
	for(u32 i = 0; i < 5; i++ ) {
		doublespriteSetHidden( &(enemy_buff.enemies[i]), true );
	}

}

// screen 240x160
void enemybufferSpawnEnemy( u32 kind, u32 pos_x, u32 movement ) {
	for(u32 i = 0; i < 5; i++ ) {

		// is slot free -> spawn
		if( enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);

			// hardcoded for now
			DoubleSprite *enemy;
			if( kind == 1 ) {
				enemy = &fish1;
			} else if( kind == 2 ) {
				enemy = &fish2;
			} else if( kind == 3 ) {
				enemy = &fish3;
			} else {
				enemy = NULL;	// how to assert here?
			}

			doublespriteCopy( enemy, curr_enemy );
			curr_enemy->pos_x = pos_x;
			curr_enemy->pos_y = 160 + (rand() % 32);
			doublespriteSetHidden( curr_enemy, false );
			doublespriteSetPosition( curr_enemy );	// so it's in the obj_buffer

			enemy_buff.enemy_kind[i] = kind;

			enemy_buff.movement_kind[i] = movement;
			enemy_buff.movement_change_cooldown[i] = 30; // get half a sec benefit
			enemy_buff.movement_dir[i] = -1;
			enemy_buff.movement_hover_cooldown[i] = 10;
			enemy_buff.movement_hover[i] = false;
			enemy_buff.movement_attracted[i] = false;

			break;
		}

	}
}

void enemybufferUpdateEnemies( Sprite *worm ) {

	// do animation
	doublespriteAdvanceAnimation( &fish1 );
	doublespriteAdvanceAnimation( &fish2 );
	doublespriteAdvanceAnimation( &fish3 );

	for(u32 i = 0; i < 5; i++ ) {
		if( !enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);
			
			u32 attract_zone_size = 40;
			// TODO: use the eye pixel or at least the center of sprites
			// convert these into signed
			s32 worm_x1 = spriteGetCollisionPosX( worm );
			worm_x1 -= attract_zone_size;
			s32 worm_x2 = spriteGetCollisionPosX( worm ) + attract_zone_size;
			s32 worm_y1 = spriteGetCollisionPosY( worm );
			worm_y1 -= attract_zone_size;
			s32 worm_y2 = spriteGetCollisionPosY( worm ) + attract_zone_size;
			// fish can go out of screen
			s32 enemy_x = (doublespriteGetCollisionPosX( curr_enemy ) + 32) % 512;
			enemy_x -= 32;
			s32 enemy_y = (doublespriteGetCollisionPosY( curr_enemy ) + 32) % 512;
			enemy_y -= 32;
			
			if(	worm_x1 < enemy_x &&
				worm_x2 > enemy_x &&
				worm_y1 < enemy_y &&
				worm_y2 > enemy_y ) {

				enemy_buff.movement_attracted[i] = true;

			}

			s32 force_y = 0;
			s32 force_x = 0;

			// attracted fish move towards player
			if( enemy_buff.movement_attracted[i] ) {

				if( worm_x1+attract_zone_size < enemy_x ) {
					force_x -= 1;
				} else if( worm_x1+attract_zone_size > enemy_x ){
					force_x += 1;
				}

				if( worm_y1+attract_zone_size < enemy_y ) {
					force_y -= 1;
				} else if( worm_y1+attract_zone_size > enemy_y ) {
					force_y += 1;
				}

				enemy_buff.movement_attracted[i] = false; // just lost the worm for now

			} else {

				// ascend as we descend
				force_y -= 1;

				// do natural moving
				switch( enemy_buff.movement_kind[i] ) {

					case 0: {	// cruising left and right randomly

						// time to change
						enemy_buff.movement_change_cooldown[i]--;
						if( enemy_buff.movement_change_cooldown[i] <= 0 ) {
							if( rand() % 100 > 70 ) {

								enemy_buff.movement_dir[i] *= -1;
								enemy_buff.movement_speed[i] = (rand() % 9)/2;

							}

							enemy_buff.movement_change_cooldown[i] = rand() % 30 + 20;
						}

						force_x += enemy_buff.movement_speed[i] * enemy_buff.movement_dir[i];

						enemy_buff.movement_hover_cooldown[i]--;
						if( enemy_buff.movement_hover_cooldown[i] <= 0 ) {
							enemy_buff.movement_hover_cooldown[i] = rand() % 20 + 10;

							enemy_buff.movement_hover[i] =  (rand() % 6) > 5;

						}

						if( enemy_buff.movement_hover[i] ) {
							force_y = 0;
						}

					} break;

					case 1: {

					} break;

					case 2: {

					} break;
				}

			}
			
			curr_enemy->pos_y += force_y;
			curr_enemy->pos_x += force_x;

			// clamp 'cause it wraps but the hittest won't work while it's only visual
			curr_enemy->pos_x %= 512;
			curr_enemy->pos_y %= 256;
	
			if( force_x > 0 ) {
				doublespriteSetHFlipped( curr_enemy, true );
			} else if( force_x < 0) {
				doublespriteSetHFlipped( curr_enemy, false );
			} 
			
			doublespriteSetPosition( curr_enemy ); // flip will set the position for a double (at least now), isn't it? it doesn't works!

			// remove fish that "scroller" out - up out of the screen or too far down
			if( curr_enemy->pos_y > 200 ) { // u32
				doublespriteSetHidden( curr_enemy, true );
			}

			// TODO: this is a big hack! don't need to update it every frame
			doublespriteUpdateGfx( curr_enemy );

		}
	}

}


bool enemybufferDoHitTest( Sprite *worm ) {

	// hit area is hardcoded here for now
	u32 fish_hit_area = 4;
	u32 worm_hit_area = 4;

	for(u32 i = 0; i < 5; i++ ) {
		if( !enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);
			
			// works with u32 'cause we only test stuff on screen
			u32 rect1_x = spriteGetCollisionPosX( worm ) - worm_hit_area;
			u32 rect1_y = spriteGetCollisionPosY( worm ) - worm_hit_area;
			u32 rect1_w = worm_hit_area*2;
			u32 rect1_h = worm_hit_area*2;
			
			u32 rect2_x = doublespriteGetCollisionPosX( curr_enemy ) - fish_hit_area;
			u32 rect2_y = doublespriteGetCollisionPosY( curr_enemy ) - fish_hit_area;
			u32 rect2_w = fish_hit_area*2;
			u32 rect2_h = fish_hit_area*2;

			// AABB - this isn't need any fixed point
			if(	rect1_x < rect2_x + rect2_w &&
				rect1_x + rect1_w > rect2_x &&
				rect1_y < rect2_y + rect2_h &&
				rect1_y + rect1_h > rect2_y ) {

				// You are dead Jim!
				return true;

			}

		}
	}

	return false;
}


// hack 'cause I still didn't get a worm object together
static inline u32 s_GetWormHitPosX( Sprite *worm ) {
	return worm->pos_x + 3;
}
static inline u32 s_GetWormHitPosY( Sprite *worm ) {
	return worm->pos_y + 9;
}

u32 enemybufferDoSlapTest( Sprite *worm ) {
	
	// hardcoded until objects happen
	u32 worm_hit_area = 5;
	u32 fish_h_hit_area = 11;
	u32 fish_x_hit_area = 5;
			
	// works with u32 'cause we only test stuff on screen
	u32 rect1_x = s_GetWormHitPosX( worm ) - worm_hit_area;
	u32 rect1_w = 2*worm_hit_area;
	u32 rect1_y = s_GetWormHitPosY( worm ) - worm_hit_area;
	u32 rect1_h = 2*worm_hit_area;

	for(u32 i = 0; i < 5; i++ ) {
		if( !enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);
		
			u32 rect2_x = curr_enemy->pos_x+16 - fish_h_hit_area;
			u32 rect2_y = curr_enemy->pos_y+8 - fish_x_hit_area;
			u32 rect2_w = fish_h_hit_area*2;
			u32 rect2_h = fish_x_hit_area*2;

			// AABB - this isn't need any fixed point
			if(	rect1_x < rect2_x + rect2_w &&
				rect1_x + rect1_w > rect2_x &&
				rect1_y < rect2_y + rect2_h &&
				rect1_y + rect1_h > rect2_y ) {

				// SMACK!!
				doublespriteSetHidden( curr_enemy, true );
				return enemy_buff.enemy_kind[i];

			}
		
		
		}
	}

	return 255;
}