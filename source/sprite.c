#include "sprite.h"
#include <string.h> // because of memcpy


static u32 first_free_palbank = 0;


static u32 s_LoadPalette( const unsigned short *palette, u32 from_idx, u32 count ) {

	memcpy(&pal_obj_mem[first_free_palbank*16], &palette[from_idx*2], count*2);

	u32 temp_idx = first_free_palbank;
	first_free_palbank++;

	return temp_idx;
}

// TODO: need something like this but only for palette and anim frame change probably
static void s_SetObject( OBJ_ATTR *obj_attr, u32 tile_id, u32 pal_id ) {

	obj_set_attr(obj_attr, 
		ATTR0_SQUARE,						// Square, regular sprite
		ATTR1_SIZE_16,						// 16x16p, 
		ATTR2_PALBANK(pal_id) | tile_id);	// palbank, starting tile

}


inline void spriteLoadPalette( Sprite *obj ) {
	// copying bytes but an entry is actually 2 bytes
	obj->palmem_start_idx = s_LoadPalette(obj->palette, obj->palette_startidx, obj->palette_count);
}

inline void spriteSetPosition( Sprite *obj ) {
	obj_set_pos(obj->obj_attr, obj->pos_x, obj->pos_y);
}

inline void spriteSetAnimationFrame( Sprite *obj, u32 anim_idx ) {
	obj->curr_anim = anim_idx;
	Animation *curr_anim = &obj->anims[obj->curr_anim];
	animationReset(curr_anim);
	s_SetObject(obj->obj_attr, curr_anim->tilemem_start_idx + (curr_anim->curr_frame_idx * curr_anim->tiles_per_frame), obj->palmem_start_idx);
}

inline void spriteAdvanceAnimation( Sprite *obj ) {
	Animation *curr_anim = &obj->anims[obj->curr_anim];

	if( animationAdvance( curr_anim ) ) {

		// one-shot animation should change back to the default
		if( !curr_anim->do_loop && curr_anim->done ) {
			spriteSetAnimationFrame( obj, obj->default_anim );
		} else {
			s_SetObject(obj->obj_attr, curr_anim->tilemem_start_idx + (curr_anim->curr_frame_idx * curr_anim->tiles_per_frame), obj->palmem_start_idx);
		}
	}
}