#include "enemy_buffer.h"

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
			curr_enemy->pos_y = 140 - i*20,
			doublespriteCopy( enemy, curr_enemy );
			doublespriteSetHidden( curr_enemy, false );

			enemy_buff.movement_kind[i] = movement;

			break;
		}

	}
}

void enemybufferUpdateEnemies( Sprite *worm ) {

	for(u32 i = 0; i < 5; i++ ) {
		if( !enemy_buff.enemies[i].hidden ) {
	
			DoubleSprite *curr_enemy = &(enemy_buff.enemies[i]);

			u32 force_x = 0;
			force_x += 1;
			curr_enemy->pos_x += force_x;
	
			doublespriteSetPosition( curr_enemy );
			if( force_x > 0 ) {
				doublespriteSetHFlipped( curr_enemy, true );
			} else if( force_x < 0) {
				doublespriteSetHFlipped( curr_enemy, false );
			}

			// TODO: this is a big hack! don't need to update it every frame
			doublespriteUpdateGfx( curr_enemy );

		}
	}

}