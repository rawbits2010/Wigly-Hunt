#include "sprite.h"
#include <string.h> // because of memcpy


static u32 first_free_tile_idx = 0;	// this is actually a byte index
static u32 first_free_palbank = 0;


static u32 s_LoadTiles( const unsigned int *tiles, u32 tile_count, u32 tile_size ) {

	u32 bytesize = tile_count*tile_size;
	memcpy(&tile_mem[4][first_free_tile_idx], tiles, bytesize);

	u32 temp_idx = first_free_tile_idx;
	first_free_tile_idx += tile_count;

	return temp_idx;
}

static u32 s_LoadPalette( const unsigned short *palette, u32 from, u32 len ) {

	memcpy(&pal_obj_mem[first_free_palbank*16], &palette[from], len);

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


inline void spriteLoadTiles( Sprite *obj ) {
	obj->tilemem_start_idx = s_LoadTiles(obj->tiles, obj->frame_count * obj->tiles_per_frame, obj->tile_size);
}

inline void spriteLoadPalette( Sprite *obj ) {
	// copying bytes but an entry is actually 2 bytes
	obj->palmem_start_idx = s_LoadPalette(obj->palette, obj->palette_startidx*2, obj->palette_count*2);
}

inline void spriteSetPosition( Sprite *obj ) {
	obj_set_pos(obj->obj_attr, obj->pos_x, obj->pos_y);
}

inline void spriteSetAnimationFrame( Sprite *obj ) {
	s_SetObject(obj->obj_attr, obj->tilemem_start_idx + (obj->curr_frame * obj->tiles_per_frame), obj->palmem_start_idx);
}
