#ifndef DOUBLE_SPRITE_H
#define DOUBLE_SPRITE_H

#include <tonc.h>
#include "sprite.h"

typedef struct DoubleSprite {

	// only 16x16@4 for now
	Sprite *left;
	Sprite *right;

	// NOTE: all internal below this point

	// actual sprite position - left and right will be set internally
	u32 pos_x;
	u32 pos_y;

	// need to change the sides and also flip
	bool h_mirror;

	// not on screen
	bool hidden;

} DoubleSprite;

extern void doublespriteCopy( DoubleSprite *src, DoubleSprite *dst );
extern void doublespriteUpdateGfx( DoubleSprite *obj );

extern void doublespriteSetHidden( DoubleSprite *obj, bool hide );
extern void doublespriteSetHFlipped( DoubleSprite *obj, bool state );
extern void doublespriteSetPosition( DoubleSprite *obj );

// convenience
extern void doublespriteSetAnimationFrame( DoubleSprite *obj, u32 anim_idx );
extern void doublespriteAdvanceAnimation( DoubleSprite *obj );

extern u32 doublespriteGetCollisionPosX( DoubleSprite *obj );
extern u32 doublespriteGetCollisionPosY( DoubleSprite *obj );


#endif // DOUBLE_SPRITE_H