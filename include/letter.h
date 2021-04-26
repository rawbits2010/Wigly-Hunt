#ifndef LETTER_H
#define LETTER_H

#include <tonc.h>


typedef struct Letter {

	// points to a place in the buffer - filled by sprite_buffer
	OBJ_ATTR *obj_attr;

	u32 tile_id;

	u32 pos_x;
	u32 pos_y;

} Letter;

extern void letterSetPosition( Letter *obj );
extern void letterSetTile( Letter *obj, u32 tile_idx );

#endif // LETTER_H