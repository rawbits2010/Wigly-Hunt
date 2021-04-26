#include "enemy_buffer.h"

#include <stdlib.h>


static EnemyBuffer enemy_buff;


void enemybufferInit() {

	// create empty, hidden sprites
	for(u32 i = 0; i < 5; i++ ) {

		Sprite *TempA = &(enemy_buff.enemy_sprites_A[i]);
		spritebufferCreateEmpty( TempA );
		Sprite *TempB = &(enemy_buff.enemy_sprites_B[i]);
		spritebufferCreateEmpty( TempB );

		DoubleSprite *Temp = &(enemy_buff.enemies[i]);
		Temp->left = TempA;
		Temp->right = TempB;
		Temp->hidden = true;

	}

}

// screen 240x160
void enemybufferSpawnEnemy(DoubleSprite *enemy, u32 pos_x, u32 movement ) {
	for(u32 i = 0; i < 5; i++ ) {

		// is slot free -> spawn
		if( enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);

			curr_enemy->pos_x = pos_x;
			curr_enemy->pos_y = 160 + (rand() % 32);
			doublespriteCopy( enemy, curr_enemy );
			doublespriteSetHidden( curr_enemy, false );

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

	for(u32 i = 0; i < 5; i++ ) {
		if( !enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);

			// hit area is hardcoded here for now
			u32 fish_hit_area = 4;
			u32 worm_hit_area = 4;
			
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
