#ifndef ENEMY_BUFFER_H
#define ENEMY_BUFFER_H

#include "sprite_buffer.h"

typedef struct EnemyBuffer {

	// max 5 enemies on screen at the same time
	DoubleSprite enemies[5];	// sprite to use
	u32 movement_kind[5];		// on what kind of path they move on - can be changed on the fly

	// actual empty sprites - handled internally
	Sprite enemy_sprites_A[5];
	Sprite enemy_sprites_B[5];

} EnemyBuffer;


// TODO: rnd
extern void enemybufferInit();

// add enemy to the buffer if there is an empty slot
extern void enemybufferSpawnEnemy(DoubleSprite *enemy, u32 pos_x, u32 movement );

// move them, remove them, do all that can be done
extern void enemybufferUpdateEnemies( Sprite *worm );


#endif // ENEMY_BUFFER_H