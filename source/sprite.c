#include "sprite.h"
#include <string.h> // because of memcpy


static u32 first_free_palbank = 1;


void createSprite( Sprite *out, Animation *anim_arr, const unsigned short *pal ) {
	out->anims = &anim_arr[0];
	out->default_anim_idx = 0;
	out->palette = pal;
	out->palette_startidx = 0;
	out->palette_count = 16;	// use only the first 16 colors
}

static u32 s_LoadPalette( const unsigned short *palette, u32 from_idx, u32 count ) {

	memcpy(&pal_obj_mem[first_free_palbank*16], &palette[from_idx*2], count*2);

	u32 temp_idx = first_free_palbank;
	first_free_palbank++;

	return temp_idx;
}


static void s_SetGfx( OBJ_ATTR *obj_attr, u32 tile_id, u32 pal_id ) {
	obj_attr->attr2 = ATTR2_PALBANK(pal_id) | tile_id;
}


void spriteCopySprite( Sprite *src, Sprite *dst ) {
	
	dst->anims = src->anims;
	dst->default_anim_idx = src->default_anim_idx;
	dst->curr_anim_idx = src->curr_anim_idx;

	dst->palette = src->palette;
	dst->palette_startidx = src->palette_startidx;
	dst->palmem_start_idx = src->palmem_start_idx;

	spriteUpdateGfx( dst );
}

void spriteUpdateGfx( Sprite *obj ) {
	Animation *curr_anim = &obj->anims[obj->curr_anim_idx];
	s_SetGfx(obj->obj_attr, curr_anim->tilemem_start_idx + (curr_anim->curr_frame_idx * curr_anim->tiles_per_frame), obj->palmem_start_idx);
}


inline void spriteLoadPalette( Sprite *obj ) {
	// copying bytes but an entry is actually 2 bytes
	obj->palmem_start_idx = s_LoadPalette(obj->palette, obj->palette_startidx, obj->palette_count);
}

inline void spriteSetPosition( Sprite *obj ) {
	obj_set_pos(obj->obj_attr, obj->pos_x, obj->pos_y);
}

inline void spriteSetHFlipped( Sprite *obj, bool state ) {
	obj->h_mirror = state;
	if( state ) {
		obj->obj_attr->attr1 |= ATTR1_HFLIP;
	} else {
		obj->obj_attr->attr1 &= ~ATTR1_HFLIP;
	}
}

inline void spriteSetHidden( Sprite *obj, bool hide ) {
	if( hide ) {
		obj_hide( obj->obj_attr );
	} else {
		obj_unhide( obj->obj_attr, ATTR0_SQUARE );
	}
	obj->hidden = hide;
}

inline void spriteSetAnimationFrame( Sprite *obj, u32 anim_idx ) {

	// horisontal position reset
	if( obj->h_mirror) {
		obj->pos_x += (obj->anims[obj->curr_anim_idx]).h_offset;
	} else {
		obj->pos_x -= (obj->anims[obj->curr_anim_idx]).h_offset;
	}

	// set the new animation
	obj->curr_anim_idx = anim_idx;
	Animation *curr_anim = &obj->anims[anim_idx];

	if( obj->h_mirror ) {
		obj->pos_x -= curr_anim->h_offset;
	} else {
		obj->pos_x += curr_anim->h_offset;
	}

	animationReset(curr_anim);
	s_SetGfx(obj->obj_attr, curr_anim->tilemem_start_idx + (curr_anim->curr_frame_idx * curr_anim->tiles_per_frame), obj->palmem_start_idx);

}

inline void spriteAdvanceAnimation( Sprite *obj ) {
	Animation *curr_anim = &obj->anims[obj->curr_anim_idx];

	if( animationAdvance( curr_anim ) ) {

		// one-shot animation should change back to the default
		if( !curr_anim->do_loop && curr_anim->done ) {
			spriteSetAnimationFrame( obj, obj->default_anim_idx );
		} else {
			s_SetGfx(obj->obj_attr, curr_anim->tilemem_start_idx + (curr_anim->curr_frame_idx * curr_anim->tiles_per_frame), obj->palmem_start_idx);
		}
	}
}


inline u32 spriteGetCollisionPosX( Sprite *obj ) {
	Animation *curr_anim = &obj->anims[obj->curr_anim_idx];
	if( obj->h_mirror ) {	// can only be mirrorred horizontally
		return obj->pos_x + 16 - curr_anim->coll_x_offset;
	} else {
		return obj->pos_x + curr_anim->coll_x_offset;
	}
}

inline u32 spriteGetCollisionPosY( Sprite *obj ) {
	Animation *curr_anim = &obj->anims[obj->curr_anim_idx];
	return obj->pos_y + curr_anim->coll_y_offset;
}
