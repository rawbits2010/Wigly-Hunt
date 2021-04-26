#ifndef ENEMY_BUFFER_H
#define ENEMY_BUFFER_H

#include "sprite_buffer.h"

typedef struct EnemyBuffer {

	// max 5 enemies on screen at the same time
	DoubleSprite enemies[5];	// sprite to use
	u32 enemy_kind[5];

	bool movement_attracted[5];			// overrides the mode in the player get close and gets attracted

	u32 movement_kind[5];				// on what kind of path they move on - can be changed on the fly
	u32 movement_change_cooldown[5];	// wait at least that many frames before have a chance to change

	s32 movement_dir[5];				// -1 or 1
	s32 movement_speed[5];				// in pixels for now

	u32 movement_hover_cooldown[5];		// can keep up with the screen scroll for some frames
	bool movement_hover[5];				// do hover

	// actual empty sprites - handled internally
	Sprite enemy_sprites_A[5];
	Sprite enemy_sprites_B[5];

} EnemyBuffer;


extern void enemybufferInit();

// add enemy to the buffer if there is an empty slot
// TODO: should only use kind, but no time to have correct objs
extern void enemybufferSpawnEnemy(DoubleSprite *enemy, u32 kind, u32 pos_x, u32 movement );

// move them, remove them, do all that can be done
extern void enemybufferUpdateEnemies( Sprite *worm );

// did a fish got us?
extern bool enemybufferDoHitTest( Sprite *worm );
// did we hit a fish - returns the kind of fish or 255 when no hit
extern u32 enemybufferDoSlapTest( Sprite *worm );

#endif // ENEMY_BUFFER_H