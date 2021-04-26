#include "letter.h"


inline void letterSetPosition( Letter *obj ) {
	obj_set_pos(obj->obj_attr, obj->pos_x, obj->pos_y);
}

inline void letterSetTile( Letter *obj, u32 tile_idx ) {
	obj->obj_attr->attr2 = ATTR2_PALBANK(0) | tile_idx;
}