#ifndef SPRITE_H
#define SPRITE_H

#include <tonc.h> // actually only types are needed at the momo
#include "animation.h"

// unified 16x16@4 sprites with multiple animation frames
typedef struct Sprite {

	// animation
	Animation *anims;
	u32 curr_anim;

	// points to a place in the buffer - filled by sprite_buffer
	OBJ_ATTR *obj_attr;

	// these are the options you are looking for
	u32 curr_frame;	// current animation frame - zero based
	u32 pos_x;		// storing x position on screen
	u32 pos_y;		// storing y position on screen

} Sprite;

extern void spriteLoadTiles( Sprite *obj );
extern void spriteLoadPalette( Sprite *obj );
extern void spriteSetPosition( Sprite *obj );
extern void spriteAddAnimation( Sprite *obj );

extern void spriteSetAnimationFrame( Sprite *obj );

#endif // SPRITE_H